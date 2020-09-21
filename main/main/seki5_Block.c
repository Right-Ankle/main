/* seki5_Block.c */

/****************************/
/*   �s��̐ς��v�Z���܂��B	*/
/****************************/

#include<stdio.h>
// y -> w64*64 �� x64*1024 �̐�
void seki5_Block(double w[][64], double x[][1024], double y[64], int num){

	int i, j, k;

	for (i = 0; i < 64; i++) {
		for (k = 0; k < 64; k++) {
			y[i] += w[i][k] * x[k][num];
		}
	}
}

