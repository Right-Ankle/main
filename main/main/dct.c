/* dct.c */

/********************************************************/
/* gen(���摜�̉�f�l)��DCT��K������,coe(dct�W��)�Ɋi�[	  */
/* �����Ƃ��ė^����ꂽunsigned char�^�̔z��gen��			*/
/* DCT��K�p���v�Z��̒l�������Ƃ��ė^����ꂽdouble�^��	*/
/* �z��coe�Ɋi�[����										*/
/* ����n��DCT�̃u���b�N�̈�ӂ̒���						*/
/*														*/
/********************************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 256
// return coe -> dct�W��256*256
void dct(unsigned char gen[][256], double coe[][256], int n){

	int i, j;//���[�v�J�E���^
	int k, l, x, y;//�v�Z�p
	double c1, c2, sum = 0;//�v�Z�p
	static double tmp[N][N] = {0};//�v�Z�p

	/* double�^�ɃL���X�g���ăR�s�[ */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			/* char > double�֕ϊ� */
			tmp[i][j] = (double)gen[i][j];
		}
	}

	// N > 256 , n -> �u���b�N�̑傫��
	for(k = 0; k < N/n; k++){/* DCT */
		for(l = 0; l < N/n; l++){
			// �u���b�N���̌v�Z
			for(i = 0; i < n; i++){
				for(j = 0; j < n; j++){
					/* �ϊ��s��ւ̏��� */
					for(x = 0; x < n; x++){
						for(y = 0; y < n; y++){
							sum += (tmp[x + (n * k)][y + (n * l)] // �u���b�N�����ꂼ��̉�f�l
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j * M_PI)/ (2.0 * n)));
						}
					}
					// �u���b�N���P�Ԗڂ̉�f�� c1>0.5, c2>0.5, ����ȊO��0.5
					if(i == 0)
						c1 = 1.0 / sqrt(2);
					else
						c1 = 1.0;

					if(j == 0)
						c2 = 1.0 / sqrt(2);
					else
						c2 = 1.0;

					coe[i + (n * k)][j + (n * l)] = c1 * c2 * sum / (n / 2); // n/2 -> ��ʓI�� /4�Ȃ̂�OK
					sum = 0;
					/* 1��f�̕ϊ��s�񏈗��I�� ������dct�W���Ƃ��Ďg�p�\�@�W���͉�f�P��*/
				}
			}
		}
	}

}