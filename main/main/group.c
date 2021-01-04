
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

// 画像出力用(領域指定[1024])
int group(double date1[65][1024], static char filename[20], int rate) {

	FILE* fp5;
	int count = 0, count4 = 0;
	int i, j, k, l, b, a, c, d, e, Q, QQ, QQQ = 0, basis[64][64], temp[2][1024], hist[64][64], temp2[64]; //basis[65][64] 使用基底ごとのヒストグラム
	double max, num[64], num2[1024], basis1[64], basis2[64], flag[64], flag2[64], gloup[64][64], gloup2[64][64][2], count2[64], count3[64][64], gloup3[64][64][2], temp3[64], temp4[65][1024];
	char out[50];


	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++)
			basis[i][j] = 0;
		for (b = 0; b < 64; b++)
			hist[b][i] = 0;
	}

	for (i = 0; i < 64; i++) {
		temp2[i] = 0;
		temp3[i] = 0;
		num[i] = 0;
		basis1[i] = 0;
		basis2[i] = 0;
		flag[i] = 0;
		count2[i] = 0;
		for (j = 0; j < 64; j++) {
			gloup[i][j] = 0;
			gloup2[i][j][0] = 99;
			gloup2[i][j][1] = 99;
			count3[i][j] = 0;
			gloup3[i][j][0] = 99;
			gloup3[i][j][1] = 99;
		}
	}
	for (i = 0; i < 1024; i++) {
		temp[0][i] = 99;
		temp[1][i] = 99;
		num2[i] = 99;
		for (b = 0; b < 65; b++)
			temp4[b][i] = 99;
	}

	max = 0;

	sprintf(out, "OUTPUT\\groupgroup[%d].txt", rate);

	if ((fp5 = fopen(out, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	printf(" now output  - - - ->");
	fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp5, "\n\n  Basis used in the ICA area \n\n\n  Number of basis used : 0 ~ 64 \n\n\n  Use basis -> $$ \n\n\n\n  Use Q rate : %d\n\n----------------------------------------------------------------------------------\n\n", rate);

	// 使用基底を位置フラグから番号に変換
	for (j = 0; j < 1024; j++) { // 使用基底個数をコピー
		temp4[64][j] = date1[64][j];
		//printf("%d : %d\n", (int)temp4[64][j], (int)date1[64][j]);
		//if(j%10 == 0)
			//printf("-------------\n");
	}
	for (j = 0; j < 1024; j++) { // フラグから番号へ
		QQ = 0;
		if ((int)date1[64][j] != 99) {
			for (i = 0; i < 64; i++) {
				if ((int)date1[i][j] == 1) {
					temp4[QQ][j] = i;
					QQ++;
				}
			}
		}
	} // temp4 -> 0~63 : 使用基底番号，64 : 使用基底数

	for (j = 0; j < 1024; j++) {
		if ((int)temp4[64][j] != 99) {
			for (i = 0; i < 64; i++) {
				if ((int)temp4[i][j] != 99) {
					basis[(int)temp4[64][j]][(int)temp4[i][j]]++; // 使用基底数：基底番号
				}
			}
		}
	}

	//for (j = 0; j < 64; j++) {
	//	for (i = 0; i < 64; i++) {
	//		temp3[i] = basis[j][i];
	//	}
	//	gnuplot5(temp3, (j + 1) * 1000);
	//}

	//for (j = 0; j < 64; j++) {
	//	for (i = 0; i < 64; i++) {
	//		basis[j][i] = 0;
	//	}
	//}
	//for (j = 0; j < 1024; j++) {

	//	if ((int)date1[64][j] != 99)
	//		for (i = 0; i < 64; i++)
	//			if ((int)date1[i][j] == 1)
	//				basis[(int)date1[64][j]][i]++;
	//}

	//for (j = 0; j < 64; j++) {
	//	for (i = 0; i < 64; i++)
	//		temp3[i] = basis[j][i];
	//	gnuplot5(temp3, (j + 1) * 1000);
	//}

	Q = 0;
	for (j = 0; j < 1024; j++) {
		max = 0;

		if ((int)temp4[64][j] != 99) {
			for (i = 0; i < 64; i++) {
				if ((int)temp4[i][j] != 99) {
					if (max < basis[(int)temp4[64][j]][(int)temp4[i][j]]) {
						max = basis[(int)temp4[64][j]][(int)temp4[i][j]];
						temp[0][j] = (int)temp4[i][j]; //グループ番号
					}
				}
			}
			temp[1][j] = (int)temp4[64][j]; // 使用基底数
			hist[(int)temp4[64][j]][temp[0][j]]++; // 使用基底数：基底番号
		}
	}


	for (j = 0; j < 64; j++) {
		for (i = 0; i < 64; i++)
			temp3[i] = hist[j][i];
		gnuplot5(temp3, rate*1000+j);
	}


	for (i = 2; i < 64; i++) { // 使用基底数
		for (j = 0; j < 64; j++) { // 基底（グループ）番号
			if (hist[i][j] == 1) { // ヒストグラムが1
				for (b = 0; b < 1024; b++) {
					max = 0;
					if (temp[1][b] == i && temp[0][b] == j) // ヒストグラムが1の使用基底数とグループ番号
						for (a = 0; a < 64; a++)
							if ((int)date1[a][b] == 1) { // ヒストグラムが1の領域内の使用基底
								for (c = 0; c < 64; c++) {
									for (e = 0; e < 64; e++)
										temp2[e] = 0;
									if (hist[i][c] > 2) // 使用基底のヒストグラム
										for (d = 0; d < 1024; d++)
											if (temp[1][d] == i && temp[0][d] == c)
												if ((int)date1[a][d] == 1)
													temp2[c]++;
								}
								for (e = 0; e < 64; e++)
									if (max < temp2[e])
										max = temp2[e];
							}
					temp[0][b] = (int)max; //グループ番号
				}
			}
		}
	}

	for (i = 0; i < 1024; i++)
		if (temp[1][i] == 2)
			temp3[temp[0][i]]++;

	//gnuplot2(temp3);

	//for (i = 0; i < 64; i++) {
	//	l = 0;
	//	for (k = 0; k < 64; k++)
	//		temp2[k] = gloup[i][k];
	//	l = rate * 100 + i;
	//	//gnuplot5(temp2, l);
	//}

	printf(" end\n\n");
	fclose(fp5);
	return filename;
}