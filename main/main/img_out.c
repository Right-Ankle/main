#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// 画像出力用(出力画像でーた, 領域指定[1024](0は黒), 領域名)
int img_out(unsigned char origin[][256], int date1[1024], int name) {
	static int temp_sai[256 * 256];
	char output[1000];
	int a, b, i, l, k;
	//gp = _popen(GNUPLOT_PATH, "w");
	printf(" \n now output  - - - ->");
	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai[a * 256 + b] = origin[a][b];

	for (i = 0; i < 1024; i++) {
		k = i % 32;
		l = i / 32;
		if (date1[i] == 0) {
			for (b = 0; b < 8; b++) {
				for (a = 0; a < 8; a++) {
					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
				}
			}
		}
	}

	sprintf(output, "OUTPUT\\%d.bmp", name);
	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
	name++;

	printf(" end\n\n");
	return name;
}

void img_out2(unsigned char dct[][256], unsigned char ica[][256], int block_flag[1024], int name) {

	static int temp_ica[256 * 256];
	static int temp_dct[256 * 256];
	static int temp_sai[256 * 256];
	char output[1000];
	int a, b, i, l, k;
	//gp = _popen(GNUPLOT_PATH, "w");
	printf(" \n now output  - - - ->");
	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++) {
			temp_ica[a * 256 + b] = ica[a][b];
			temp_dct[a * 256 + b] = dct[a][b];
		}
	for (i = 0; i < 1024; i++) {
		k = i % 32;
		l = i / 32;
		if (block_flag[i] == 0) {
			for (b = 0; b < 8; b++) {
				for (a = 0; a < 8; a++) {
					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = temp_dct[256 * 8 * l + 8 * k + a + 256 * b];
				}
			}
		}
		else {
			for (b = 0; b < 8; b++) {
				for (a = 0; a < 8; a++) {
					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = temp_ica[256 * 8 * l + 8 * k + a + 256 * b];
				}
			}
		}
	}

	sprintf(output, "OUTPUT\\ProMet%d.bmp", name);
	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	printf(" end\n\n");



}