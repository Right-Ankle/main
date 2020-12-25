#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"

void gnuplot5(double date[64]) {

	static char yn;

	//printf("Can you proceed ? [y/n] :");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {

		FILE* gp;
		static int max, min;
		double temp[64];

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
		fprintf(gp, "set output 'OUTPUT\\gnuplot10.png'\n");
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