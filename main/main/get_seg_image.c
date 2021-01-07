/* get_seg_image.c */

#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"

void get_seg_image(unsigned char origin[][256], int b_flag[1024], int Q){
	static int i, j, k, l, m, n = 0;
	static char output[50];
	static unsigned char seg[32][32] = { 0 }, seg2[256][256] = { 0 };
	static int temp_basis[32 * 32], temp_basis2[256 * 256];
	static double tmp[32][32] = { 0 }, tmp2[256][256] = { 0 }, tmp3[64][1024] = { 0 }, oritmp[64][1024];
	static int block = 0, sub = 0;

	for (i = 0; i < 256; i++){
		for (j = 0; j < 256; j++){
			seg2[i][j] = origin[i][j];
		}
	}


	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			if (b_flag[(i / 8) * 32 + (j / 8)] == 0){
				for (k = 0; k < 8; k++)
					for (l = 0; l < 8; l++)
						seg2[i + k][j + l] = 0;
			}
		}
	}

	/* 分類bmpで出力 */
	sprintf(output, "OUTPUT/seg_%d.bmp", Q);
	//sprintf(output, "segmentation_pr/seg_%.2lf.bmp", lambda);
	//sprintf(output, "seg_output.bmp");


	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			temp_basis2[i * 256 + j] = seg2[i][j];
	img_write_gray(temp_basis2, output, 256, 256);

}