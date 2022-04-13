/* quantization.c */

#include<stdio.h>

#define N 256
//量子化
void quantization(double coe[][N], int Q){

	double qcoe[N][N] = { 0 };
	int n, i, j, k, l, x, y, S, Qb[256][256] = { 0 };
	int Ts[8][8] = {0}, Tb[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
									{12, 12, 14, 19, 26, 58, 60, 55},
									{14, 13, 16, 24, 40, 57, 69, 56},
									{14, 17, 22, 29, 51, 87, 80, 62},
									{18, 22, 37, 56, 68, 109, 103, 77},
									{24, 35, 55, 64, 81, 104, 113, 92},
									{49, 64, 78, 87, 103, 121, 120, 101},
									{72, 92, 95, 98, 112, 100, 103, 99}};

	n = 8;//分割数
	if (Q == 0)
		Q = 1;

	if(Q < 50)/* スケーリングファクタの計算 */
		S = 5000 / Q;
	else
		S = 200 - 2 * Q;

	for(i = 0; i < 8; i++){/* 量子化テーブルの決定 */
		for(j = 0; j < 8; j++){
			Ts[i][j] = (S * Tb[i][j] + 50) / 100;
			if(Ts[i][j] == 0)
				Ts[i][j] = 1;
		}
	}


	//for(i = 0; i < 8; i++){/* 量子化テーブルの表示 */
	//	for(j = 0; j < 8; j++){
	//		printf("%3d ", Ts[i][j]);
	//	}
	//	printf("\n");
	//}

	for(i = 0; i < 32; i++)/* 量子化 */
		for(j = 0; j < 32; j++)
			for(x = 0; x < n; x++)
				for(y = 0; y < n; y++)
					Qb[x + (n * i)][y + (n * j)] = coe[x + (n * i)][y + (n * j)] / (double)(Ts[x][y]);


	for(i = 0; i < 32; i++)/* 逆量子化 */
		for(j = 0; j < 32; j++)
			for(x = 0; x < n; x++)
				for(y = 0; y < n; y++)
					qcoe[x + (n * i)][y + (n * j)] = Qb[x + (n * i)][y + (n * j)] * (double)Ts[x][y];

	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			coe[i][j] = (double)qcoe[i][j];
}