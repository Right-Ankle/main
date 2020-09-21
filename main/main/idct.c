/* idct.c */

/*******************************************************/
/*										               */
/* �����Ƃ��ė^����ꂽdouble�^�̔z��coe��IDCT��K�p�� */
/* �v�Z��̒l��,�����Ƃ��ė^����ꂽunsigned char�^��  */
/* �z��sai�Ɋi�[����								   */
/* ����n��IDCT�̃u���b�N�̈�ӂ̒���				   */
/*													   */
/*******************************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 256
// �t���U�R�T�C���ϊ� return -> (char)sai
void idct(double coe[][N], unsigned char sai[][N], int n){

	int i, j;//���[�v�J�E���^
	int k, l, x, y;//�v�Z�p
	double c1, c2, sum = 0;//�v�Z�p
	static double tmp[N][N] = {0};//�v�Z�p

	double max=0,min=0;

	for(k = 0; k < N/n; k++){/* IDCT */
		for(l = 0; l < N/n; l++){
			for(x = 0; x < n; x++){
				for(y = 0; y < n; y++){
					for(i = 0; i < n; i++){
						for(j = 0; j < n; j++){
						if(i == 0)
							c1 = 1.0 / sqrt(2);
						else
							c1 = 1.0;

						if(j == 0)
							c2 = 1.0 / sqrt(2);
						else
							c2 = 1.0;

						sum += (coe[i + (n * k)][j + (n * l)] * c1 * c2
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j * M_PI)/ (2.0 * n)));
						}
					}
					tmp[x + (n * k)][y + (n * l)] = sum / (n / 2);
					sum = 0;
				}
			}
		}
	}
    // �ő�ŏ��̊m�F
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(tmp[i][j] > max)
				max = tmp[i][j];
			if(tmp[i][j] < min)
				min = tmp[i][j];
		}
	}

	/* unsigned char�^�ɃL���X�g���ăR�s�[ */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			// �P�x�l�̒���
			if(tmp[i][j] < 0)
				tmp[i][j] = 0;
			if(tmp[i][j] > 255)
				tmp[i][j] = 255;
			sai[i][j] = (unsigned char)tmp[i][j];
		}
	}
}