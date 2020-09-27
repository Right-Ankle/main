#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

int main()
{
	FILE* fp, * fp2, * fp3, * fp4, * fp5; //ファイルポインタの定義

	//宣言
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
	//static  double ori_temp2[64][1024] = { 0 };
	static int i, j, n, m, k, l, mk, ml, Q, QQ, QQQ, QQQQ, b, a, c, out_count =0, seg[64 * 64], img_out1[1024], img_out2[1024], img_out3[1024], img_out4[1024], y_rank[64][1024], y_rank_pm[64],seg0[64 * 64], seg1[64 * 64], ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai11[256 * 256], temp_sai22[256 * 256], temp_sai2[64][1024], temp_sai3[256][256], temp_sai4[64 * 64], ica[64], temp_temp[64];
	static double sum, sum0, sum1,sum11,sum22, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[1024], mse_ica[64][1024], mse_ica0[64][1024], mse_ica1[64][1024], cost_ica[1024], cost_dct[1024], mse_ica2[1024], result_dct[2][1024], result_ica[2][1024], result[2][1024], lambda = 1024.0;
	static double result_coe, coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 }, test[5][1024], test2[64][1024], test3[64][1024], ica_test[64][64][1024], ica_test2[2][64][1024], ica_test3[2][1024], ica_test4[2][1024];
	static double avg[1024], y0[64][1024], y1[64][1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dct_cost[64][1024], mse_cost[64][1024], ica_bent[1024], dct_bent[1024], ica_ent[64][1024], dct_ent[64][1024], dcoe_temp[64][1024] = { 0 };
	static unsigned char dammy[256][256] = { 0 };
	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
	static unsigned char  ica_sai[256][256] = { 0 }, ica_sai0[256][256] = { 0 }, ica_sai1[256][256] = { 0 };
	static struct pca pcaStr = { 0 };
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	char g[50];
	char output[1000], in[50];

	struct tt** sort_d, temp;
	double* temp_1;
	double* temp_2;
	double* temp_3;
	double* temp_4;
	double* temp_5;
	double* temp_6;

	sum = 0;

	printf("mkdir start\n");
	printf("+ - - - - - Now Running - - - - +\n");
	//フォルダ作成　(なければ作成)
	sprintf(g, "rmdir /s /q OUTPUT");
	system(g);
	_mkdir("OUTPUT"); //原画像のブロック画像を拡大して出力
	_mkdir("OUTPUT\\ORIGIN"); //原画像のブロック画像を拡大して出力
	_mkdir("OUTPUT\\ICA"); //ICA基底をそれぞれ拡大して出力
	_mkdir("OUTPUT\\ICA/COEFFICIENT"); //ICA基底をそれぞれ拡大して出力
	//for (j = 0; j < 1024; j++) {
	//	sprintf(g, "mkdir OUTPUT\\ICA\\COEFFICIENT\\coe[%d]", j);
	//	system(g);
	//	if (j % 128 == 0)
	//		printf(" @");
	//}
	_mkdir("OUTPUT\\DCT"); //実験中のICAとDCTの再構成画像を出力
	_mkdir("OUTPUT\\MSE"); //MSE比較後、基底ごとの領域で分割した画像を出力
	_mkdir("OUTPUT\\test"); //MSE比較後、基底ごとの領域で分割した画像を出力
	//_mkdir("DEFAULT"); //MSE比較後、基底ごとの領域で分割した画像を出力
	//for (j = 0; j < 1024; j++) {
	//	sprintf(g, "mkdir OUTPUT\\test\\MSE[%d]", j);
	//	system(g);
	//	if (j % 128 == 0)
	//		printf(" @");
	//}
	printf("\n\n");
//_mkdir("coe"); //原画像のブロック画像を拡大して出力
//	for (j = 0; j < 20; j++) {
//	sprintf(g, "mkdir coe\\coefficient[%d]", j);
//	system(g);
//}
	printf("mkdir end\n");

//読み込むファイル名
	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'c', 'a', 'm', 'e', 'r', 'a', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'l', 'a', 'x', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'l', 'e', 'n', 'n', 'a', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 's', 'a', 'i', 'l', 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'w', 'o', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'b', 'r', 'i', 'd', 'g', 'e', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'b', 'u', 'i', 'l', 'd', 'i', 'n', 'g', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'g', 'i', 'r', 'l', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'l', 'i', 'g', 'h', 't', 'h', 'o', 'u', 's', 'e', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 't', 'e', 'x', 't', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'e', 'a', 'r', 't', 'h', '.', 'b', 'm', 'p' };
	//static char filename[20] = { 'm', 'a', 'n', 'd', 'r', 'i', 'l', 'l', '.', 'b', 'm', 'p' };

	printf("\nfilename plz .... :");
	scanf("%s", filename);

	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	/* 一次元配列から二次元配列に変換 */
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin[i][j] = ori_temp[i * 256 + j];

	//ソート用
	sort_d = (struct tt**)malloc(sizeof(struct tt*) * 64);
	for (i = 0; i < 64; i++) {
		sort_d[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
	}

	temp_1 = (double*)malloc(sizeof(double) * 1024);
	temp_2 = (double*)malloc(sizeof(double) * 1024);
	temp_3 = (double*)malloc(sizeof(double) * 1024);
	temp_4 = (double*)malloc(sizeof(double) * 1024);
	temp_5 = (double*)malloc(sizeof(double) * 1024);
	temp_6 = (double*)malloc(sizeof(double) * 1024);


	//出力ファイル　宣言
	if ((fp = fopen("OUTPUT\\fp1.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp2 = fopen("OUTPUT\\fp2.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp3 = fopen("OUTPUT\\result_ical.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp4 = fopen("OUTPUT\\ICA.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp5 = fopen("OUTPUT\\ica.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	/////////////////宣言処理 終了///////////////////////


	// ///////////////////////// ica //////////////////////////////////
	// ICA基底・係数
	// origin = 画素値(256*256),  y = ica係数(ブロックで64個で1024ブロック分),  w = ica基底(64個の計算法の中にそれぞれ64個の計算法がある)
	// ICAに"origin"を入れることで"y"(計算後の値)と"w"(計算の仕方)の結果が出力される
	// 基底は計算方法。係数は 8*8の画素ブロックを構成するのに 64個の基底がそれぞれ どれくらい使われているのか（含まれているか）の値。
	// ブロックとは 256*256画素のうち縦8横8のブロック。一画像につき(256/8) 32*32 = 1024ブロック
	pcaStr = new_pca(origin);
	ICA(origin, pcaStr, y, w, avg, 100, 0.002);

	// 計算用にコピー
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			ny[j][i] = y[j][i]; // ny -> yy(ica係数コピー)

	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			nw[j][i] = w[j][i]; // nw-> w(ica基底コピー)


	// 64個の基底のうち1個だけ使用するための処理
	// 基底はいじれないから、使用する係数を64個から1個に制限する。
	// ブロック内で使用する基底を1個×64個あるから64パターン×全1024ブロック
	printf("+ - - - - - Now Running - - - - +\n");
	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 64; i++) {
			b = i;

			// 該当係数以外0
			// i番目の係数（基底）のみ使用。それ以外の係数は0。
			for (a = 0; a < 64; a++) {
				if (b == a)
					ny[a][j] = y[a][j];
				else
					ny[a][j] = 0;
			}

			// 初期化（必ず行う）
			for (a = 0; a < 64; a++)
				xx[a] = 0.0;

			// 1ブロックで処理を行っているため、そのブロック番号（ｊ）の係数と
			// すべての基底を用いることで もとのブロックを再構成する処理
			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
			avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②


			// ブロックごとのMSE
			// MSEは（元の値 - 再構成の値）^2をすることで
			// 再構成した値が元の値とどれくらいずれているのかを見るための指標
			sum = 0.0;
			sum2 = 0.0;
			mk = j % 32;
			ml = j / 32;

			// 64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
			// （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
			for (a = 0; a < 8; a++) {
				for (b = 0; b < 8; b++) {
					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
				}
			}
			mse_ica[i][j] = sum / 64;//平均
		}
		// 実行確認用
		if (j % 64 == 0)
			printf(" @");
	}
	printf("\n\n");

	// ///////////// ica MSE ソート/////////////////////////
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			// .val -> 値を取得・属性を変更し記憶
			// .abs -> 絶対値を記憶
			// .num -> 元々の係数に対応するブロック内番号を記憶
			sort_d[i][j].val = mse_ica[i][j];		/* 元々の係数値 */
			sort_d[i][j].abs = fabs(mse_ica[i][j]);	/* ソートは係数の絶対値で行う*/
			sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
		}
	}

	for (n = 0; n < 1024; n++) {
		for (i = 0; i < 64 - 1; i++) {
			min = sort_d[i][n].abs;
			k = i;
			for (j = i + 1; j < 64; j++) {
				if (sort_d[j][n].abs < min) {
					min = sort_d[j][n].abs;
					k = j;
				}
			}
			temp = sort_d[i][n];
			sort_d[i][n] = sort_d[k][n];
			sort_d[k][n] = temp;
		}
	}

	////////////////////////////ソート終了////////////////////////////////

	//ソート結果を格納
	// 各ブロック64パターンの中で一番誤差のないもの
	for (i = 0; i < 1024; i++) {
		result_ica[0][i] = sort_d[0][i].num;  // 基底番号
		result_ica[1][i] = sort_d[0][i].val;    // MSE値
		//printf("%lf : %lf\n", result_ica[0][i], (double)sort_d[0][i].num);
	}

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			// .val -> 値を取得・属性を変更し記憶
			// .abs -> 絶対値を記憶
			// .num -> 元々の係数に対応するブロック内番号を記憶
			sort_d[i][j].val = y[i][j];		/* 元々の係数値 */
			sort_d[i][j].abs = fabs(y[i][j]);	/* ソートは係数の絶対値で行う*/
			sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
		}
	}

	for (n = 0; n < 1024; n++) {
		for (i = 0; i < 64 - 1; i++) {
			max = sort_d[i][n].abs;
			k = i;
			for (j = i + 1; j < 64; j++) {
				if (sort_d[j][n].abs > max) {
					max = sort_d[j][n].abs;
					k = j;
				}
			}
			temp = sort_d[i][n];
			sort_d[i][n] = sort_d[k][n];
			sort_d[k][n] = temp;
		}
	}

	for (n = 0; n < 1024; n++) {
		for (i = 0; i < 64; i++) {
			y_rank[i][n] = sort_d[i][n].num;//係数順位
		}
	}



	// use dobule ica///////////////////////

	// 1 start
	printf("Method 1 start - ->");
	for (j = 0; j < 1024; j++) {
		for (m = 0; m < 64; m++) {
			for (i = 0; i < 64; i++)
				ny[i][j] = 0;

			ny[(int)result_ica[0][j]][j] = y[(int)result_ica[0][j]][j]; //一つ目の基底選択
			ny[m][j] = y[m][j]; // ２つ目の基底選択

				// 初期化（必ず行う）
			for (a = 0; a < 64; a++)
				xx[a] = 0.0;

			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
			avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

			sum = 0.0;
			mk = j % 32;
			ml = j / 32;
			for (a = 0; a < 8; a++) {
				for (b = 0; b < 8; b++) {
					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
				}
			}

			mse_ica1[m][j] = sum / 64.0;
		}
	}

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			// .val -> 値を取得・属性を変更し記憶
			// .abs -> 絶対値を記憶
			// .num -> 元々の係数に対応するブロック内番号を記憶
			sort_d[i][j].val = mse_ica1[i][j];		/* 元々の係数値 */
			sort_d[i][j].abs = fabs(mse_ica1[i][j]);	/* ソートは係数の絶対値で行う*/
			sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
		}
	}

	for (n = 0; n < 1024; n++) {
		for (i = 0; i < 64 - 1; i++) {
			min = sort_d[i][n].abs;
			k = i;
			for (j = i + 1; j < 64; j++) {
				if (sort_d[j][n].abs < min) {
					min = sort_d[j][n].abs;
					k = j;
				}
			}
			temp = sort_d[i][n];
			sort_d[i][n] = sort_d[k][n];
			sort_d[k][n] = temp;
		}
	}

	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++) {
			dct_coe[i][j] = (double)sort_d[i][j].num;
			dcoe_temp[i][j] = sort_d[i][j].val;
		}
	printf(" end\n\n");
	/////////  1 end

	// 2 start ///////
	//printf("Method 2 start - ->\n");
	//printf("+ - - - - - Now Running - - - - +\n");
	//for (j = 0; j < 1024; j++) {
	//	for (m = 0; m < 64; m++)
	//		for (n = 0; n < 64; n++) {
	//			for (i = 0; i < 64; i++)
	//				ny[i][j] = 0;

	//			ny[m][j] = y[m][j]; //一つ目の基底選択
	//			ny[n][j] = y[n][j]; // ２つ目の基底選択

	//			// 初期化（必ず行う）
	//			for (a = 0; a < 64; a++)
	//				xx[a] = 0.0;

	//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
	//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
	//			avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

	//			sum = 0.0;
	//			mk = j % 32;
	//			ml = j / 32;
	//			for (a = 0; a < 8; a++) {
	//				for (b = 0; b < 8; b++) {
	//					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
	//				}
	//			}

	//			ica_test[n][m][j] = sum / 64.0;
	//		}
	//	if (j % 64 == 0)
	//		printf(" @");
	//}
	//printf("\n\n");


	//for (l = 0; l < 1024; l++) {
	//	// 64*64*1024 -> 最小ＭＳＥ　64*1024こ
	//	for (m = 0; m < 64; m++) {
	//		for (i = 0; i < 64; i++) {
	//			// .val -> 値を取得・属性を変更し記憶
	//			// .abs -> 絶対値を記憶
	//			// .num -> 元々の係数に対応するブロック内番号を記憶
	//			sort_d[i][m].val = ica_test[i][m][l];		/* 元々の係数値 */
	//			sort_d[i][m].abs = fabs(ica_test[i][m][l]);	/* ソートは係数の絶対値で行う*/
	//			sort_d[i][m].num = i;					/* numに元々の係数に対応する番号を記憶 */
	//		}
	//	}

	//	for (n = 0; n < 64; n++)
	//		for (i = 0; i < 64 - 1; i++) {
	//			min = sort_d[i][n].abs;
	//			k = i;
	//			for (j = i + 1; j < 64; j++) {
	//				if (sort_d[j][n].abs < min) {
	//					min = sort_d[j][n].abs;
	//					k = j;
	//				}
	//			}
	//			temp = sort_d[i][n];
	//			sort_d[i][n] = sort_d[k][n];
	//			sort_d[k][n] = temp;
	//		}

	//	for (m = 0; m < 64; m++) {
	//		ica_test2[0][m][l] = sort_d[0][m].val;//mse
	//		ica_test2[1][m][l] = (double)sort_d[0][m].num;//基底番号

	//	}
	//}

	//	for (j = 0; j < 1024; j++) {
	//		for (i = 0; i < 64; i++) {
	//			// .val -> 値を取得・属性を変更し記憶
	//			// .abs -> 絶対値を記憶
	//			// .num -> 元々の係数に対応するブロック内番号を記憶
	//			sort_d[i][j].val = ica_test2[0][i][j];		/* 元々の係数値 */
	//			sort_d[i][j].abs = fabs(ica_test2[0][i][j]);	/* ソートは係数の絶対値で行う*/
	//			sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
	//		}
	//	}

	//	for (n = 0; n < 1024; n++) {
	//		for (i = 0; i < 64 - 1; i++) {
	//			min = sort_d[i][n].abs;
	//			k = i;
	//			for (j = i + 1; j < 64; j++) {
	//				if (sort_d[j][n].abs < min) {
	//					min = sort_d[j][n].abs;
	//					k = j;
	//				}
	//			}
	//			temp = sort_d[i][n];
	//			sort_d[i][n] = sort_d[k][n];
	//			sort_d[k][n] = temp;
	//		}
	//	}

	//	for (j = 0; j < 1024; j++) {
	//		ica_test3[0][j] = sort_d[0][j].val;//mse
	//		ica_test3[1][j] = (double)sort_d[0][j].num;//基底番号

	//		ica_test4[0][j] = ica_test3[1][j];//1つ目の基底番号
	//		ica_test4[1][j] = ica_test2[1][(int)ica_test3[1][j]][j];// 2つ目の基底番号
	//	}

	//	printf("Method 2 end\n\n");
	//		////////////// 2 fin ///////////////////////

	//				// Method 1:2 の1個目と2個目の基底の係数順位を比較中
	//		for (a = 0; a < 256; a++)
	//			for (b = 0; b < 256; b++)
	//				temp_sai[a * 256 + b] = origin[a][b];

	//		for (i = 0; i < 1024; i++) {
	//			k = i % 32;
	//			l = i / 32;
	//			if (dcoe_temp[0][i] == ica_test3[0][i]) {
	//				for (b = 0; b < 8; b++) {
	//					for (a = 0; a < 8; a++) {
	//						temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
	//					}
	//				}
	//			}
	//		}

	//		sprintf(output, "OUTPUT\\MSE_test.bmp");
	//		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
            fprintf(fp, "\n\n Use image  :  %s\n\n\n",filename);
			fprintf(fp, "\n\n  [Block No] | Basis Number order | MSE order | Coefficient order\n\n\n\n------------------\n\n");
			QQ = 0;
			QQQ = 0;
			QQQQ = 0;
			mk = ml = 0;

				for (b = 0; b < 1024; b++) {
					img_out1[b] = 0;
					img_out2[b] = 0;
				}
				i = 0;
				for (b = 0; b < 1024; b++) {
					QQ = 0;
					QQQ = 0;
					QQQQ = 0;
					for (j = 0; j < 64; j++) {
						if (y[y_rank[j][b]][b] > 0) {
							y_rank_pm[(int)y_rank[j][b]] = QQQ;
							QQQ++;
						}
						else {
							y_rank_pm[(int)y_rank[j][b]] = QQQQ;
							QQQQ++;
						}
						if (y_rank[j][b] == (int)result_ica[0][b])
							QQ = j;
					}
					fprintf(fp, " -- [area No.%d] ------------------------------------------------------------------------------------------  \n\n", b);
					fprintf(fp, " ~~~~  BEST MSE [basis No.%d]  mse = %5.4f   ~~~~ \n\n",(int)result_ica[0][b], result_ica[1][b]);
					for (a = 0; a < 64; a++) {



						fprintf(fp, " [%4d]   |   [%d][%d] : mse = %5.4f   ", a, (int)result_ica[0][b], a, mse_ica1[a][b]);
						fprintf(fp, " |    [%d][%d] : mse = %5.4f  ", (int)result_ica[0][b], (int)dct_coe[a][b], dcoe_temp[a][b]);

						if (y[(int)result_ica[0][b]][b] > 0) {
							if (y[(int)dct_coe[a][b]][b] > 0)
								fprintf(fp, " --->  [+%d][+%d]   ", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)dct_coe[a][b]]);
							else
								fprintf(fp, " --->  [+%d][-%d]   ", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)dct_coe[a][b]]);
						}
						else {
							if (y[(int)dct_coe[a][b]][b] > 0)
								fprintf(fp, " --->  [-%d][+%d]   ", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)dct_coe[a][b]]);
							else
								fprintf(fp, " --->  [-%d][-%d]   ", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)dct_coe[a][b]]);
						}

						if (mse_ica1[(int)y_rank[a][b]][b] == dcoe_temp[0][b])
							fprintf(fp, " |   ' [%d][%d] : mse = %5.4f ' ", (int)result_ica[0][b], (int)y_rank[a][b], mse_ica1[(int)y_rank[a][b]][b]);
						else
							fprintf(fp, " |   [%d][%d] : mse = %5.4f  ", (int)result_ica[0][b], (int)y_rank[a][b], mse_ica1[(int)y_rank[a][b]][b]);

						if (y[y_rank[a][b]][b] > 0)
							fprintf(fp, " --->  [%d][+%d]  \n\n ", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)y_rank[a][b]]);
						else
							fprintf(fp, " --->  [%d][-%d]  \n\n", y_rank_pm[(int)result_ica[0][b]], y_rank_pm[(int)y_rank[a][b]]);





						//fprintf(fp, " [%f]\n\n", result_ica[1][b] - dcoe_temp[0][b]);//１個目と2個目のMSE差
						//temp_1[i] = (result_ica[0][b] + dct_coe[0][b]) / 2;//順位平均
						//temp_2[b] = result_ica[1][b] - dcoe_temp[0][b];//MSE改善量
						//temp_3[i] = dcoe_temp[0][b];//MSE値
						//temp_4[i] = (fabs(y[(int)result_ica[0][b]][b]) + fabs(y[(int)dct_coe[0][b]][b])) / 2 - (fabs(y[(int)ica_test4[0][b]][b]) + fabs(y[(int)ica_test4[1][b]][b])) / 2;//係数値の平均の差
						//temp_5[b] = (fabs(y[(int)result_ica[0][b]][b]) + fabs(y[(int)dct_coe[0][b]][b])) / 2; //係数値の平均
						//temp_6[i] = max(fabs(y[(int)result_ica[0][b]][b]), fabs(y[(int)dct_coe[0][b]][b])); //係数値の大きいほう
						//i++;
					}
					fprintf(fp, "\n\n\n\n");
				}

			//out_count = img_out(origin, img_out1, out_count);

				//sum0 = sum1 = sum2 = sum11 = sum22 = result_coe = 0;
				//for (j = 0; j < i; j++) {
				//	sum1 += temp_2[j];
				//	sum2 += temp_5[j];
				//}

				//sum1 /= (double)i;
				//sum2 /= (double)i;

				//for (j = 0; j < i; j++) {
				//	sum0 += (temp_2[j] - sum1) * (temp_5[j] - sum2);
				//	sum11 += pow(temp_2[j] - sum1, 2);
				//	sum22 += pow(temp_5[j] - sum2, 2);
				//}

				//result_coe = (sum0 / i) / (sqrt(sum11 / i) * sqrt(sum22 / i));
				//fprintf(fp, " \n\n\n average of coefficient : amount of improvement= %lf", result_coe);

				// ２個目を選択する過程の調査中
				for (a = 0; a < 1024; a++) {
					for (b = 0; b < 64; b++) {
						test2[b][a] = mse_ica1[y_rank[b][a]][a] - result_ica[1][a]; //番号順
						test3[b][a] = dcoe_temp[b][a] - result_ica[1][a]; //MSE順
					}
				}
				gnuplot(test2);
				//gnuplot(test3);


///////// 2 fin//////////////////
	//動作確認
			printf("<ica fin>\n\n");
			/////////////////////////////////ica 終了/////////////////////////////////////////


			// //////////////////////////// dct ////////////////////////////////////////
			// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

			//動作確認
			printf("<dct start>\n");


			// 10段階品質があるから10段階分やる
			//for (Q = 100; Q > 0; Q -= 10) {
			//	printf("now Q is %d\n", Q);
			Q = 100;

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			dctcoe_conv(dcoe, dct_ent);

			for (i = 0; i < 64; i++) {

				// 該当係数以外0
				// i番目の係数（基底）のみ使用。それ以外の係数は0。
				for (j = 0; j < 1024; j++) {
					for (a = 0; a < 64; a++) {
						if (i == a)
							ny[a][j] = dct_ent[a][j];
						else
							ny[a][j] = 0;
					}
				}
				xtogen2(ny, dcoe);
				idct(dcoe, dcoe2, 8); // 普通の再構成

				for (j = 0; j < 1024; j++) {

					// ブロックごとのMSE
					// MSEは（元の値 - 再構成の値）^2をすることで
					// 再構成した値が元の値とどれくらいずれているのかを見るための指標
					sum = 0.0;
					sum2 = 0.0;
					mk = j % 32;
					ml = j / 32;

					// 64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
					// （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
					for (a = 0; a < 8; a++) {
						for (b = 0; b < 8; b++) {
							sum += pow(origin[ml * 8 + b][mk * 8 + a] - dcoe2[ml * 8 + b][mk * 8 + a], 2);
						}
					}
					mse_dct[i][j] = sum / 64;//平均
				}
				// 実行確認用
				//printf("%d\n", i);
				//if (i == 0) {
				//	for (a = 0; a < 256; a++)
				//		for (b = 0; b < 256; b++)
				//			temp_sai[a * 256 + b] = dcoe2[a][b];

				//	sprintf(output, "test.bmp");
				//	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
				//}

			}
			/////////////////////////////////////////////////////////
			for (b = 0; b < 1024; b++) {
				fprintf(fp2, "\n --- [%d] --- \n\n", b);
				for (a = 0; a < 64; a++) {
					fprintf(fp2, " [%d][%d]  -->  %lf\n", a, b, mse_dct[a][b]);
				}
			}


			//for (a = 0; a < 256; a++)
			//	for (b = 0; b < 256; b++)
			//		temp_sai[a * 256 + b] = dcoe2[a][b];

			//sprintf(output, "DEFAULT/DCT[%d].bmp", Q);
			//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
			//
			//b_entropy_dct(dcoe, dct_bent);
			//b_mse(origin, dcoe2, mse_dct2);
			//make_cost(dct_bent, mse_dct2, cost_dct);

			// 256*256->64*1024（係数の順番を変更)
			//  256*256だと0番目のブロックの値は [0~7]*[0~7]に格納されている（1番目だと[0~7]*[8~15]) が、
			//  64*1024にすれば[0~64]*[0]と計算しやすく・わかりやすくなる

		   // ブロックごとのMSE
			sum = 0.0;
			sum2 = 0.0;
			mk = j % 32;
			ml = j / 32;
			/////////////////////////dct 終了/////////////////////////////

			//if (Q == 100) {
			//for (a = 0; a < 256; a++)
			//	for (b = 0; b < 256; b++)
			//		temp_sai[a * 256 + b] = dcoe3[a][b];

		//}

			// dct係数をソート
			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					// .val -> 値を取得・属性を変更し記憶
					// .abs -> 絶対値を記憶
					// .num -> 元々の係数に対応するブロック内番号を記憶
					sort_d[i][j].val = dct_ent[i][j];		/* 元々の係数値 */
					sort_d[i][j].abs = fabs(dct_ent[i][j]);	/* ソートは係数の絶対値で行う*/
					sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
				}
			}

			for (n = 0; n < 1024; n++) {
				for (i = 0; i < 64 - 1; i++) {
					max = sort_d[i][n].abs;
					k = i;
					for (j = i + 1; j < 64; j++) {
						if (sort_d[j][n].abs > max) {
							max = sort_d[j][n].abs;
							k = j;
						}
					}
					temp = sort_d[i][n];
					sort_d[i][n] = sort_d[k][n];
					sort_d[k][n] = temp;
				}
			}
			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++) {
					dct_coe[i][j] = (double)sort_d[i][j].num;//dct_coe -> ソート後のdct係数順位
					dcoe_temp[i][j] = sort_d[i][j].val;
				}
			////////////////////////////

					// dctのMSEをソート
			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					// .val -> 値を取得・属性を変更し記憶
					// .abs -> 絶対値を記憶
					// .num -> 元々の係数に対応するブロック内番号を記憶
					sort_d[i][j].val = mse_dct[i][j];		/* 元々の係数値 */
					sort_d[i][j].abs = fabs(mse_dct[i][j]);	/* ソートは係数の絶対値で行う*/
					sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
				}
			}

			for (n = 0; n < 1024; n++) {
				for (i = 0; i < 64 - 1; i++) {
					max = sort_d[i][n].abs;
					k = i;
					for (j = i + 1; j < 64; j++) {
						if (sort_d[j][n].abs > max) {
							max = sort_d[j][n].abs;
							k = j;
						}
					}
					temp = sort_d[i][n];
					sort_d[i][n] = sort_d[k][n];
					sort_d[k][n] = temp;
				}
			}

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					mse_dct[i][j] = sort_d[i][j].val;//dct_coe -> ソート後のmse

			/// //////////////////////////////


			//　最小ＭＳＥの係数順位のグラフ出力


			////////////////出力///////////////////

			//////////////////出力終了///////////////////////
		//} // dctの最初に戻る

			fclose(fp);
			fclose(fp2);
			fclose(fp3);
			fclose(fp4);
			fclose(fp5);
			//gnuplot(dcoe_temp);
			for (i = 0; i < 64; i++) {
				free(sort_d[i]);
			}

			free(sort_d);
			free(temp_1);
			free(temp_2);
			free(temp_3);
			free(temp_4);
			free(temp_5);
			free(temp_6);
			printf(" All finish");

}
