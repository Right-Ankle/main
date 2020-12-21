#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// ‰æ‘œo—Í—p(—ÌˆæŽw’è[1024])
int txt_out(double date1[4][1024], static char filename[20], int rate) {

	FILE* fp;
	int count = 0;
	char out[50];

	sprintf(out, "OUTPUT\\%d.txt", rate);

	if ((fp = fopen(out, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	printf(" now output  - - - ->");

	fprintf(fp, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp, "\n\n  DCT vs ICA  \n\n    Area with a small number of basis\n  Number of basis used : 1 ~ 64 \n    DCT Rate : %d\n\n----------------------------------------------------------------------------------\n\n", rate);

	for (int j = 0; j < 1024; j++) {
		if (date1[0][j] > date1[2][j]) {
			count++;
			if (rate == 100 || rate == 90)
				fprintf(fp, "   %d,", j);
		}
	}
	fprintf(fp, "\n\n    DCT : %d / 1024 \n    ICA : %d / 1024\n\n", 1024 - count, count);

	for (int j = 0; j < 1024; j++) {
		fprintf(fp, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);

		fprintf(fp, "\n\n    DCT NUM : %2d (%3d)\n\n    DCT mse : %lf  (%2.2lf dB)\n", (int)date1[0][j], rate, date1[1][j], 10*log10((255*255)/date1[1][j]));
		fprintf(fp, "\n\n    ICA NUM : %2d\n\n    ICA mse : %lf  (%2.2lf dB)\n", (int)date1[2][j], date1[3][j], 10 * log10((255 * 255) / date1[3][j]));
	}

	printf(" end\n\n");
	fclose(fp);
	return filename;
}