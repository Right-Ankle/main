/* new_ytoyt.c */

/****************************/
/*   転置行列を計算します。	*/
/****************************/

#include<stdio.h>
// yt -> yyの転置行列（対角線で線対称）
void new_ytoyt(double **yy, double **yt, int data){
	int i, j;

	for(i = 0; i < 64; i++)
		for(j = 0; j < data; j++)
			yt[j][i] = yy[i][j];

}