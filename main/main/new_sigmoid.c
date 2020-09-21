/* new_sigmoid.c */

/************************************/
/*   y �ɃV�O���C�h�֐���K�p���܂��B */
/************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES//M_PI���g�����߂̒�`
#include<math.h>
// sig -> yy�𗘗p�����V�O���C�h�֐�
void new_sigmoid(double **yy, double **sig, int data)
{
	// �V�O���C�h�֐� -> �P������, f(0,0.5)�œ_�Ώ�
    //                 lim->�� = 1, lim->-�� = 0
	//                 ���͂ɑ΂���0~1�͈̔͂ŏo��
	int i, j;

	for(i = 0; i < 64; i++){
		for(j = 0; j < data; j++){
			sig[i][j] = 1.0 / (1.0 + exp(-1.0 * yy[i][j]));
		}
	}

}