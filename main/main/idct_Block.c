/* idct_Block.c */

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
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>

#define N 8
// 逆離散コサイン変換 return -> (char)sai
void idct_Block(double coe[64][1024], unsigned char sai[64], int n , int num){
	FILE* fp3, *fp;

	int i, j, a, b;//ループカウンタ
	int k, l, x, y,nk,nl;//計算用
	double c1, c2, sum = 0;//計算用
	static double tmp[N][N] = {0};//計算用

	double max=0,min=0;

	k = num % 32;
	l = num / 32;

	if ((fp3 = fopen("block_dct.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp = fopen("coe.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	// 係数確認（問題なし）
	//	for (a = 0; a < 64; a++)
	//		fprintf(fp, "%d\n", (int)coe[a][num]);

	//fprintf(fp, "---------------------------------------------\n\n\n\n");

//	for(k = 0; k < N/n; k++){/* IDCT */
//		for(l = 0; l < N/n; l++){
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
						fprintf(fp3,"%d\n",(int)coe[i* n + j][num]);
						sum += (coe[i*n + j][num] * c1 * c2
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j* M_PI)/ (2.0 * n)));
						}
					}
					tmp[x][y] = sum / (n / 2);
					sum = 0;
				}
			}
//		}
//	}

	/* unsigned char型にキャストしてコピー */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			// 輝度値の調整
			if(tmp[i][j] < 0)
				tmp[i][j] = 0;
			if(tmp[i][j] > 255)
				tmp[i][j] = 255;
			sai[j * n + i] = (unsigned char)tmp[i][j];

			//fprintf(fp3,"%d\n",saiji * n + j]);
		}
	}
	fclose(fp3);
	fclose(fp);
}