/* ICA.c */

/****************************************************************************************************************************************/
/*  �����i256*256�̉摜�f�[�^�CPCA�̌��ʁCICA�W���p��64*1024�̔z��CICA���p��64*64�̔z��CICA�̒��������p��1024�̔z��C�v�Z�񐔁C�w�K�W���j	*/
/*  ny, nw, navg �͈����Ƃ��ēn���ƒ��g�������������܂��B�K�؂Ȍv�Z�񐔂Ɗw�K�W���͉摜�ɂ��قȂ�܂��B�ڍׂ�new_ica.c���Q�Ƃ��Ă��������B	*/
/****************************************************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "ica.h"
// �����i256*256�̉摜�f�[�^�CPCA�̌��ʁCICA�W���p��64*1024�̔z��CICA���p��64*64�̔z��CICA�̒��������p��1024�̔z��C�v�Z�񐔁C�w�K�W���j
// ny->ica�W��, nw->ica���, navg->���u���b�N�̕���, e->������, f->�w�K�W��
void ICA_temp(unsigned char origin[][256], struct pca pcaTemp, double ny[][1024], double nw[][64], double navg[1024], int e, double f, int num1, int num2) {
	//FILE * fp2; // fp1:pca���, fp2:ica���, fp3:ica�W��, fp4:
	int i, j, k, m, n, l, a, b, t, count = 0, count2 = 0; // �v�Z�p
	static unsigned char gen[256][256] = { 0 }, npca_basis[64][64] = { 0 }, nica_basis[64][64] = { 0 }, ica_sai[256][256] = { 0 };
	static double  x[64][1024] = { 0 }, w[64][64] = { 0 }, v[64][64] = { 0 }, p[64][64] = { 0 }, y[64][1024] = { 0 }, z[64][64] = { 0 }, q[64][64] = { 0 }, dw[64][64] = { 0 }, w2[64][64] = { 0 };
	static double** xx, ** yy, ** sig, * avg, ** yt, * b_kurt;
	static double temp;
	static double sum = 0;

	char output[50];
	static int temp_basis[64 * 64];
	static int temp_basis2[64 * 64];
	static int ica[64];
	static int temp_sai[256 * 256];

	/*�@�t�@�C���I�[�v���@*/
	//if ((fp1 = fopen("PCA_BASIS.pgm", "wb")) == NULL) {  //pgm > �O���[�X�P�[���摜���������߂̊g���q
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp2 = fopen("ICA_test.pgm", "wb")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp3 = fopen("y.csv", "w")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp4 = fopen("ICA_SAI.pgm", "wb")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp5 = fopen("ica_ori.txt", "a")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	printf("\nICA --> [%d : %d] --> ", num1, num2);

	/*�@�̈���m�ہ@*/
	/*  malloc > �����Ŏw�肷��o�C�g�����̃������𓮓I�Ɋm�ہ@(�g�p����stdlib���C���|�[�g) */
	/*           �������̈�ւ̃|�C���^���Ԃ���� (�m�ۂł��Ȃ��ꍇ��NULL)�@�@�@�@�@�@�@�@�@*/
	/*           �ÓI�E���I�������m�ۂ̏ꍇ,��L����Ƀv���O���������݁E�T�u���[�`�������s�� */
	/*           �̊Ԃ�,���������m�ۂ��������Ă���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ */
	/*           -> ���I�ɂ���΁C����������������̖��ʂ��Ȃ����Ƃ��\                 */

	// sizeof > �o�C�g�����w��
	xx = (double**)malloc(sizeof(double*) * 64);
	yy = (double**)malloc(sizeof(double*) * 64);
	sig = (double**)malloc(sizeof(double*) * 64);
	avg = (double*)malloc(sizeof(double) * 1024);
	// �\����1��1�ɂ����������m��
	for (i = 0; i < 64; i++) {
		// 64��1024�o�C�g
		xx[i] = (double*)malloc(sizeof(double) * 1024);
		yy[i] = (double*)malloc(sizeof(double) * 1024);
		sig[i] = (double*)malloc(sizeof(double) * 1024);
	}

	yt = (double**)malloc(sizeof(double*) * 1024);
	for (i = 0; i < 1024; i++) {
		// 1024��64�o�C�g
		yt[i] = (double*)malloc(sizeof(double) * 64);
	}

	// �R�s�[
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			gen[i][j] = origin[i][j];

	// �ŗL�l�����������Ƀ\�[�g -> �ŗL�x�N�g��������
	for (i = 0; i < 64 - 1; i++) {
		for (j = 64 - 1; j > i; j--) {
			if (pcaTemp.r[j - 1] < pcaTemp.r[j]) {  /* �O�̌ŗL�l�̕����傫�������� */
				temp = pcaTemp.r[j];        /* �������� */
				pcaTemp.r[j] = pcaTemp.r[j - 1];
				pcaTemp.r[j - 1] = temp;

				// �ŗL�x�N�g���̗�����ׂČ���
				for (k = 0; k < 64; k++) {
					temp = pcaTemp.a[j][k];
					pcaTemp.a[j][k] = pcaTemp.a[j - 1][k];
					pcaTemp.a[j - 1][k] = temp;
				}
			}
		}
	}


	gentox(gen, x); //256*256 -> 64*1024�ɕϊ�

	// �c�ɃR�s�[���Ă邾��(�u���b�N���ƂɌv�Z���邽�߁B�R�s�[���͓��Ɋ֌W�Ȃ�)
	for (i = 0; i < 1024; i++) {
		for (j = 0; j < 64; j++) {
			xx[j][i] = x[j][i];
		}
	}
	//
	new_pre(xx, avg, 1024);	//���S�� �ixx -> ���̉�f�l - ���ϒl, avg = �u���b�N���Ƃ̕��ϒl�j

	new_whitening(pcaTemp, xx, v, p, 1024);	//���F�� -> xx(�f�[�^�̋����U�s�񂪒P�ʍs��ƂȂ�悤�ɕϊ�)
	// ���F����̍s����m�F�������B

	makew(w);	//�����s�񐶐�

	/*�@�����J�n�@e -> ������*/
	for (t = 0; t < e; t++) {

		new_seki(w, xx, yy, 1024);	//�s��̐ς��v�Z
//
		new_sigmoid(yy, sig, 1024);	//�V�O���C�h�֐���K�p, sig�͈̔͂�0~1

		new_ytoyt(yy, yt, 1024);	//y �̓]�u�s����v�Z

		new_seki2(sig, yt, z, 1024);	//�s��̐ς��v�Z

		identity(z, q);	//�P�ʍs��Ƃ̌��Z

		seki3(q, w, dw);	//�s��̐ς��v�Z

		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				w[i][j] += f * dw[i][j];	//�w�K�W�� f �ɂ��ߓx�Ȋw�K��}����
			}
		}

		normal(w);	//������
	}

	seki3(w, v, w2); // w2 -> w��v�̐�
	inv(w2, w);	//�����܂ł� w ������ w -> w2�̋t�s��, w2 -> �P�ʍs��

	printf("END\n");

	wtosai(p, npca_basis);	//�o�͗pPCA���̍쐬�@p -> pca���
	wtosai(w, nica_basis);	//�o�͗pICA���̍쐬�@w -> ica���


	//fprintf(fp1, "P5\n64 64\n255\n");
	//fwrite(npca_basis, sizeof(unsigned char), 64 * 64, fp1);	//PCA���o��, 64*64 0~255

	//fprintf(fp2, "P5\n64 64\n255\n");
	//fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA���o��, 64*64 0~255



	//sprintf(output, "PCA_BASIS.bmp"); //PCA���bmp�ŏo��
	//for (i = 0; i < 64; i++)
	//	for (j = 0; j < 64; j++)
	//		temp_basis[i * 64 + j] = npca_basis[i][j];
	//img_write_gray(temp_basis, output, 64, 64);

	//sprintf(output, "ICA_BASIS.bmp"); //ICA���bmp�ŏo��
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			temp_basis[i * 64 + j] = nica_basis[i][j];
	//img_write_gray(temp_basis, output, 64, 64);

	// ICA���o��
	for (i = 0; i < 64; i++) {
		m = i % 8;
		n = i / 8;
		for (a = 0; a < 8; a++)
			for (b = 0; b < 8; b++)
				ica[a * 8 + b] = temp_basis[64 * 8 * m + 8 * n + b + 64 * a];
		for (a = 0; a < 8; a++)
			for (b = 0; b < 8; b++)
				for (k = 0; k < 8; k++)
					for (l = 0; l < 8; l++)
						temp_basis2[64 * 8 * a + 8 * b + l + 64 * k] = ica[a * 8 + b];

		if (8 * m + n == num2) {
			sprintf(output, "OUTPUT\\coe/coefficient[%d]/ICA[%d].bmp", num1, 8 * m + n);
			img_write_gray(temp_basis2, output, 64, 64); // output�ɏo�͉摜�������o��
		}
		//sprintf(output, "ICA/ICA'[%d].bmp", 8 * m + n);
		//img_write_gray(temp_basis2, output, 64, 64); // output�ɏo�͉摜�������o��
		//sprintf(output, "test/ICA[%d]/ICA[%d].bmp", 8 * m + n, 8 * m + n);
		//img_write_gray(temp_basis2, output, 64, 64); // output�ɏo�͉摜�������o��

	}



	//for (i = 0; i < 64; i++) {
	//	for (j = 0; j < 1024; j++) {
	//		fprintf(fp3, "%lf,", yy[i][j]);	//ICA�W����������
	//	}
	//	fprintf(fp3, "\n");
	//}

	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++)
	//		ny[j][i] = yy[j][i]; // ny -> yy(ica�W���R�s�[)

	//for (i = 0; i < 64; i++)
	//	for (j = 0; j < 64; j++)
	//		nw[j][i] = w[j][i]; // nw-> w(ica���R�s�[)

	//for (i = 0; i < 1024; i++)
	//	navg[i] = avg[i]; // navg -> avg(�u���b�N���Ƃ̕��ϒl�R�s�[)

	///*�@�č\���摜�̐����i��{�̗���j�@*/
	//seki5(nw, ny, x); // x -> nw * ny
	//xtogen(x, ica_sai, navg); // ica_sai -> �č\���χ@
	//avg_inter(ica_sai, navg); // ica_sai -> �č\���χA
	///*�@�č\���摜�����I���@*/

	//fprintf(fp5, "---------- ICA ORIGIN ----------\n");
	//fprintf(fp5, "[8~15][0~7]\n");
	//for (j = 0; j < 8; j++) {
	//	for (i = 8; i < 16; i++) {
	//		fprintf(fp5, "%3d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");

	//fprintf(fp5, "---------- ICA ORIGIN ----------\n");
	//fprintf(fp5, "[0~7][8~15]\n");
	//for (j = 8; j < 16; j++) {
	//	for (i = 0; i < 8; i++) {
	//		fprintf(fp5, "%3d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");



	///*�@�č\���摜�̐����i��{�̗���j�@*/
	//seki5(nw, ny, x); // x -> nw * ny
	//xtogen(x, ica_sai, navg); // ica_sai -> �č\���χ@
	//avg_inter(ica_sai, navg); // ica_sai -> �č\���χA
	///*�@�č\���摜�����I���@*/

	//fprintf(fp5, "---------- ICA Process ----------");
	//for (j = 0; j < 256; j++) {
	//	for (i = 0; i < 256; i++) {
	//		fprintf(fp5, "%4d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");

	//fprintf(fp4, "P5\n256 256\n255\n");
	//fwrite(ica_sai, sizeof(unsigned char), 256 * 256, fp4);	//�č\���摜�o��


	//sprintf(output, "ICA_SAI.bmp"); //�č\���摜bmp�ŏo��
	//for (i = 0; i < 256; i++)
	//	for (j = 0; j < 256; j++)
	//		temp_sai[i * 256 + j] = ica_sai[i][j];
	//img_write_gray(temp_sai, output, 256, 256);


	/*�@�̈����@*/
	// free�֐��ŉ�� > �������Ɠ���
	for (i = 0; i < 64; i++) {
		free(xx[i]);
		free(yy[i]);
		free(sig[i]);
	}

	for (i = 0; i < 1024; i++) {
		free(yt[i]);
	}

	free(xx);
	free(yy);
	free(sig);
	free(avg);
	free(yt);

	/*�@�t�@�C���N���[�Y�@*/
	//fclose(fp1);
	//fclose(fp2);
	//fclose(fp3);
	//fclose(fp4);
	//fclose(fp5);

}
