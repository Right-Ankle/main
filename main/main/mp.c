/* mp.c */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "ica.h"
// mpans -> �u���b�N���ő�l�̓Y��
void mp(double y[][1024], double avg[1024], double w[][64], int mpans[][64]){
	FILE *fp;
	int i, j, k, l, key[1024] = {0}, count, hist[64][64] = {0};
	static unsigned char gen[256][256] = {0}, sai[256][256] = {0};
	static double x[64][1024] = {0}, temp[64][1024] = {0}, x2[64][1024] = {0}, y2[64][1024] = {0}, xori[64][1024] = {0};
	static double atemp[64] = {0}, btemp[64] = {0}, in[1024][64] = {0}, max, inans[1024][64] = {0}, inans2[1024][64] = {0};
	int gauge = 4;

	if ((fp = fopen("OUTPUT\\Matching_Pursuit.csv", "w")) == NULL) {
		fprintf(stderr,"Can not open file\n");
	}

	for(k = 0; k < 1024; k++)
		for(l = 0; l < 64; l++)
			y2[l][k] = y[l][k]; //ica�W�����R�s�[

	seki(w, y2, xori); //ica�W����ica���̐�
	//xtogen(xori, gen, avg);
	//gentox(gen, xori);

	seki(w, y2, x); //ica�W����ica���̐�
	//xtogen(x, gen, avg);
	//gentox(gen, x);

	printf("\n Mutching pursuits START\n");


	for (count = 0; count < 64; count++) { //MP�@�̊i�[�J�E���g

		for (j = 0; j < 64; j++) {//�����Ώۊ��
			//temp -> �Ώۊ��ȊO�̌W����0 �Ώۊ��݂̂ł�ica���Ƃ̐�
			for (k = 0; k < 1024; k++)
				for (l = 0; l < 64; l++)
					if (j == l)
						temp[l][k] = y2[l][k];
					else
						temp[l][k] = 0.0;

			seki(w, temp, x2);//��
			//xtogen(x2, gen, avg);
			//gentox(gen, x2);

			// atemp -> ica�W��*���, btemp ->�����Ȃ�ica�W��*���
			// �u���b�N���Ƃɏ���
			for (i = 0; i < 1024; i++) {
				for (k = 0; k < 64; k++) {
					atemp[k] = x[k][i];
					btemp[k] = x2[k][i];
				}
				in[i][j] = inner_product(atemp, btemp); //�����Ȃ��č\���ƑΏۊ��̂ݍč\���̓���
			}
		}

		for (i = 0; i < 1024; i++) {
			max = -1;
			for (j = 0; j < 64; j++) {
				if (in[i][j] > max) {
					// �u���b�N���ő�l�i�[
					max = in[i][j];
					key[i] = j;
				}
			}

			//�ő�l�ƓY����ۑ�
			mpans[i][count] = key[i]; //MP�@�̗D��x�̌���
			inans[i][count] = max;

			y2[key[i]][i] = 0.0; //ica�W���̃u���b�N���ԍ�j�͂O
		}

		printf("\r Now Running  :  [%3.3lf]", ((double)count / 64.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	fclose(fp);
}









