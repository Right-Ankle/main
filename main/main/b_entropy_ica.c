/* b_entropy_ica.c */

/* �u���b�N���Ƃ�y�̃G���g���s�[���v�Z��b_ent_ica�ɏo�͂��܂� */

#define _CRT_SECURE_NO_WARNINGS	//scanf��fopen�ŃG���[���o��̂ŏ����܂���

#include <stdio.h>
#include <math.h>
#include "ica.h"

double step = 100.0;
//�u���b�N���Ƃ�y�̃G���g���s�[���v�Z��b_ent_ica�ɏo�͂��܂�
void b_entropy_ica(double y[64][1024], double b_ent_ica[1024]){
	FILE *fp;
	int i = 0, j = 0, l = 0, m = 0, n = 0;
	static double min = 0, x[64][1024] = { 0 };
	static int hist[50000] = { 0 };

	if ((fp = fopen("b_entropy_ica.csv", "w")) == NULL){
		fprintf(stderr, "Can not open file\n");
	}

	/* �ޔ� */
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			x[j][i] = y[j][i]; //�W��

	/* �u���b�N���Ƃ̌W���̃G���g���s�[�̌v�Z */
	for (n = 0; n < 1024; n++){

		/* hist�̏����� */
		for (i = 0; i < 50000; i++)
			hist[i] = 0;

		b_ent_ica[n] = 0;

		/* hist�̍쐬 */
		min = x[0][n];
		for (i = 1; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n]; //�ŏ��l

		for (i = 0; i < 64; i++){
			//hist[(int)((x[i][n]) * step) + 1]++;	//�X�e�b�v��1
			hist[(int)((x[i][n] - min) * step) + 1]++;	//�X�e�b�v��1
		}


		/* �G���g���s�[�̌v�Z */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0)
				b_ent_ica[n] += -(hist[i] / (double)(8 * 8)) * (log(hist[i] / (double)(8 * 8)) / log(2));

		b_ent_ica[n] = b_ent_ica[n] * 64;
	}

	for (i = 0; i < 1024; i++)
		fprintf(fp, "%lf,", b_ent_ica[i]);

	fclose(fp);
}