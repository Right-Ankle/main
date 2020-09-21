/* idct.c */

/*******************************************************/
/*										               */
/* 引数として与えられたdouble型の配列coeにIDCTを適用し */
/* 計算後の値を,引数として与えられたunsigned char型の  */
/* 配列saiに格納する								   */
/* 引数nはIDCTのブロックの一辺の長さ				   */
/*													   */
/*******************************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 256
// 逆離散コサイン変換 return -> (char)sai
void idct(double coe[][N], unsigned char sai[][N], int n){

	int i, j;//ループカウンタ
	int k, l, x, y;//計算用
	double c1, c2, sum = 0;//計算用
	static double tmp[N][N] = {0};//計算用

	double max=0,min=0;

	for(k = 0; k < N/n; k++){/* IDCT */
		for(l = 0; l < N/n; l++){
			for(x = 0; x < n; x++){
				for(y = 0; y < n; y++){
					for(i = 0; i < n; i++){
						for(j = 0; j < n; j++){
						if(i == 0)
							c1 = 1.0 / sqrt(2);
						else
							c1 = 1.0;

						if(j == 0)
							c2 = 1.0 / sqrt(2);
						else
							c2 = 1.0;

						sum += (coe[i + (n * k)][j + (n * l)] * c1 * c2
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j * M_PI)/ (2.0 * n)));
						}
					}
					tmp[x + (n * k)][y + (n * l)] = sum / (n / 2);
					sum = 0;
				}
			}
		}
	}
    // 最大最小の確認
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(tmp[i][j] > max)
				max = tmp[i][j];
			if(tmp[i][j] < min)
				min = tmp[i][j];
		}
	}

	/* unsigned char型にキャストしてコピー */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			// 輝度値の調整
			if(tmp[i][j] < 0)
				tmp[i][j] = 0;
			if(tmp[i][j] > 255)
				tmp[i][j] = 255;
			sai[i][j] = (unsigned char)tmp[i][j];
		}
	}
}