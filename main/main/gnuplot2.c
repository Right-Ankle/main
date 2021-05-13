#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"

void gnuplot2(int date[64], int Q) {


	static char yn;
	char out[50];



		FILE* gp;
		static int temp[64], max, min;

		//gp = _popen(GNUPLOT_PATH, "w");
		printf("----plot start----\n");
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



		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\plot[%d].png", Q);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", -2.0, max + 2.0);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%d\n", j, temp[j]);    // データの書き込み
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	
}


void gnuplot2_2(int date[10][64]) {
	FILE* fp;
	static char yn;
	int k,l,m,i;
	FILE* gp;
	static int temp[10][64];
	double max, min;
	char out[50];

	if ((fp = fopen("OUTPUT\\dct_basis_map.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}


	for (k = 0; k < 10;k++) {

		for (i = 0; i < 64; i++) {
			if (i % 8 == 0)
				fprintf(fp, "\n");
			fprintf(fp, "%lf,", (double)date[k][i]);
		}

		fprintf(fp, "\n");

		//gp = _popen(GNUPLOT_PATH, "w");
		printf("----plot start----\n");
		for (int j = 0; j < 64; j++) {
			temp[k][j] = date[k][j];
			if (j == 0) {
				max = temp[k][j];
				min = temp[k][j];
			}
			else {
				if (max < temp[k][j])
					max = temp[k][j];
				if (min > temp[k][j])
					min = temp[k][j];
			}
		}



		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\plot[%d].png", k);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", -2.0, max + 5);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%d\n", j, temp[k][j]);    // データの書き込み
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	}
	fclose(fp);
}
