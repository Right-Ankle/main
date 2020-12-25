
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// ‰æ‘œo—Í—p(—Ìˆæw’è[1024])
int txt_out2(double date1[65][1024], static char filename[20], int rate) {

	FILE* fp5;
	int count = 0;
	int i, j, QQ, QQQ = 0, basis[65], basis0[64], basis1[64], basis2[64];
	double num[64];
	char out[50];

	for (i = 0; i < 65; i++)
		basis[i] = 0;
	for (i = 0; i < 64; i++)
		num[i] = 0;
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
	gnuplot5(num);




	printf(" end\n\n");
	fclose(fp5);
	return filename;
}