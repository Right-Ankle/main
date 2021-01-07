/* block_psnr.c */

/************************************/
/*  PSNRをブロックごとに計算します。	*/
/************************************/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>

void block_psnr(unsigned char origin[][256], unsigned char gen[][256], double b_psnr[1024]){
	FILE *fp;
	int i, j, k, l, m, n, count = 0; 
	double sum = 0.0;
	static unsigned char ori_temp[64][1024] = {0}, gen_temp[64][1024] = {0};

	if((fp = fopen("b_psnr.pgm","wb")) == NULL){
		fprintf(stderr,"Can not open file\n");
	}

	for(i = 0; i < 1024; i++)
		b_psnr[i] = 0.0;

	n = 0;
	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					ori_temp[m][n] = origin[i+k][j+l];
					m++;
				}
			}
			n++;
		}
	}

	n = 0;
	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					gen_temp[m][n] = gen[i+k][j+l];
					m++;
				}
			}
			n++;
		}
	}

	for(i = 0; i < 1024; i++){

		sum = 0.0;

		for(j = 0; j < 64; j++){
			sum += pow(ori_temp[j][i] - gen_temp[j][i], 2);
		}

		b_psnr[i] = 10 * log10((255 * 255) / (sum / (8 * 8)));
	}

	fprintf(fp,"P5\n256 256\n255\n");
	fwrite(gen, sizeof(unsigned char), 256 * 256, fp);

	fclose(fp);
}
