/* dct.c */

/********************************************************/
/* gen(原画像の画素値)にDCTを適応させ,coe(dct係数)に格納	  */
/* 引数として与えられたunsigned char型の配列genに			*/
/* DCTを適用し計算後の値を引数として与えられたdouble型の	*/
/* 配列coeに格納する										*/
/* 引数nはDCTのブロックの一辺の長さ						*/
/*														*/
/********************************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 256
// return coe -> dct係数256*256
void dct(unsigned char gen[][256], double coe[][256], int n){

	int i, j;//ループカウンタ
	int k, l, x, y;//計算用
	double c1, c2, sum = 0;//計算用
	static double tmp[N][N] = {0};//計算用

	/* double型にキャストしてコピー */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			/* char > doubleへ変換 */
			tmp[i][j] = (double)gen[i][j];
		}
	}

	// N > 256 , n -> ブロックの大きさ
	for(k = 0; k < N/n; k++){/* DCT */
		for(l = 0; l < N/n; l++){
			// ブロック内の計算
			for(i = 0; i < n; i++){
				for(j = 0; j < n; j++){
					/* 変換行列への処理 */
					for(x = 0; x < n; x++){
						for(y = 0; y < n; y++){
							sum += (tmp[x + (n * k)][y + (n * l)] // ブロック内それぞれの画素値
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j * M_PI)/ (2.0 * n)));
						}
					}
					// ブロック内１番目の画素は c1>0.5, c2>0.5, それ以外は0.5
					if(i == 0)
						c1 = 1.0 / sqrt(2);
					else
						c1 = 1.0;

					if(j == 0)
						c2 = 1.0 / sqrt(2);
					else
						c2 = 1.0;

					coe[i + (n * k)][j + (n * l)] = c1 * c2 * sum / (n / 2); // n/2 -> 一般的に /4なのでOK
					sum = 0;
					/* 1画素の変換行列処理終了 処理後dct係数として使用可能　係数は画素単位*/
				}
			}
		}
	}

}