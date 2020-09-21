/* new_seki2.c */

/********************************************/
/*   data�̐��ɉ����čs��̐ς��v�Z���܂��B	*/
/********************************************/

#include<stdio.h>
// z -> sig��yt�̍s��̐�
void new_seki2(double **sig, double **yt, double z[][64], int data){

	int i, j, k;

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			z[i][j] = 0.0; //������
			for(k = 0; k < data; k++){
				z[i][j] += sig[i][k] * yt[k][j];
			}
		}
	}
}