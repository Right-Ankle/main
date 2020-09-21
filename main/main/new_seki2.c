/* new_seki2.c */

/********************************************/
/*   dataの数に応じて行列の積を計算します。	*/
/********************************************/

#include<stdio.h>
// z -> sigとytの行列の積
void new_seki2(double **sig, double **yt, double z[][64], int data){

	int i, j, k;

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			z[i][j] = 0.0; //初期化
			for(k = 0; k < data; k++){
				z[i][j] += sig[i][k] * yt[k][j];
			}
		}
	}
}