#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"
void ent_count_basis(double w[64][64]) {
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, min2 = 0, x[64][1024] = { 0 }, sum = 0, sum1 = 0;
	static double b_ent_dct[64];
	static int hist[50000] = { 0 };
	static int hist2[50000] = { 0 };


	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			x[i][j] = w[i][j];

	sum = 0;
	sum1 = 0;
	/* ブロックごとの係数のエントロピーの計算 */
	for (n = 0; n < 64; n++) {

		/* histの初期化 */
		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
			hist2[i] = 0;
		}

		b_ent_dct[n] = 0;

		/* histの作成 */
		min = x[n][0];
		for (i = 1; i < 64; i++)
			if (x[n][i] < min)
				min = x[n][i]; // histの左端

		for (i = 0; i < 64; i++) {
			//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
			hist[(int)(x[n][i] - min) + 1]++;	//ステップ幅1
		}


		/* エントロピーの計算 */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				b_ent_dct[n] += -((hist[i] / (double)(64)) * (log((hist[i] / (double)(64))) / log(2)));
				sum1 += -((hist[i] / (double)(64)) * (log((hist[i] / (double)(64))) / log(2)));
			}
		sum += sum1;
		b_ent_dct[n] = b_ent_dct[n];
	}

	sum = 0;
	sum1 = 0;
	for (i = 0; i < 64; i++) {
		printf("\n[%d] : %lf\n", i, b_ent_dct[i] / 64);
		sum += b_ent_dct[i]/64;
	}
	printf("\n%lf\n", sum / (64));



	//printf("\n%lf\n", sum1);
	///////////////////////////////// dct fin ///////////////////////////////////////////////////

}