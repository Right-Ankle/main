/* new_whitening.c */

/************************/
/*   ���F�����s���܂��B	*/
/*�@���F���F�e�������݂��ɖ����ւɂ��A*/
/*�@�@�@�@�@���ς��O�A���U���P�ɂ���B*/
/************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ica.h"

void new_seki(double w[][64], double **xx, double **yy, int data);
void seki3(double q[][64], double w[][64], double dw[][64]);
void inv(double w[][64], double inv_w[][64]);
// ica.c -> xx = ���F��, v = �ŗL�x�N�g���̋t�s��ƌŗL�l�Ίp�s�� �̐ς̋t�s��,
// p = �ŗL�x�N�g���̋t�s��ƌŗL�l�Ίp�s�� �̐�, date = 1024,
// ���F��(Whitening)�͐��f�[�^�̗v�f�Ԃ̑��ւ��Ȃ����A����ɕ��U�̐��K����O�����Ƃ��Ď��s���邱�ƂŊw�K�̌�������_��
void new_whitening(struct pca new_pca, double **xx, double v[][64], double p[][64], int data){

	static double **temp;
	static double d[64][64] = {0}, e[64][64] = {0}, temp2[64][64] = {0};  //d -> �ŗL�l�̑Ίp�s��,
	int i, j;

	temp = (double **)malloc(sizeof(double) * 64);
	for (i = 0; i < 64; i++) {
		temp[i]  = (double *)malloc(sizeof(double) * data);
	}
    // xx -> temp�փR�s�[
	for(i = 0; i < 64; i++)
		for(j = 0; j < data; j++)
			temp[i][j] = xx[i][j];
    // d -> �サ���ŗL�l��Ίp�s��ɕϊ�
	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			if(i == j)
				d[i][j] = sqrt(new_pca.r[i]);
    // e -> �ŗL�x�N�g���̓]�u
	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			e[j][i] = new_pca.a[i][j];
    // p -> e��d�̐� (q -> �ŗL�x�N�g���̓]�u, w -> �サ���ŗL�l��Ίp�s��ɕϊ�)
	seki3(e, d, p);

	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			temp2[i][j] = p[i][j]; //temp2 -> ���F��

	inv(temp2, v); // temp2 -> �P�ʍs��, v -> temp2�̋t�s��
	new_seki(v, temp, xx, data); // �ŗL�x�N�g���̓]�u�Ɓサ���ŗL�l�Ίp�s�� �̐ρ@�̋t�s�� �� ���S���@�̐� -> ���F��

	// �������
	for(i = 0; i < 64; i++)
		free(temp[i]);
	free(temp);

}
