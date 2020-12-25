/* b_entropy_ica.c */

/* ブロックごとのyのエントロピーを計算しb_ent_icaに出力します */

#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include <stdio.h>
#include <math.h>
#include "ica.h"

double step = 100.0;
//ブロックごとのyのエントロピーを計算しb_ent_icaに出力します
void b_entropy_ica(double y[64][1024]){
	FILE *fp;
	int i = 0, j = 0, l = 0, m = 0, n = 0;
	static double min = 0, x[64][1024] = { 0 }, b_ent_ica[1024];
	static int hist[50000] = { 0 };

	if ((fp = fopen("b_entropy_ica.csv", "w")) == NULL){
		fprintf(stderr, "Can not open file\n");
	}

	/* 退避 */
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			x[j][i] = y[j][i]; //係数

	/* ブロックごとの係数のエントロピーの計算 */
	for (n = 0; n < 1024; n++){

		/* histの初期化 */
		for (i = 0; i < 50000; i++)
			hist[i] = 0;

		b_ent_ica[n] = 0;

		/* histの作成 */
		min = x[0][n];
		for (i = 1; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n]; //最小値

		for (i = 0; i < 64; i++){
			//hist[(int)((x[i][n]) * step) + 1]++;	//ステップ幅1
			hist[(int)((x[i][n] - min) * step) + 1]++;	//ステップ幅1
		}


		/* エントロピーの計算 */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0)
				b_ent_ica[n] += -(hist[i] / (double)(8 * 8)) * (log(hist[i] / (double)(8 * 8)) / log(2));

		b_ent_ica[n] = b_ent_ica[n] * 64;
	}

	for (i = 0; i < 1024; i++)
		fprintf(fp, "%lf,", b_ent_ica[i]);

	fclose(fp);
}