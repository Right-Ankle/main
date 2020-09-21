/* avg_inter.c */

/********************************************/
/*�č\�����ꂽ�摜�ɒ����������U���܂�*/
/********************************************/


#include <stdio.h>
#include "ica.h"

// 256*256 return gen
void avg_inter(unsigned char gen[][256], double avg[1024]){
	int i, j;
	double sum[1024] = {0}, x[64][1024] = {0};

	// x -> ica 64*1024
	gentox(gen, x);

    // sum -> �u���b�N���̕���
	for(i = 0; i < 1024; i++){
		for(j = 0; j < 64; j++){
			sum[i] += x[j][i];
		}
		sum[i] /= 64.0;
	}

	for(i = 0; i < 1024; i++)
		sum[i] = avg[i] - sum[i];

	// gen -> ica + sum 256*256(0~255)
	xtogen(x, gen, sum);
}

