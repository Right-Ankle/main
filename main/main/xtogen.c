/* xtogen.c */

/************************************************************/
/* 再構成画像のデータに直流成分を加えて画像のデータに変換します。*/
/************************************************************/

#include<stdio.h>
// gen -> 64*1024を256*256に変換
void xtogen(double x[][1024], unsigned char gen[][256], double avg[1024]){

	int i, j, k, l;
	int block = 0;
	int sub = 0;
	double temp;

	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			sub = 0;

			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					temp = x[sub][block] + avg[block];
					if(temp > 255)
						temp = 255;
					if(temp < 0)
						temp = 0;
					gen[i+k][j+l] = (unsigned char)temp;
					sub++;
				}
			}
			block++;
		}
	}
}

void xtogen2(double x[][1024], double gen[][256]){
	int i, j, k, l;
	int block = 0;
	int sub = 0;

	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			sub = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					gen[i + k][j + l] = x[sub][block];
					sub++;
				}
			}
			block++;
		}
	}

}