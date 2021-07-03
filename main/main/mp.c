/* mp.c */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "ica.h"
// mpans -> ブロック内最大値の添字
void mp(double y[][1024], double avg[1024], double w[][64], int mpans[][64]){
	FILE *fp;
	int i, j, k, l, key[1024] = {0}, count, hist[64][64] = {0};
	static unsigned char gen[256][256] = {0}, sai[256][256] = {0};
	static double x[64][1024] = {0}, temp[64][1024] = {0}, x2[64][1024] = {0}, y2[64][1024] = {0}, xori[64][1024] = {0};
	static double atemp[64] = {0}, btemp[64] = {0}, in[1024][64] = {0}, max, inans[1024][64] = {0}, inans2[1024][64] = {0};
	int gauge = 4;

	if ((fp = fopen("OUTPUT\\Matching_Pursuit.csv", "w")) == NULL) {
		fprintf(stderr,"Can not open file\n");
	}

	for(k = 0; k < 1024; k++)
		for(l = 0; l < 64; l++)
			y2[l][k] = y[l][k]; //ica係数をコピー

	seki(w, y2, xori); //ica係数とica基底の積
	//xtogen(xori, gen, avg);
	//gentox(gen, xori);

	seki(w, y2, x); //ica係数とica基底の積
	//xtogen(x, gen, avg);
	//gentox(gen, x);

	printf("\n Mutching pursuits START\n");


	for (count = 0; count < 64; count++) { //MP法の格納カウント

		for (j = 0; j < 64; j++) {//調査対象基底
			//temp -> 対象基底以外の係数を0 対象基底のみでのica基底との積
			for (k = 0; k < 1024; k++)
				for (l = 0; l < 64; l++)
					if (j == l)
						temp[l][k] = y2[l][k];
					else
						temp[l][k] = 0.0;

			seki(w, temp, x2);//積
			//xtogen(x2, gen, avg);
			//gentox(gen, x2);

			// atemp -> ica係数*基底, btemp ->直流なしica係数*基底
			// ブロックごとに処理
			for (i = 0; i < 1024; i++) {
				for (k = 0; k < 64; k++) {
					atemp[k] = x[k][i];
					btemp[k] = x2[k][i];
				}
				in[i][j] = inner_product(atemp, btemp); //直流なし再構成と対象基底のみ再構成の内積
			}
		}

		for (i = 0; i < 1024; i++) {
			max = -1;
			for (j = 0; j < 64; j++) {
				if (in[i][j] > max) {
					// ブロック内最大値格納
					max = in[i][j];
					key[i] = j;
				}
			}

			//最大値と添字を保存
			mpans[i][count] = key[i]; //MP法の優先度の決定
			inans[i][count] = max;

			y2[key[i]][i] = 0.0; //ica係数のブロック内番号jは０
		}

		//seki(w, y2, x);
		//xtogen(x, gen, avg);
		//gentox(gen, x);

		//for (i = 0; i < 1024; i++) {
		//	for (k = 0; k < 64; k++) {
		//		atemp[k] = xori[k][i]; // ica係数*基底
		//		btemp[k] = x[k][i];    // 最大値のところ0のica係数 * 基底
		//	}
		//	inans2[i][count] = inner_product(atemp, btemp);
		//}

		////count 4ずつに'とりこ'？
		//if (count % gauge == 0)
		//	printf("擒");

		printf("\r Now Running  :  [%3.3lf]", ((double)count / 64.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");


	// 確認用 fprint
	for(i = 0; i < 64; i++){
		for(j = 0; j < 1024; j++)
			fprintf(fp, "%d,", mpans[j][i]+1);
		fprintf(fp, "\n");
	}

	fprintf(fp,"\n\n");

	for(i = 0; i < 64; i++){
		for(j = 0; j < 1024; j++)
			fprintf(fp, "%lf,", inans[j][i]);
		fprintf(fp, "\n");
	}

	fprintf(fp,"\n\n");

	for(i = 0; i < 64; i++){
		for(j = 0; j < 1024; j++)
			fprintf(fp, "%lf,", inans2[j][i]);
		fprintf(fp, "\n");
	}

	for(i = 0; i < 64; i++)
		for(j = 0; j < 1024; j++)
			hist[i][mpans[j][i]]++;

	fprintf(fp,"\n\n");

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++)
			fprintf(fp, "%d,", hist[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}









