/* mp2.c */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "ica.h"

void mp2(unsigned char origin[][256], int Q, double y[][1024], double avg[1024], double w[][64], int mpans[][128]){
	FILE *fp;
	int i, j, k, l, n, key[1024] = { 0 }, count, hist[128][128] = { 0 };
	static unsigned char gen[256][256] = { 0 }, sai[256][256] = { 0 };
	static double  tempica[64][1024] = { 0 }, temp[64][1024] = { 0 };
	static double atemp[64] = {0}, btemp[64] = {0}, in[1024][128] = {0}, max, inans[1024][128] = {0}, inans2[1024][64] = {0};

	static double coe[256][256] = { 0 };
	static double xcoe[64][1024] = { 0 }, tempcoe[64][1024] = { 0 };
	static unsigned char dammy[256][256] = { 0 };

	//ICA ŒW”@64ƒtƒ‹@c·
	static double yy[64][1024] = { 0 }, x[64][1024] = { 0 }, x2[64][1024] = { 0 };

	//DCT
	static double zz[64][1024] = { 0 }, z[64][1024] = { 0 }, z2[64][1024] = { 0 };

	int gauge = 8;

	if ((fp = fopen("Matching_Pursuit.csv", "w")) == NULL) {
		fprintf(stderr,"Can not open file\n");
	}

	//‘SŠî’ê‚ğg—p‚µ‚½M†(DCT:z‹óŠÔ—ÌˆæCICA:xŠî’ê‚ÆŒW”‚ÌÏ)‚Ìì¬
	//DCT
	dct(origin, coe, 8);
	gentox2(coe, zz);	//ü”g”—Ìˆæ‚ğ[64][1024]‚Åzz‚É‘ã“ü

	//DC¬•ª‚Íœ‚­
	//for (i = 0; i < 1024; i++)
	//	zz[0][i] = 0.0;

	xtogen2(zz, coe);
	double_idct(coe, coe, 8);
	gentox2(coe, z);	//‹óŠÔ—Ìˆæ‚ğ[64][1024]‚Åz‚É‘ã“ü

	//ICA
	for (k = 0; k < 1024; k++)
		for (l = 0; l < 64; l++)
			yy[l][k] = y[l][k];

	seki(w, yy, x);


	printf("\n[0%%]--------[50%%]---------[100%%]\n");
	//128‚Ì“àÏ‚Ì”äŠr‚É‚æ‚è—Dæ‡ˆÊ‚ğ‹‚ß‚é
	for (count = 0; count < 128; count++){



		////“àÏ‚ğ‹‚ß‚é
		for (n = 0; n < 128; n++){

			//c·M†(DCT:‹óŠÔ—ÌˆæCICA : Šî’ê‚ÆŒW”‚ÌÏ)‚Ìì¬
			for (k = 0; k < 1024; k++){
				//ICA
				if (n <= 63){
					for (l = 0; l < 64; l++){
						if (n == l)
							tempica[l][k] = yy[l][k];
						else
							tempica[l][k] = 0.0;
					}
				}
				//DCT
				else{
					for (l = 64; l < 128; l++){
						if (n == l)
							tempcoe[l - 64][k] = zz[l - 64][k];
						else
							tempcoe[l - 64][k] = 0.0;
					}
				}
			}

			//c·M†‚ğŠi”[
			if (n <= 63){
				//ICA
				seki(w, tempica, x2);
			}
			else{
				//DCT, ü”g”—Ìˆæ‚©‚ç‹óŠÔ—Ìˆæ‚É•ÏŠ·
				xtogen2(tempcoe, coe);
				double_idct(coe, coe, 8);
				gentox2(coe, z2);
			}

			//“àÏ‚ğZo‚µCŠi”[
			for (i = 0; i < 1024; i++){
				//ICA
				if (n <= 63){
					for (k = 0; k < 64; k++){
						atemp[k] = x[k][i];
						btemp[k] = x2[k][i];
					}
				}
				//DCT
				else{
					for (k = 0; k < 64; k++){
						atemp[k] = z[k][i];
						btemp[k] = z2[k][i];
					}
				}
				in[i][n] = inner_product(atemp, btemp);


				//if (n >= 62 && i == 5){
				//	for (k = 0; k < 64; k++){
				//		printf("k=%d\ti=%d\tn=%d\n", k, i, n);
				//		printf("atemp=%lf\n", atemp[k]);
				//		printf("btemp=%lf\n", btemp[k]);
				//	}
				//	printf("============================\n");
				//	printf("in=%lf\n", in[i][n]);
				//}

			}
		}

		//128x1024 ‚Ì“àÏ‚ğ‹‚ß‚½

		//ƒuƒƒbƒN‚²‚Æ‚ÉÅ‘å‚Ì“àÏ‚ğ‹‚ßCÅ‘å‚Æ‚È‚Á‚½Šî’ê‚ÌŒW”‚ğ0‚É‚·‚éi‚Q‰ñ–Ú‚Í‚PˆÊ‚ğœ‚¢‚Ä—Dæ‡ˆÊ‚ğ‹‚ß‚éj
		for (i = 0; i < 1024; i++){
			max = -1;
			for (n = 0; n < 128; n++){
				if (in[i][n] > max){
					max = in[i][n];
					key[i] = n;
				}
			}
			mpans[i][count] = key[i];
			inans[i][count] = max;

			if (key[i] <= 63){
				yy[key[i]][i] = 0.0;
			}
			else{
				zz[key[i] - 64][i] = 0.0;
			}
		}

		seki(w, yy, x);

		xtogen2(zz, coe);
		double_idct(coe, coe, 8);
		gentox2(coe, z);

		if (count % gauge == 0)
			printf(" ");
	}
	printf("\n\n");

	for (i = 0; i < 128; i++){
		for (j = 0; j < 1024; j++)
			fprintf(fp, "%d,", mpans[j][i] + 1);
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	for (i = 0; i < 128; i++){
		for (j = 0; j < 1024; j++)
			fprintf(fp, "%lf,", inans[j][i]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	for (i = 0; i < 128; i++)
		for (j = 0; j < 1024; j++)
			hist[i][mpans[j][i]]++;

	for (i = 0; i < 128; i++){
		for (j = 0; j < 128; j++)
			fprintf(fp, "%d,", hist[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}









