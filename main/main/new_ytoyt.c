/* new_ytoyt.c */

/****************************/
/*   �]�u�s����v�Z���܂��B	*/
/****************************/

#include<stdio.h>
// yt -> yy�̓]�u�s��i�Ίp���Ő��Ώ́j
void new_ytoyt(double **yy, double **yt, int data){
	int i, j;

	for(i = 0; i < 64; i++)
		for(j = 0; j < data; j++)
			yt[j][i] = yy[i][j];

}