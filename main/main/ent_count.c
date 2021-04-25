#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"
void ent_count(double y[256][256], double ica_dc[1024]) {
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, min2 = 0, x[64][1024] = { 0 }, sum = 0, sum1 = 0;
	static double b_ent_dct[1024];
	static int hist[100000] = { 0 };
	static int hist2[100000] = { 0 };

/////////////////////////////////// dct /////////////////////////////////////////////////
	for (i = 0; i < 256; i += 8) {
		for (j = 0; j < 256; j += 8) {
			m = 0;
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {
					x[m][n] = y[i + k][j + l]; //256*256 -> 64*1024

					m++;
				}
			}
			n++;
		}
	}

	sum = 0;
	sum1 = 0;
	/* ブロックごとの係数のエントロピーの計算 */
	for (n = 0; n < 1024; n++) {

		/* histの初期化 */
		for (i = 0; i < 100000; i++) {
			hist[i] = 0;
			hist2[i] = 0;
		}

		b_ent_dct[n] = 0;

		/* histの作成 */
		min = x[1][n];
		for (i = 2; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n]; // histの左端

		for (i = 1; i < 64; i++) {
			//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
			hist[(int)(x[i][n] - min) + 1]++;	//ステップ幅1
		}


		/* エントロピーの計算 */
		for (i = 0; i < 100000; i++)
			if (hist[i] > 0) {
				b_ent_dct[n] += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));
				sum1 += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));
			}
		sum += sum1;
		b_ent_dct[n] = b_ent_dct[n] * 63;
	}

	/* hist2の作成 */
	min2 = x[0][0];
	for (i = 0; i < 1024; i++)
		if (x[0][i] < min2)
			min2 = x[0][i]; // histの左端

	for (i = 0; i < 1024; i++) {
		//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
		hist2[(int)(x[0][i] - min2) + 1]++;	//ステップ幅1
	}

	sum1 = 0;
	/* エントロピーの計算 */
	for (i = 0; i < 100000; i++)
		if (hist2[i] > 0) {
			sum1 += -((hist2[i] / (double)(1024)) * (log((hist2[i] / (double)(1024))) / log(2)));
		}
	//printf("\n%lf\n", sum1 / 64);

	//printf("\n%lf\n", sum / (1024*1024));
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_ent_dct[i];

	printf("\n%lf\n", sum / (256*256));
	printf("\n%lf\n", (sum / (256 * 256)) + (sum1 / 64));

	//printf("\n%lf\n", sum1);
	///////////////////////////////// dct fin ///////////////////////////////////////////////////

	/* histの初期化 */
	for (i = 0; i < 100000; i++) {
		hist[i] = 0;
		hist2[i] = 0;
	}
	int step = 100;
	sum1 = 0;

	/* hist2の作成 */
	min2 = ica_dc[0];
	for (i = 0; i < 1024; i++)
		if (ica_dc[i] < min2)
			min2 = ica_dc[i]; // histの左端

	for (i = 0; i < 1024; i++) {
		//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
		hist2[(int)((ica_dc[i] - min2))+1]++;	//ステップ幅1
	}

	/* エントロピーの計算 */
	for (i = 0; i < 100000; i++)
		if (hist2[i] > 0) {
			sum1 += -((hist2[i] / (double)(1024)) * (log((hist2[i] / (double)(1024))) / log(2)));
		}

	printf("\nica_dc : %lf\n", sum1 / 64);

	/* hist2の作成 */
	static double min3 = 0;
	sum1 = 0;
	min3 = x[0][0];
	for (j = 0; j < 64; j++)
		for (i = 0; i < 1024; i++)
			if (x[j][i] < min3)
				min3 = x[j][i]; // histの左端

	for (j = 0; j < 64; j++)
		for (i = 0; i < 1024; i++) {
			//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
			hist[(int)(x[j][i] - min3) + 1]++;	//ステップ幅1
		}

	for (i = 0; i < 100000; i++)
		if (hist[i] > 0) {
			sum1 += -((hist[i] / (double)(1024*64)) * (log((hist[i] / (double)(1024*64))) / log(2)));
		}

	printf("\nall_ent = %lf\n", sum1);

}