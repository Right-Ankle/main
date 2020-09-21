/* new_whitening.c */

/************************/
/*   白色化を行います。	*/
/*　白色化：各成分を互いに無相関にし、*/
/*　　　　　平均を０、分散を１にする。*/
/************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ica.h"

void new_seki(double w[][64], double **xx, double **yy, int data);
void seki3(double q[][64], double w[][64], double dw[][64]);
void inv(double w[][64], double inv_w[][64]);
// ica.c -> xx = 白色化, v = 固有ベクトルの逆行列と固有値対角行列 の積の逆行列,
// p = 固有ベクトルの逆行列と固有値対角行列 の積, date = 1024,
// 白色化(Whitening)は生データの要素間の相関をなくし、さらに分散の正規化を前処理として実行することで学習の効率化を狙う
void new_whitening(struct pca new_pca, double **xx, double v[][64], double p[][64], int data){

	static double **temp;
	static double d[64][64] = {0}, e[64][64] = {0}, temp2[64][64] = {0};  //d -> 固有値の対角行列,
	int i, j;

	temp = (double **)malloc(sizeof(double) * 64);
	for (i = 0; i < 64; i++) {
		temp[i]  = (double *)malloc(sizeof(double) * data);
	}
    // xx -> tempへコピー
	for(i = 0; i < 64; i++)
		for(j = 0; j < data; j++)
			temp[i][j] = xx[i][j];
    // d -> √した固有値を対角行列に変換
	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			if(i == j)
				d[i][j] = sqrt(new_pca.r[i]);
    // e -> 固有ベクトルの転置
	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			e[j][i] = new_pca.a[i][j];
    // p -> eとdの積 (q -> 固有ベクトルの転置, w -> √した固有値を対角行列に変換)
	seki3(e, d, p);

	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			temp2[i][j] = p[i][j]; //temp2 -> 白色化

	inv(temp2, v); // temp2 -> 単位行列, v -> temp2の逆行列
	new_seki(v, temp, xx, data); // 固有ベクトルの転置と√した固有値対角行列 の積　の逆行列 と 中心化　の積 -> 白色化

	// 解放処理
	for(i = 0; i < 64; i++)
		free(temp[i]);
	free(temp);

}
