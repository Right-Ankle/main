/* b_entropy_dct.c */

/* �u���b�N���Ƃ�y�̃G���g���s�[���v�Z��b_ent_dct�ɏo�͂��܂� */

#define _CRT_SECURE_NO_WARNINGS	//scanf��fopen�ŃG���[���o��̂ŏ����܂���

#include <stdio.h>
#include <math.h>
#include "ica.h"

//�G���g���s�[�v�Z
void b_entropy_dct(double y[][256]){
	FILE *fp;
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, x[64][1024] = { 0 }, b_ent_dct[1024];
	static int hist[50000] = { 0 };

	if ((fp = fopen("b_entropy_dct.csv", "w")) == NULL){
		fprintf(stderr, "Can not open file\n");
	}

	/* �ޔ� */
	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++)
	//		x[j][i] = y[j][i];


	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			m = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					x[m][n] = y[i + k][j + l];

					m++;
				}
			}
			n++;
		}
	}

	/* �u���b�N���Ƃ̌W���̃G���g���s�[�̌v�Z */
	for (n = 0; n < 1024; n++){

		/* hist�̏����� */
		for (i = 0; i < 50000; i++)
			hist[i] = 0;

		b_ent_dct[n] = 0;

		/* hist�̍쐬 */
		min = x[1][n];
		for (i = 2; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n];

		for (i = 1; i < 64; i++){
			//hist[(int)(x[i][n]) + 1]++;	//�X�e�b�v��1
			hist[(int)(x[i][n] - min) + 1]++;	//�X�e�b�v��1
		}


		/* �G���g���s�[�̌v�Z */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0)
				b_ent_dct[n] += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));

		b_ent_dct[n] = b_ent_dct[n] * 63;
	}

	for (i = 0; i < 1024; i++)
		fprintf(fp, "%lf,", b_ent_dct[i]);

	fclose(fp);


}