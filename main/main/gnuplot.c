#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"

void gnuplot(double date[64][1024]) {
	FILE* gp;
	static double temp[64], max, min;
	//gp = _popen(GNUPLOT_PATH, "w");
	printf("\n+ - - - - - Now Plotting - - - - +\n");
	for (int i = 0; i < 1024; i++) {
		//printf("%d\n", i);
		for (int j = 0; j < 64; j++) {
			temp[j] = date[j][i];
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
		if (max < 1)
			min = 1;
		else
			max = max + max / 5;

		if (min > 0)
			min = 0;
		else {
			if (min > -1)
				min = -1;
			else
				min = min + min / 5;
		}

		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る
		//fprintf(gp, "set terminal png color font 'VL PGothic,20' enhanced\n");
		fprintf(gp, "set terminal png\n");
		fprintf(gp, "set output 'OUTPUT\\test\\area[%d].png'\n", i);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min, max);
		fprintf(gp, "set xlabel 'ICA coefficient (descending)'\n");
		fprintf(gp, "set ylabel 'MSE'\n");

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
		if (i % 64 == 0)
			printf(" @");
	}
	printf("\n\n");
	return 0;
}