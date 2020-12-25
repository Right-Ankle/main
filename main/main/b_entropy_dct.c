/* b_entropy_dct.c */

/* ブロックごとのyのエントロピーを計算しb_ent_dctに出力します */

#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include <stdio.h>
#include <math.h>
#include "ica.h"

//エントロピー計算
void b_entropy_dct(double y[][256]){
	FILE *fp;
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, x[64][1024] = { 0 }, b_ent_dct[1024];
	static int hist[50000] = { 0 };

	if ((fp = fopen("b_entropy_dct.csv", "w")) == NULL){
		fprintf(stderr, "Can not open file\n");
	}

	/* 退避 */
	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++)
	//		x[j][i] = y[j][i];


	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			m = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					x[m][n] = y[i + k][j + l];

					m++;
				}
			}
			n++;
		}
	}

	/* ブロックごとの係数のエントロピーの計算 */
	for (n = 0; n < 1024; n++){

		/* histの初期化 */
		for (i = 0; i < 50000; i++)
			hist[i] = 0;

		b_ent_dct[n] = 0;

		/* histの作成 */
		min = x[1][n];
		for (i = 2; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n];

		for (i = 1; i < 64; i++){
			//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
			hist[(int)(x[i][n] - min) + 1]++;	//ステップ幅1
		}


		/* エントロピーの計算 */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0)
				b_ent_dct[n] += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));

		b_ent_dct[n] = b_ent_dct[n] * 63;
	}

	for (i = 0; i < 1024; i++)
		fprintf(fp, "%lf,", b_ent_dct[i]);

	fclose(fp);


}