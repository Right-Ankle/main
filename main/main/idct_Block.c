/* idct_Block.c */

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
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>

#define N 8
// �t���U�R�T�C���ϊ� return -> (char)sai
void idct_Block(double coe[64][1024], unsigned char sai[64], int n , int num){
	FILE* fp3, *fp;

	int i, j, a, b;//���[�v�J�E���^
	int k, l, x, y,nk,nl;//�v�Z�p
	double c1, c2, sum = 0;//�v�Z�p
	static double tmp[N][N] = {0};//�v�Z�p

	double max=0,min=0;

	k = num % 32;
	l = num / 32;

	if ((fp3 = fopen("block_dct.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp = fopen("coe.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	// �W���m�F�i���Ȃ��j
	//	for (a = 0; a < 64; a++)
	//		fprintf(fp, "%d\n", (int)coe[a][num]);

	//fprintf(fp, "---------------------------------------------\n\n\n\n");

//	for(k = 0; k < N/n; k++){/* IDCT */
//		for(l = 0; l < N/n; l++){
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
						fprintf(fp3,"%d\n",(int)coe[i* n + j][num]);
						sum += (coe[i*n + j][num] * c1 * c2
							* cos(((2.0 * x + 1.0) * i * M_PI)/ (2.0 * n))
							* cos(((2.0 * y + 1.0) * j* M_PI)/ (2.0 * n)));
						}
					}
					tmp[x][y] = sum / (n / 2);
					sum = 0;
				}
			}
//		}
//	}

	/* unsigned char�^�ɃL���X�g���ăR�s�[ */
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			// �P�x�l�̒���
			if(tmp[i][j] < 0)
				tmp[i][j] = 0;
			if(tmp[i][j] > 255)
				tmp[i][j] = 255;
			sai[j * n + i] = (unsigned char)tmp[i][j];

			//fprintf(fp3,"%d\n",saiji * n + j]);
		}
	}
	fclose(fp3);
	fclose(fp);
}