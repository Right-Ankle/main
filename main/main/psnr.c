/* psnr.c */

/****************************************************************/
/*  引数として渡された2つの画像からPSNRを計算し，その値を返します。	*/
/****************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

double psnr(unsigned char origin[][256], unsigned char saikousei[][256]){

	int i, j;
	double PSNR, sum = 0.0;

	for(i = 0; i < 256; i++)
		for(j = 0; j < 256; j++)
			sum += pow(origin[i][j] - saikousei[i][j], 2);
      
    PSNR = 10 * log10((double)(255 * 255) / (sum / (double)(256 * 256)));
	
	return PSNR;
}