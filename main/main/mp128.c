/* mp128.c */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "ica.h"

void mp128(unsigned char origin[][256], int Q, double y[][1024], double avg[1024], double w[][64], int mpans[][128]){
	FILE *fp;
	int i, j, k, l, n, key[1024] = { 0 }, count, hist[128][128] = { 0 };
	static unsigned char gen[256][256] = { 0 }, sai[256][256] = { 0 };
	static double  tempica[64][1024] = { 0 }, y2[64][1024] = { 0 }, temp[64][1024] = { 0 };
	static double atemp[64] = {0}, btemp[64] = {0}, in[1024][128] = {0}, max, inans[1024][128] = {0}, inans2[1024][64] = {0};

	static double coe[256][256] = { 0 };
	static double xcoe[64][1024] = { 0 }, z[64][1024] = { 0 }, tempcoe[64][1024] = { 0 };
	static unsigned char dammy[256][256] = { 0 };

	static double x[64][1024] = { 0 }, x2[64][1024] = { 0 }, x3[64][1024] = { 0 };
	static double xx[64][1024] = { 0 }, xx2[64][1024] = { 0 };

	int gauge = 8;

	if ((fp = fopen("Matching_Pursuit.csv", "w")) == NULL) {
		fprintf(stderr,"Can not open file\n");
	}


	//全基底を使用した信号 x の作成
	for(k = 0; k < 1024; k++)
		for(l = 0; l < 64; l++)
			y2[l][k] = y[l][k];

	seki(w, y2, x);
	//xtogen(x, gen, avg);
	//avg_inter(gen, avg);
	//gentox(gen, x);

	dct(origin, coe, 8);
	//quantization(coe, Q);
	gentox2(coe, xcoe);

	for (i = 0; i < 1024; i++){
		for (j = 0; j < 64; j++){
			z[j][i] = xcoe[j][i];
		}
	}

	xtogen2(z, coe);
	double_idct(coe, coe, 8);
	gentox2(coe, xcoe);

	for (i = 0; i < 1024; i++){
		for (j = 0; j < 64; j++){
			xx[j][i] = xcoe[j][i];
		}
	}

	//全ての基底を選択するため128回繰り返し
	printf("\n[0%%]--------[50%%]---------[100%%]\n");

	for(count = 0; count < 128; count++){
		//残差信号 x2 の作成
		for (n = 0; n < 128; n++){
			//128個の基底の内1個の基底だけ使った時のx2
			for (k = 0; k < 1024; k++){
				for (l = 0; l < 64; l++){
					if (n == l)
						tempica[l][k] = y2[l][k];
					else
						tempica[l][k] = 0.0;
				}
				for (l = 64; l < 128; l++){
					if (n == l)
						tempcoe[l - 64][k] = z[l - 64][k];
					else
						tempcoe[l - 64][k] = 0.0;
				}
			}

			seki(w, tempica, x2);
			//xtogen(temp, gen, avg);
			//avg_inter(gen, avg);
			//gentox(gen, temp);

			//周波数領域から空間領域に変換してやる
			//xtogen2(tempcoe, coe);
			double_idct(coe, coe, 8);
			gentox2(coe, tempcoe);

			for (i = 0; i < 1024; i++){
				for (j = 0; j < 64; j++){
					x3[j][i] = tempcoe[j][i];
				}
			}

			//内積を求める
			for(i = 0; i < 1024; i++){
				for(k = 0; k < 128; k++){
					if (k <= 63){
						atemp[k] = x[k][i];
						btemp[k] = x2[k][i];
						//printf("atemp=%lf\tk=%d\tn=%d\n", atemp[k], k, n);
						//printf("btemp=%lf\tk=%d\tn=%d\n", btemp[k], k, n);
					}
					else{
						atemp[k - 64] = xx[k - 64][i];
						btemp[k - 64] = x3[k - 64][i];
						//printf("atemp=%lf\t\tk=%d\tn=%d\n", atemp[k - 64], k, n);
						//printf("btemp=%lf\t\tk=%d\tn=%d\n", btemp[k - 64], k, n);
					}
				}
				in[i][n] = inner_product(atemp, btemp);

				//printf("x =%lf\tn=%d\ti=%d\n", x[0][i], n, i);
				//printf("x2=%lf\tn=%d\ti=%d\n", x2[0][i], n, i);
				//printf("x3=%lf\tn=%d\ti=%d\n", x3[0][i], n, i);
				//printf("atemp=%lf\tn=%d\ti=%d\n", atemp[0], n, i);
				//printf("btemp=%lf\tn=%d\ti=%d\n", btemp[0], n, i);
				//printf("in=%lf\ti=%d\tn=%d\n", in[i][n], i, n);
				//printf("\n\n");
			}

		}

		//for (i = 0; i < 1024; i++){
		//	for (j = 62; j < 66; j++){
		//		printf("%lf\ti=%d\tj=%d\n", in[i][j], i, j);
		//	}
		//	printf("\n\n\n\n");
		//}

		for (i = 0; i < 1024; i++){
			max = -1;
			for (n = 0; n < 128; n++){
				if (in[i][n] > max){
					max = in[i][n];
					key[i] = n;
				}
			}
			//printf("key=%d\tmax=%lf\ti=%d\n", key[i], max, i);
			mpans[i][count] = key[i];
			inans[i][count] = max;

			if (key[i] <= 63){
				y2[key[i]][i] = 0.0;
			}
			else{
				z[key[i] - 64][i] = 0.0;
			}
		}

		if(count % gauge == 0)
			printf("□");

	}
	printf("\n\n");

	for(i = 0; i < 128; i++){
		for(j = 0; j < 1024; j++)
			fprintf(fp, "%d,", mpans[j][i]+1);
		fprintf(fp, "\n");
	}

	fprintf(fp,"\n\n");

	for(i = 0; i < 128; i++){
		for(j = 0; j < 1024; j++)
			fprintf(fp, "%lf,", inans[j][i]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	for(i = 0; i < 128; i++)
		for(j = 0; j < 1024; j++)
			hist[i][mpans[j][i]]++;

	for(i = 0; i < 128; i++){
		for(j = 0; j < 128; j++)
			fprintf(fp, "%d,", hist[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}









