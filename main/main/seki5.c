/* seki5.c */

/****************************/
/*   s—ñ‚ÌÏ‚ğŒvZ‚µ‚Ü‚·B	*/
/****************************/

#include<stdio.h>
// y -> w64*64 ‚Æ x64*1024 ‚ÌÏ
void seki5(double w[][64], double x[][1024], double y[][1024]){

	int i, j, k;

	for(i = 0; i < 1024; i++){
		for(j = 0; j < 64; j++){
			y[j][i] = 0.0;
			for(k = 0; k < 64; k++){
				y[j][i] += w[j][k] * x[k][i];
			}
		}
	}
}

