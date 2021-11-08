
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// 配列を定義して，少しいじるだけで他のフィルタも適用可能
void smoothing(unsigned char origin[][256], unsigned char out[][256], int size) {
	int i, j, k, l;
	double sum;

	for (j = 0; j < 256; j++)
		for (i = 0; i < 256; i++) {
			if (i + size > 256 && j + size > 256) {
				sum = 0;
				for (k = j; k < 256; k++)
					for (l = i; l < 256; l++)
						sum += (double)origin[l][k];
				out[i][j] = (int)(sum / ((k - j) * (l - i)));
			}
			else if (i + size > 256) {
				sum = 0;
				for (k = j; k < j + size; k++)
					for (l = i; l < 256; l++)
						sum += (double)origin[l][k];
				out[i][j] = (int)(sum / (size * (l - i)));
			}
			else if (j + size > 256) {
				sum = 0;
				for (k = j; k < 256; k++)
					for (l = i; l < i + size; l++)
						sum += (double)origin[l][k];
				out[i][j] = (int)(sum / ((k - j) * size));
			}
			else {
				sum = 0;
				for (k = j; k < j + size; k++)
					for (l = i; l < i + size; l++)
						sum += (double)origin[l][k];
				out[i][j] = (int)(sum / (size * size));
			}
		}

}