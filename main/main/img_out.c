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
	int a, b, i, l, k, Q;

	//gp = _popen(GNUPLOT_PATH, "w");
	printf(" \n now outputing  - - - ->");
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

	sprintf(output, "OUTPUT\\%d.bmp", (int)name);
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
	printf(" \n now outputing  - - - ->");
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

void img_out3(unsigned char origin[][256]) {

	char output[1000];
	static int origin_temp[256 * 256] = {0};
	static int out[64*64];

	int a, b, c, d, i, l, k;
	//gp = _popen(GNUPLOT_PATH, "w");
	printf(" \n now outputing  - - - ->");
	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++) {
			origin_temp[a * 256 + b] = origin[a][b];
		}

	for (i = 0; i < 1024; i++) {
		k = i % 32;
		l = i / 32;

		for (b = 0; b < 8; b++) {
			for (d = 0; d < 8; d++) {
				for (a = 0; a < 8; a++) {
					for (c = 0; c < 8; c++) {
						out[64 * 8 * b + 64 * d + 8 * a + c] = origin_temp[256 * 8 * l + 8 * k + a + 256 * b];
					}
				}
			}
		}

		sprintf(output, "OUTPUT\\ORIGIN\\%d.bmp", i);
		img_write_gray(out, output, 64, 64); // outputに出力画像を書き出す

	}
	printf(" end\n\n");


}