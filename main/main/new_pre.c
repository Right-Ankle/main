/* new_pre.c */

/****************************/
/*   ���S���������s���܂��B	*/
/****************************/

#include<stdio.h>
#include<math.h>
// xx -> (���̉�f�l - ���ϒl), avg -> �u���b�N���Ƃ̕��ϒl,
void new_pre(double **xx, double *avg, int data){
	double sum;
	int i, j;
    // ica.c -> date = 1024
	for(i = 0; i < data; i++){

		sum = 0;

		for(j = 0; j < 64; j++)
			sum += xx[j][i];

		avg[i] = sum / 64.0; //�u���b�N���Ƃ̕��ϒl

		for(j = 0; j < 64; j++)
			xx[j][i] = (xx[j][i] - avg[i]); // xx <- (���̉�f�l - ���ϒl)
	}
}

