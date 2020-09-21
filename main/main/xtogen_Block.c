/* xtogen_Block.c */

/************************************************************/
/* 再構成画像のデータに直流成分を加えて画像のデータに変換します。*/
/************************************************************/

#include<stdio.h>
// gen -> 64*1024を256*256に変換
void xtogen_Block(double x[64], unsigned char gen[64], double avg[1024], int num) {

	int i, j, k, l;
	int block = 0;
	int sub = 0;
	double temp;

	for (k = 0; k < 64; k++) {
		temp = x[k] + avg[num];
		if (temp > 255)
			temp = 255;
		if (temp < 0)
			temp = 0;
		gen[k] = (unsigned char)temp;
	}
}