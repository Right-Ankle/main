
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// �摜�o�͗p(�̈�w��[1024])
int txt_out5(double date1[64][64][2], static char filename[20], int rate) {

	FILE* fp5;
	int count = 0;
	int i, j, QQ, QQQ = 0, basis[65], basis1[64];
	double num[64], num2[1024], basis2[64];
	char out[50];

	for (i = 0; i < 65; i++)
		basis[i] = 0;
	for (i = 0; i < 64; i++) {
		num[i] = 0;
		basis1[i] = 0;
		basis2[i] = 0;
	}
	for (i = 0; i < 1024; i++)
		num2[i] = 0;
	sprintf(out, "OUTPUT\\group[%d].txt", rate);

	if ((fp5 = fopen(out, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	printf(" now output  - - - ->");
	fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp5, "\n\n  Basis used in the ICA area \n\n\n  Number of basis used : 0 ~ 64 \n\n\n  Use basis -> $$ \n\n\n\n  Use Q rate : %d\n\n----------------------------------------------------------------------------------\n\n", rate);

	int line = 67;

	fprintf(fp5, "\n +----+");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "---------+");
	fprintf(fp5, "\n | ## |");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "     [%2d]|", i);

	for (j = 0; j < 64; j++) {
		fprintf(fp5, "\n +----+");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "---------+");

		fprintf(fp5, "\n |[%2d]|", j);
		for (i = 0; i < 64; i++)
			if (date1[j][i][0] != 99||date1[j][i][1] != 99) {
				fprintf(fp5, "%4d,%4d|", (int)date1[j][i][0], (int)date1[j][i][1]);
			}
			else
				fprintf(fp5, "         |");
	}
	fprintf(fp5, "\n +----+");
	for (i = 0; i < 64; i++)
		fprintf(fp5, "---------+");

	fprintf(fp5, "\n\n\n\n\n\n");


	printf(" end\n\n");
	fclose(fp5);
	return filename;
}