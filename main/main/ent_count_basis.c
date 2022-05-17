#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"
void ent_count_basis(double w[64][64], double result_w[3]) {
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, min2 = 0, x[64][64] = { 0 }, sum = 0, sum1 = 0;
	static double b_ent_dct[64];
	static int hist[50000] = { 0 };
	static int hist2[50000] = { 0 };


	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			x[i][j] = w[i][j];

	for (i = 0; i < 64; i++)
		b_ent_dct[i] = 0;

	sum = 0;
	sum1 = 0;

	/* ブロックごとの係数のエントロピーの計算 */
	for (n = 0; n < 3; n++) {

		/* histの初期化 */
		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
			hist2[i] = 0;
		}

		/* histの作成 */
		min = 0;

		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				if (n == 0) {
					if (i == 0)
						hist[(int)(x[j][i] - min) + 1]++;//ステップ幅1
					else
						hist[0 + 1]++;
				}
				if (n == 1) {
					if (i == 0 || i == 1)
						hist[(int)(x[j][i] - min) + 1]++;//ステップ幅1
					else
						hist[0 + 1]++;
				}
				if (n == 2) {
					if (i == 0 || i == 1 || i == 2)
						hist[(int)(x[j][i] - min) + 1]++;//ステップ幅1
					else
						hist[0 + 1]++;
				}
			}
		}

		sum = 0;
		/* エントロピーの計算 */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				sum += -((hist[i] / (double)(64 * 64)) * (log((hist[i] / (double)(64 * 64))) / log(2)));
			}

		result_w[n] = sum;
	}








	//printf("\n%lf\n", sum1);
	///////////////////////////////// dct fin ///////////////////////////////////////////////////

}