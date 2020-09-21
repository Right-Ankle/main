/* new_seki.c */

/********************************************/
/*   data�̐��ɉ����čs��̐ς��v�Z���܂��B	*/
/********************************************/

#include<stdio.h>
// yy -> w �� xx�̐�
void new_seki(double w[][64], double **xx, double **yy, int data){

	int i, j, k;

	for(i = 0; i < 64; i++){
		for(j = 0; j < data; j++){
			yy[i][j] = 0.0;
			for(k = 0; k < 64; k++){
				yy[i][j] += w[i][k] * xx[k][j];
			}
		}
	}
}

