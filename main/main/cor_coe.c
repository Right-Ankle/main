#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"

double cor_coe(double date1[1024], double date2[1024]) {
	int i, j;
	double result_coe, sum0, sum1, sum2, sum11, sum22;

	sum2 = 0; sum0 = 0; sum1 = 0; sum11 = 0; sum22 = 0;

	for (j = 0; j < 1024; j++) {
		sum1 += date1[j];
		sum2 += date2[j];
	}

	sum1 /= 1024.0;
	sum2 /= 1024.0;

	for (j = 0; j < 1024; j++){
		sum0 += (date1[j] - sum1) * (date2[j] - sum2);
		sum11 += pow(date1[j] - sum1, 2);
		sum22 += pow(date2[j] - sum2, 2);
	}

	result_coe = (sum0 / 1024.0) / (sqrt(sum11 / 1024.0) * sqrt(sum22 / 1024.0));


	return result_coe;
}

double cor_coe2(unsigned char origin[][256], int a, int b) {
	int i, j, n, m, l, k;
	double result_coe, sum0, sum1, sum2, sum11, sum22;
	double x[64][1024];

	sum2 = 0; sum0 = 0; sum1 = 0; sum11 = 0; sum22 = 0;

	// 256*256  ‚ð@64*1024
	n = 0;
	for (i = 0; i < 256; i += 8) {
		for (j = 0; j < 256; j += 8) {
			m = 0;
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {
					x[m][n] = origin[i + k][j + l];
					m++;
				}
			}
			n++;
		}
	}

	for (j = 0; j < 64; j++) {
		sum1 += x[j][a];
		sum2 += x[j][b];
	}

	sum1 /= 64.0;
	sum2 /= 64.0;

	for (j = 0; j < 64; j++) {
		sum0 += (x[j][a] - sum1) * (x[j][b] - sum2);
		sum11 += pow(x[j][a] - sum1, 2);
		sum22 += pow(x[j][b] - sum2, 2);
	}

	result_coe = (sum0 / 64.0) / (sqrt(sum11 / 64.0) * sqrt(sum22 / 64.0));


	return result_coe;
}

double cor_coe3(int temp[2][10]) {
	int i, j, n, m, l, k;
	double result_coe, sum0, sum1, sum2, sum11, sum22;
	double x[64][1024];

	sum2 = 0; sum0 = 0; sum1 = 0; sum11 = 0; sum22 = 0;

	// W‡“à‚Ì“¯‚¶”Žš‚ð’Tõ
	n = 0;
	for (k = 0; k < 4; k++) {
		for (l = 0; l < 4; l++) {
			if (temp[0][k] == temp[1][l])
				n++;
		}
	}

	result_coe = (double)n/4.0;//•ªŽq*2‚¾‚¯‚Ç–ñ•ª‚µ‚Ä‚ ‚è‚Ü‚·

	return result_coe;
}