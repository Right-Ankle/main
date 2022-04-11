#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"

void gnuplot5(double date[64], int name) {
	char out[50];
	static char yn;

	//printf("Can you proceed ? [y/n] :");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {

		FILE* gp;
		static double max, min;
		double temp[64];

		//gp = _popen(GNUPLOT_PATH, "w");
		//printf("----plot start----\n");
		for (int j = 0; j < 64; j++) {
			temp[j] = date[j];
			if (j == 0) {
				max = temp[j];
				min = temp[j];
			}
			else {
				if (max < temp[j])
					max = temp[j];
				if (min > temp[j])
					min = temp[j];
			}
		}

		if (max < 0)
			max = max - max / 5;
		else
			max = max + max / 5;

		if (min > 0)
			min = min - min / 5;
		else
			min = min + min / 5;


		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\MSE\\plot[%d].png", name);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min - 0.5, max + 0.5);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%lf\n", j, temp[j]);    // データの書き込み
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	}
}

void gnuplot5_2(double date[64], int name) {
	char out[50];
	static char yn;

		FILE* gp;
		static double max, min;
		double temp[64], origin[64];//temp=正規分布、origin=降順

		int a, b, c, d, e, i, j;
		double k, l, m, n;

		//gp = _popen(GNUPLOT_PATH, "w");

		for (i = 0; i < 64; i++)
			origin[i] = fabsf(date[i]);

		//係数値を正規分布に直す
		for (i = 0; i < 64; i++) {
			max = 0;
			for (j = i; j < 64; j++)
				if (max < fabsf(origin[j])) {
					max = fabsf(origin[j]);
					a = j;
				}
			origin[a] = origin[i];
			origin[i] = max;
			if (i == 0)
				temp[32] = max;
			else if (i % 2 == 1)
				temp[32 - ((i + 1) / 2)] = max;
			else
				temp[32 + (i / 2)] = max;
		}

		// 最大最小チェック
		for (j = 0; j < 64; j++) {
			if (j == 0) {
				max = fabsf(date[j]);
				min = fabsf(date[j]);
			}
			else {
				if (max < fabsf(date[j]))
					max = fabsf(date[j]);
				if (min > fabsf(date[j]))
					min = fabsf(date[j]);
			}
		}

		if (max < 0)
			max = max - max / 5;
		else
			max = max + max / 5;

		if (min > 0)
			min = min - min / 5;
		else
			min = min + min / 5;



		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\MSE\\plot[%d].png", name);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min, max);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%lf\n", j, temp[j]);    // データの書き込み
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);

		//尖度の計算
		k = l = m = n = 0;
		for (i = 0; i < 64; i++)
			k += temp[i];
		k = k / 64; //平均

		//標準偏差
		for (i = 0; i < 64; i++)
			l += pow((temp[i] - k),2);
		l = l / 64;
		l = sqrt(l);

		for (i = 0; i < 64; i++) {
			m += (pow((temp[i]-k),4)/pow(l,4)) - ((double)(3*(64-1)*(64-1))/(double)((64-2)*(64-3)));
		}
		k = ((double)(64 * 65) / (double)(63 * 62 * 61));
		n = k * m -3;

		printf("\n [%d] = %lf", name, n);

}