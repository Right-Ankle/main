
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// �摜�o�͗p(�̈�w��[1024])
int txt_out2(double date1[65][1024], static char filename[20], int rate) {

	FILE* fp5;
	int count = 0, count4 = 0;
	int i, j, k, l, Q, QQ, QQQ = 0, basis[65], temp[2][1024];
	double num[64], num2[1024], basis1[64], basis2[64], flag[64], flag2[64], gloup[64][64], temp2[64], gloup2[64][64][2], count2[64], count3[64][64], gloup3[64][64][2], group[64][64][2];
	char out[50];


	for (i = 0; i < 65; i++)
		basis[i] = 0;
	for (i = 0; i < 64; i++) {
		num[i] = 0;
		basis1[i] = 0;
		basis2[i] = 0;
		flag[i] = 0;
		count2[i] = 0;
		for (j = 0; j < 64; j++) {
			gloup[i][j] = 0;
			gloup2[i][j][0] = 99;
			gloup2[i][j][1] = 99;
			count3[i][j] = 0;
			gloup3[i][j][0] = 99;
			gloup3[i][j][1] = 99;
			group[i][j][0] = 99;
			group[i][j][1] = 99;
		}
	}
	for (i = 0; i < 1024; i++) {
		temp[0][i] = 99;
		temp[1][i] = 99;
		num2[i] = 99;
	}
	sprintf(out, "OUTPUT\\ICAbasis[%d].txt", rate);

	if ((fp5 = fopen(out, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	printf(" now output  - - - ->");
	fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp5, "\n\n  Basis used in the ICA area \n\n\n  Number of basis used : 0 ~ 64 \n\n\n  Use basis -> $$ \n\n\n\n  Use Q rate : %d\n\n----------------------------------------------------------------------------------\n\n", rate);

	int line = 67;
	fprintf(fp5, " +----+");
	for (i = 0; i < line; i++)
		fprintf(fp5, "----+");

	fprintf(fp5, "\n | ## |");
	fprintf(fp5, " No |");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "[%2d]|", i);
	fprintf(fp5, " ## | num|");

	for (i = 0; i < 65; i++)
		basis[i] = 0;
	for (j = 0; j < 1024; j++) {
		QQ = 0;
		fprintf(fp5, "\n +----+");
		for (i = 0; i < line; i++)
			fprintf(fp5, "----+");
		fprintf(fp5, "\n |%4d|", j);
		if (date1[64][j] == 1) {
			fprintf(fp5, " ## |");
			basis[64]++;
		}
		else
			fprintf(fp5, "    |");
		for (i = 0; i < 64; i++) {
			if ((int)date1[64][j] != 3) {
				if ((int)date1[i][j] == 1) {
					fprintf(fp5, " $$ |");
					QQ++;
					basis[i]++;
				}
				else
					fprintf(fp5, "    |");
			}
			else {
				fprintf(fp5, "    |");
			}
		}

		fprintf(fp5, "%4d|", j);
		if (date1[64][j] == 2) {
			fprintf(fp5, "[--]|");
			QQQ++;
		}
		else {
			fprintf(fp5, "[%2d]|", (int)QQ);
			num[QQ]++;
			num2[j] = QQ;
		}
	}
	fprintf(fp5, "\n +----+");
	for (i = 0; i < line; i++)
		fprintf(fp5, "----+");

	fprintf(fp5, "\n | ## |");
	fprintf(fp5, " No |");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "[%2d]|", i);
	fprintf(fp5, " ## | ## |");

	fprintf(fp5, "\n +----+");
	for (i = 0; i < line; i++)
		fprintf(fp5, "----+");

	fprintf(fp5, "\n | num|");
	fprintf(fp5, "%4d|", basis[64]);
	for (i = 0; i < 64; i++)
		fprintf(fp5, "%4d|", basis[i]);
	fprintf(fp5, " ## |%4d|                                                                                    ", 1024 - QQQ);

	fprintf(fp5, "\n +----+");
	for (i = 0; i < line; i++)
		fprintf(fp5, "----+");

	fprintf(fp5, "\n\n\n\n\n\n");

	fprintf(fp5, "\n +----+");
	for (i = 0; i < 63; i++)
		fprintf(fp5, "----+");
	fprintf(fp5, "\n |");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "[%2d]|", i);

	fprintf(fp5, "\n +----+");
	for (i = 0; i < 63; i++)
		fprintf(fp5, "----+");
	fprintf(fp5, "\n |");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "%4d|", (int)num[i]);
	fprintf(fp5, "\n +----+");
	for (i = 0; i < 63; i++)
		fprintf(fp5, "----+");

	fprintf(fp5, "\n\n\n\n\n\n");
	gnuplot5(num, rate);

	for (j = 0; j < 1024; j++)
		for (i = 0; i < 64; i++) {
			if (num2[j] == 1) {
				if ((int)date1[i][j] == 1)
					basis1[i]++;
			}
			else if (num2[j] == 2) {
				if ((int)date1[i][j] == 1)
					basis2[i]++;
			}
		}
	txt_out3(basis1, filename, rate + 1);
	txt_out3(basis2, filename, rate + 2);


	for (j = 0; j < 1024; j++) {
		count = 0;
		for (i = 0; i < 64; i++) {
			if (num2[j] == 2) {
				if (date1[i][j] == 1) {
					if (count == 0)
						temp[0][j] = i;
					else
						temp[1][j] = i;
					count++;
				}
			}
			else
			{
				temp[0][j] = 99;
				temp[1][j] = 99;
			}

		}
		//printf("[%d,%d]\n", temp[0][j], temp[1][j]);
		if (num2[j] == 2) {
			group[temp[0][j]][(int)count2[temp[0][j]]][0] = temp[0][j];
			group[temp[0][j]][(int)count2[temp[0][j]]][1] = temp[1][j];
			group[temp[1][j]][(int)count2[temp[1][j]]][0] = temp[0][j];
			group[temp[1][j]][(int)count2[temp[1][j]]][1] = temp[1][j];
			count2[temp[0][j]]++;
			count2[temp[1][j]]++;

			//if (basis2[temp[0][j]] >= basis2[temp[1][j]]) {
			//	flag[temp[0][j]]++;
			//	gloup2[temp[0][j]][(int)count2[temp[0][j]]][0] = temp[0][j];
			//	gloup2[temp[0][j]][(int)count2[temp[0][j]]][1] = temp[1][j];
			//	gloup3[temp[0][j]][(int)count2[temp[0][j]]][0] = temp[0][j];
			//	gloup3[temp[0][j]][(int)count2[temp[0][j]]][1] = temp[1][j];
			//	count2[temp[0][j]]++;
			//	count3[temp[0][j]][temp[0][j]]++;
			//	count3[temp[0][j]][temp[1][j]]++;
			//}
			//else if (basis2[temp[0][j]] < basis2[temp[1][j]]) {
			//	flag[temp[1][j]]++;
			//	gloup2[temp[1][j]][(int)count2[temp[1][j]]][0] = temp[0][j];
			//	gloup2[temp[1][j]][(int)count2[temp[1][j]]][1] = temp[1][j];
			//	gloup3[temp[1][j]][(int)count2[temp[1][j]]][0] = temp[0][j];
			//	gloup3[temp[1][j]][(int)count2[temp[1][j]]][1] = temp[1][j];
			//	count2[temp[1][j]]++;
			//	count3[temp[1][j]][temp[0][j]]++;
			//	count3[temp[1][j]][temp[1][j]]++;
			//}
		}
	}
	txt_out5(group, filename, rate);
	txt_out3(flag, filename, rate + 3);
	//txt_out4(count3, filename, rate);
	//txt_out5(gloup2, filename, rate);

	for (i = 0; i < 64; i++)
		if (gloup2[i][0][0] != 99 && gloup2[i][1][0] == 99) { //1�������̃O���[�v

			QQ = count2[i];
			count = (int)count3[i][(int)gloup2[i][0][0]]; //�����炭1
			for (j = 0; j < 64; j++) {
				if (count < count3[j][(int)gloup2[i][0][0]]) {
					count = count3[j][(int)gloup2[i][0][0]];
					QQ = j;
				}
				else if (count == count3[j][(int)gloup2[i][0][0]] && count2[QQ] < count2[j]) {
					count = count3[j][(int)gloup2[i][0][0]];
					QQ = j;
				}

			}

			Q = count2[i]; //���ԍ�
			count4 = (int)count3[i][(int)gloup2[i][0][1]]; //�p�x
			for (j = 0; j < 64; j++) {
				if (count4 <= count3[j][(int)gloup2[i][0][1]]) {
					count4 = count3[j][(int)gloup2[i][0][1]];
					Q = j;
				}
				else if (count == count3[j][(int)gloup2[i][0][1]] && count2[Q] < count2[j]) {
					count4 = count3[j][(int)gloup2[i][0][1]];
					Q = j;
				}
			}

			if (count > count4)
				Q = QQ;
			else if(count == count4 && count2[Q] < count2[QQ])
				Q = QQ;

			if (QQ != i && Q !=i) {
				gloup3[i][0][0] = 99;
				gloup3[i][0][1] = 99;
				gloup3[Q][(int)count2[Q]][0] = gloup2[i][0][0];
				gloup3[Q][(int)count2[Q]][1] = gloup2[i][0][1];
				count2[Q]++;
			}
		}

	//txt_out5(gloup3, filename, rate);
	for (j = 0; j < 64; j++)
		flag[j] = 0;

	for (j = 0; j < 1024; j++) {
		if (num2[j] == 2)
			if (basis2[temp[0][j]] <= basis2[temp[1][j]])
				flag[temp[0][j]]++;
			else if (basis2[temp[0][j]] > basis2[temp[1][j]])
				flag[temp[1][j]]++;
	}
	txt_out3(flag, filename, rate + 4);

	for (j = 0; j < 1024; j++) {
		if (num2[j] == 2)
			if (flag[temp[0][j]] >= flag[temp[1][j]])
				flag2[temp[0][j]]++;
			else if (flag[temp[0][j]] < flag[temp[1][j]])
				flag2[temp[1][j]]++;
	}
	txt_out3(flag2, filename, rate + 5);

	for (j = 0; j < 1024; j++) {
		if (num2[j] == 2)
		for (i = 0; i < 64; i++) {
			if (temp[0][j] == i)
				gloup[i][temp[1][j]]++;
			if (temp[1][j] == i)
				gloup[i][temp[0][j]]++;
		}
	}

	txt_out4(gloup, filename, rate);

	//for (i = 0; i < 64; i++) {
	//	l = 0;
	//	for (k = 0; k < 64; k++)
	//		temp2[k] = gloup[i][k];
	//	l = rate * 100 + i;
	//	//gnuplot5(temp2, l);
	//}

	printf(" end\n\n");
	fclose(fp5);
	return filename;
}