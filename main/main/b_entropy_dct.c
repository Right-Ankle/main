/* b_entropy_dct.c */

/* �u���b�N���Ƃ�y�̃G���g���s�[���v�Z��b_ent_dct�ɏo�͂��܂� */

#define _CRT_SECURE_NO_WARNINGS	//scanf��fopen�ŃG���[���o��̂ŏ����܂���

#include <stdio.h>
#include <math.h>
#include "ica.h"

//�G���g���s�[�v�Z
void b_entropy_dct(double y[][256], double b_ent_dct[1024]){
	FILE *fp;
	int i = 0, j = 0, l = 0, m = 0, n = 0, k = 0;
	static double min = 0, min2 = 0, x[64][1024] = { 0 }, sum=0, sum1=0;
	static int hist[50000] = { 0 };
	static int hist2[50000] = { 0 };

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
					x[m][n] = y[i + k][j + l]; //256*256 -> 64*1024

					m++;
				}
			}
			n++;
		}
	}
	sum = 0;
	sum1 = 0;
	/* �u���b�N���Ƃ̌W���̃G���g���s�[�̌v�Z */
	for (n = 0; n < 1024; n++){

		/* hist�̏����� */
		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
			hist2[i] = 0;
		}

		b_ent_dct[n] = 0;

		/* hist�̍쐬 */
		min = x[1][n];
		for (i = 2; i < 64; i++)
			if (x[i][n] < min)
				min = x[i][n]; // hist�̍��[

		for (i = 1; i < 64; i++){
			//hist[(int)(x[i][n]) + 1]++;	//�X�e�b�v��1
			hist[(int)(x[i][n] - min) + 1]++;	//�X�e�b�v��1
		}


		/* �G���g���s�[�̌v�Z */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				b_ent_dct[n] += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));
				sum1 += -((hist[i] / (double)(63)) * (log((hist[i] / (double)(63))) / log(2)));
			}
		sum += sum1;
		b_ent_dct[n] = b_ent_dct[n] * 63;
	}

	/* hist2�̍쐬 */
	min2 = x[0][0];
	for (i = 0; i < 1024; i++)
		if (x[0][i] < min2)
			min2 = x[0][i]; // hist�̍��[

	for (i = 0; i < 1024; i++) {
		//hist[(int)(x[i][n]) + 1]++;	//�X�e�b�v��1
		hist2[(int)(x[0][i] - min2) + 1]++;	//�X�e�b�v��1
	}

	sum1 = 0;
	/* �G���g���s�[�̌v�Z */
	for (i = 0; i < 50000; i++)
		if (hist2[i] > 0) {
			sum1 += -((hist2[i] / (double)(1024)) * (log((hist2[i] / (double)(1024))) / log(2)));
		}
	//printf("\n%lf\n", sum1 / 64);

	//printf("\n%lf\n", sum / (1024*1024));
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_ent_dct[i];

	//printf("\n%lf\n", sum / (256*256));
	printf("\n%lf\n", (sum / (256 * 256)) + (sum1/64));

	/// <summary>///////////////////////////////////////////////////////////////////////////////////
	sum = 0;
	for (n = 0; n < 64; n++) {
		sum1 = 0;
		/* hist�̏����� */
		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
			hist2[i] = 0;
		}

		/* hist�̍쐬 */
		min = 1000000;
		for (i = 0; i < 1024; i++)
			if (x[n][i] < min)
				min = x[n][i]; // hist�̍��[

		for (i = 0; i < 1024; i++) {
			//hist[(int)(x[i][n]) + 1]++;	//�X�e�b�v��1
			hist[(int)(x[n][i] - min) + 1]++;	//�X�e�b�v��1
		}


		/* �G���g���s�[�̌v�Z */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				//b_ent_dct[n] += -((hist[i] / (double)(1024)) * (log((hist[i] / (double)(1024))) / log(2)));
				sum1 += -((hist[i] / (double)(1024)) * (log((hist[i] / (double)(1024))) / log(2)));
			}
		sum += (sum1*1024);
	}
	sum = sum / (64 * 1024);
	printf("\ndct_basis_all : %lf\n", sum);
	/// <param name="b_ent_dct"></param>//////////////////////////////////////////////////////////////////////////////////

	for (i = 0; i < 1024; i++)
		fprintf(fp, "%lf,", b_ent_dct[i]);

	fclose(fp);


}