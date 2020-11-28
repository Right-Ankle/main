#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"
// date1 のみ使用
void gnuplot4(double date1[7][1024]) {
	FILE* gp;
	static double max1, max2, min1, min2;
	//gp = _popen(GNUPLOT_PATH, "w");

	for (int i = 0; i < 1024; i++) {
		printf(" plot start - - - ->");

		gp = _popen("gnuplot -persist", "w"); // パイプを開き、gnuplotの立ち上げ

		// gnuplotにコマンドを送る
		//fprintf(gp, "set terminal png color font 'VL PGothic,20' enhanced\n");+
		fprintf(gp, "set terminal png\n");
		fprintf(gp, "set output 'OUTPUT\\test\\[%d].png'\n", i);
		//fprintf(gp, "set boxwidth 0.5 relative\n");
		//fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-1:7]\n");
		fprintf(gp, "set yrange[-10:110]\n");
		//fprintf(gp, "set y2range[%lf:%lf]\n", min2, max2);
		//fprintf(gp, "set xlabel 'ICA coefficient (descending)'\n");
		//fprintf(gp, "set ylabel 'MSE'\n");

		fprintf(gp, "plot '-' with linespoints lw 3 pointtype 3\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 7; j++)
			if (j >= 1 && date1[j][i] == 0)
				;
			else
				fprintf(gp, "%d\t%lf\n", j, date1[j][i]);    // データの書き込み 			fprintf(gp, "%d\t%lf\n", j, date1[j][i]);    // データの書き込み
		fprintf(gp, "e\n");

		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // バッファに格納されているデータを吐き出す（必須）
		_pclose(gp);
		//exit(EXIT_SUCCESS);
		printf("  plot end\n\n");
	}
	return 0;
}