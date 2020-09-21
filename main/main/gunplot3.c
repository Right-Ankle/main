#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"
// date1 のみ使用
void gnuplot3(double date1[1024], double date2[1024]) {
	FILE* gp;
	static double max1, max2, min1, min2;
	//gp = _popen(GNUPLOT_PATH, "w");
	printf("----plot start----\n");
	for (int j = 0; j < 1024; j++) {
		if (j == 0) {
			max1 = date1[j];
			min1 = date1[j];
		}
		else {
			if (max1 < date1[j])
				max1 = date1[j];
			if (min1 > date1[j])
				min1 = date1[j];
		}
	}
	if (max1 == 0)
		max1 = 1;
	else
		max1 = max1 + max1 / 3;

		if (min1 > 0)
			min1 = 0;
		else
			min1 = min1 + min1 / 3;

		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplotにコマンドを送る
		//fprintf(gp, "set terminal png color font 'VL PGothic,20' enhanced\n");+
		fprintf(gp, "set terminal png\n");
		fprintf(gp, "set output 'OUTPUT\\test\\COE.png'\n");
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:1026]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min1, max1);
		//fprintf(gp, "set y2range[%lf:%lf]\n", min2, max2);
		//fprintf(gp, "set xlabel 'ICA coefficient (descending)'\n");
		//fprintf(gp, "set ylabel 'MSE'\n");

		fprintf(gp, "plot '-' with boxes lw 1 \n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 1024; j++)
			fprintf(gp, "%d\t%lf\n", j, date1[j]);    // データの書き込み
		fprintf(gp, "e\n");

		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);

	return 0;
}