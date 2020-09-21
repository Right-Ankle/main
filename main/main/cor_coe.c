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