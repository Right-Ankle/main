/* makew.c */

/*********************************************/
/*  �����s��������_���l�i0�`1�j�Ő������܂��B	 */
/*********************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES//M_PI���g�����߂̒�`
#include<math.h>
#include<time.h>
#include"MT.h"//��l���z�𔭐������郁���Z���k�c�C�X�^���g�����߂̃w�b�_�t�@�C��

// w -> �����s��������_���l�i0�`1�j�Ő���
void makew(double w[][64]){
	int i,j;

	init_genrand(82405);

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			w[i][j] = genrand_real3();
		}
	}
}