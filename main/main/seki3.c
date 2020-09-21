/* seki3.c */

/****************************/
/*   s—ñ‚ÌÏ‚ğŒvZ‚µ‚Ü‚·B	*/
/****************************/

#include<stdio.h>
// dw -> q64*64‚Æw64*64‚ÌÏ
void seki3(double q[][64], double w[][64], double dw[][64]){

	int i, j, k;

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			dw[i][j] = 0.0;
			for(k = 0; k < 64; k++){
				dw[i][j] += q[i][k] * w[k][j];
			}
		}
	}
}