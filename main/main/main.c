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
	static int i, j, n, m, k, l, mk, ml, Q, QQ, QQQ, QQQQ, b, a, c, out_count = 0, seg[64 * 64], img_out1[1024], img_out2[1024], img_out3[1024], img_out4[1024], y_rank[64][1024], y_rank_pm[64], seg0[64 * 64], seg1[64 * 64], ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai11[256 * 256], temp_sai22[256 * 256], temp_sai2[64][1024], temp_sai3[256][256], temp_sai4[64 * 64], ica[64], temp_temp[64], temp1[64], temp2[64], temp3[64], temp4[64], temp5[64], temp6[64], count_temp[4][1024];
	static double percent, sum, sum0, sum1, sum11, sum22, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[1024], mse_ica[64][1024], mse_ica0[64][1024], mse_ica1[64][1024], cost_ica[1024], cost_dct[1024], total_mse[3][64], result_dct[2][1024], result_ica[2][1024], result_ica0[2][1024], y3[3][1024], lambda = 1024.0;
	static double result_coe, coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 }, test[5][1024], test2[64][1024], test3[64][1024], ica_test[64][64][1024], ica_test2[2][64][1024], ica_test3[2][1024], ica_test4[2][1024], ica_test5[64][64][64], ica_test0[64][1024], ica_test1[64][64], average2[4][2], test_per[4][64];
	static double avg[1024], y0[64][1024], y1[64][1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dct_cost[64][1024], mse_cost[64][1024], total_test[20][64], dct_bent[1024], ica_ent[64][1024], dct_ent[64][1024], dcoe_temp[64][1024] = { 0 }, all_mse[4][1024];
	static unsigned char dammy[256][256] = { 0 };
	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
	static unsigned char  ica_sai[256][256] = { 0 }, ica_sai0[256][256] = { 0 }, ica_sai1[256][256] = { 0 };
	static struct pca pcaStr = { 0 };
	static char image_name[20] = { 0 }, yn;	//画像ファイル名(拡張子含まず)
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
	//printf("+ - - - - - Now Running - - - - +\n");
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
	//for (i = 1; i < 5; i++) {
	//	sprintf(g, "mkdir OUTPUT\\test\\%d", i);
	//	system(g);
	//	for (j = 0; j < 64; j++) {
	//		sprintf(g, "mkdir OUTPUT\\test\\%d\\%d", i, j);
	//		system(g);
	//	}
	//}
	printf("\n\n");
	//_mkdir("coe"); //原画像のブロック画像を拡大して出力
	//	for (j = 0; j < 20; j++) {
	//	sprintf(g, "mkdir coe\\coefficient[%d]", j);
	//	system(g);
	//}
	printf("mkdir end\n");
	//
//読み込むファイル名
	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
	static char filename2[20] = { 'c', 'a', 'm', 'e', 'r', 'a', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
	static char filename3[20] = { 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e', '.', 'b', 'm', 'p' };
	static char filename4[20] = { 'l', 'a', 'x', '.', 'b', 'm', 'p' };
	static char filename5[20] = { 'l', 'e', 'n', 'n', 'a', '.', 'b', 'm', 'p' };
	static char filename6[20] = { 's', 'a', 'i', 'l', 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
	static char filename7[20] = { 'w', 'o', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
	static char filename8[20] = { 'b', 'r', 'i', 'd', 'g', 'e', '.', 'b', 'm', 'p' };
	static char filename9[20] = { 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
	static char filename10[20] = { 'b', 'u', 'i', 'l', 'd', 'i', 'n', 'g', '.', 'b', 'm', 'p' };
	static char filename11[20] = { 'g', 'i', 'r', 'l', '.', 'b', 'm', 'p' };
	static char filename12[20] = { 'l', 'i', 'g', 'h', 't', 'h', 'o', 'u', 's', 'e', '.', 'b', 'm', 'p' };
	static char filename13[20] = { 't', 'e', 'x', 't', '.', 'b', 'm', 'p' };
	static char filename14[20] = { 'e', 'a', 'r', 't', 'h', '.', 'b', 'm', 'p' };
	static char filename15[20] = { 'm', 'a', 'n', 'd', 'r', 'i', 'l', 'l', '.', 'b', 'm', 'p' };

	printf("\nfilename plz .... (1, barbara  2, cameraman  3, mandrill) :");
	scanf("%d", &i);

	if (i == 2)
		strcpy(filename, filename2);
	else if (i == 3)
		strcpy(filename, filename15);


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

	if ((fp3 = fopen("OUTPUT\\fp3.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp4 = fopen("OUTPUT\\fp4.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp5 = fopen("OUTPUT\\fp5.txt", "w")) == NULL) {
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
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
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
	for (i = 0; i < 1024; i++) { // use 1 basis
		result_ica[0][i] = sort_d[0][i].num;  // 基底番号
		result_ica[1][i] = sort_d[0][i].val;    // MSE値
		//printf("%lf : %lf\n", result_ica[0][i], (double)sort_d[0][i].num);
		all_mse[1][i] = sort_d[0][i].val;
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

	// use 0 basis


	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 64; i++) {

			// 該当係数以外0
			// i番目の係数（基底）のみ使用。それ以外の係数は0。
			for (a = 0; a < 64; a++) {
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
			mse_ica0[i][j] = sum / 64;//平均
		}
		// 実行確認用
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	// sort MSE of 0 basis
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			// .val -> 値を取得・属性を変更し記憶
			// .abs -> 絶対値を記憶
			// .num -> 元々の係数に対応するブロック内番号を記憶
			sort_d[i][j].val = mse_ica0[i][j];		/* 元々の係数値 */
			sort_d[i][j].abs = fabs(mse_ica0[i][j]);	/* ソートは係数の絶対値で行う*/
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

	for (i = 0; i < 1024; i++) { // use 0 basis
		result_ica0[0][i] = sort_d[0][i].num;  // 基底番号
		result_ica0[1][i] = sort_d[0][i].val;    // MSE値
		//printf("%lf : %lf\n", result_ica[0][i], (double)sort_d[0][i].num);
		all_mse[0][i] = sort_d[0][i].val;
	}

	///////////////////////

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
		for (j = 0; j < 1024; j++) { // use 2 basis
			dct_coe[i][j] = (double)sort_d[i][j].num;
			dcoe_temp[i][j] = sort_d[i][j].val;
		}
	printf(" end\n\n");
	/////////  1 end

	// 2 start ///////
	//printf("Do you start method 2 ? [ y/n ] : ");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {
		printf("\nMethod 2 start - ->\n\n");

		for (j = 0; j < 1024; j++) {
			for (m = 0; m < 64; m++)
				for (n = 0; n < 64; n++) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;

					ny[m][j] = y[m][j]; //一つ目の基底選択
					ny[n][j] = y[n][j]; // ２つ目の基底選択

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

					ica_test[n][m][j] = sum / 64.0;
				}
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");



		for (l = 0; l < 1024; l++) {
			// 64*64*1024 -> 最小ＭＳＥ　64*1024こ
			for (m = 0; m < 64; m++) {
				for (i = 0; i < 64; i++) {
					// .val -> 値を取得・属性を変更し記憶
					// .abs -> 絶対値を記憶
					// .num -> 元々の係数に対応するブロック内番号を記憶
					sort_d[i][m].val = ica_test[i][m][l];		/* 元々の係数値 */
					sort_d[i][m].abs = fabs(ica_test[i][m][l]);	/* ソートは係数の絶対値で行う*/
					sort_d[i][m].num = i;					/* numに元々の係数に対応する番号を記憶 */
				}
			}

			for (n = 0; n < 64; n++)
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

			for (m = 0; m < 64; m++) {
				ica_test2[0][m][l] = sort_d[0][m].val;//mse
				ica_test2[1][m][l] = (double)sort_d[0][m].num;//基底番号

			}
		}

		for (j = 0; j < 1024; j++) {
			for (i = 0; i < 64; i++) {
				// .val -> 値を取得・属性を変更し記憶
				// .abs -> 絶対値を記憶
				// .num -> 元々の係数に対応するブロック内番号を記憶
				sort_d[i][j].val = ica_test2[0][i][j];		/* 元々の係数値 */
				sort_d[i][j].abs = fabs(ica_test2[0][i][j]);	/* ソートは係数の絶対値で行う*/
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

		for (j = 0; j < 1024; j++) {// 2 basis
			ica_test3[0][j] = sort_d[0][j].val;//mse
			ica_test3[1][j] = (double)sort_d[0][j].num;//基底番号

			all_mse[2][j] = sort_d[0][j].val;

			ica_test4[0][j] = ica_test3[1][j];//1つ目の基底番号
			ica_test4[1][j] = ica_test2[1][(int)ica_test3[1][j]][j];// 2つ目の基底番号
		}

		// 生成画像確認
		for (j = 0; j < 1024; j++) {
			for (i = 0; i < 64; i++)
				ny[i][j] = 0;

			ny[(int)ica_test4[0][j]][j] = y[(int)ica_test4[0][j]][j]; //一つ目の基底選択
			ny[(int)ica_test4[1][j]][j] = y[(int)ica_test4[1][j]][j]; // ２つ目の基底選択
		}
		// 初期化（必ず行う）
		for (a = 0; a < 64; a++)
			xx[a] = 0.0;

		seki5(nw, ny, x); // x -> nw * ny
		xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
		avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = ica_sai[a][b];

		sprintf(output, "OUTPUT/Basis2.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す



		printf("Method 2 end\n\n");
	}
	//		////////////// 2 fin ///////////////////////

		///////////////////////// 3 start ////////////////////////////////
		//printf("Do you use 3 basis ? [ y/n ] : ");
		//scanf("%s", &yn);
	yn = 'n';
	if (yn == 'y') {
		printf("\nMethod 3 basis start - ->\n\n");

		for (j = 0; j < 1024; j++) {
			for (m = 0; m < 64; m++)
				for (n = 0; n < 64; n++)
					for (l = 0; l < 64; l++) {
						for (i = 0; i < 64; i++)
							ny[i][j] = 0;

						ny[m][j] = y[m][j]; // 3つ目の基底選択
						ny[n][j] = y[n][j]; // 2つ目の基底選択
						ny[l][j] = y[l][j]; // 1つ目の基底選択

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

						ica_test5[l][n][m] = sum / 64.0;
					}


			//printf("+ - - - - - Now Running - - - - +\n");

			min = ica_test5[0][0][0];
			for (m = 0; m < 64; m++)
				for (n = 0; n < 64; n++)
					for (l = 0; l < 64; l++) {
						if (ica_test5[l][n][m] < min) {
							min = ica_test5[l][n][m];
							y3[0][j] = (double)l;
							y3[1][j] = (double)m;
							y3[2][j] = (double)n;
						}
					}
			all_mse[3][j] = min;
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		// 生成画像確認
		for (j = 0; j < 1024; j++) {
			for (i = 0; i < 64; i++)
				ny[i][j] = 0;

			ny[(int)y3[0][j]][j] = y[(int)y3[0][j]][j]; //一つ目の基底選択
			ny[(int)y3[1][j]][j] = y[(int)y3[1][j]][j]; // ２つ目の基底選択
			ny[(int)y3[2][j]][j] = y[(int)y3[2][j]][j]; // ２つ目の基底選択
		}
		// 初期化（必ず行う）
		for (a = 0; a < 64; a++)
			xx[a] = 0.0;

		seki5(nw, ny, x); // x -> nw * ny
		xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
		avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = ica_sai[a][b];

		sprintf(output, "OUTPUT/Basis3.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
	}
	///////// 2 fin//////////////////
		//動作確認

	QQ = 0;
	QQQ = 0;
	QQQQ = 0;
	mk = 0;

	// 分類
	for (b = 0; b < 1024; b++) {
		if (fabs(y[(int)result_ica[0][b]][b]) < 1.1 && (result_ica0[1][b] - result_ica[1][b]) > 10.0 && (result_ica[1][b] - dcoe_temp[0][b]) < 100.0) {
			temp1[(int)result_ica[0][b]]++;
			QQ++;
			count_temp[0][b] = 1;
		}
		else if (1.1 <= fabs(y[(int)result_ica[0][b]][b]) && fabs(y[(int)result_ica[0][b]][b]) < 2 && (result_ica0[1][b] - result_ica[1][b]) > 10.0 && (result_ica[1][b] - dcoe_temp[0][b]) < 100.0) {
			temp2[(int)result_ica[0][b]]++;
			QQQ++;
			count_temp[0][b] = 2;
		}
		else if (fabs(y[(int)result_ica[0][b]][b]) >= 2 && (result_ica0[1][b] - result_ica[1][b]) > 10.0 && (result_ica[1][b] - dcoe_temp[0][b]) < 100.0) {
			temp3[(int)result_ica[0][b]]++;
			QQQQ++;
			count_temp[0][b] = 3;
		}
		else if ((result_ica[1][b] - dcoe_temp[0][b]) > 100.0) {
			temp5[(int)result_ica[0][b]]++;
			mk++;
			count_temp[0][b] = 4;
		}
		else
			count_temp[0][b] = 0;
	}

	// 0->1改善量
	for (j = 0; j < 1024; j++)
		for (i = 0; i < 64; i++)
			test2[i][j] = (mse_ica0[i][j] - mse_ica[i][j]);

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			// .val -> 値を取得・属性を変更し記憶
			// .abs -> 絶対値を記憶
			// .num -> 元々の係数に対応するブロック内番号を記憶
			sort_d[i][j].val = test2[i][j];		/* 元々の係数値 */
			sort_d[i][j].abs = test2[i][j];	/* ソートは係数の絶対値で行う*/
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
		for (j = 0; j < 1024; j++) {// 0->1の改善量
			test2[i][j] = sort_d[i][j].val; //改善量（mse）
			test3[i][j] = sort_d[i][j].num; //基底番号
		}

	// 置換可能基底
	printf(" ~ Investigation of replaceable basis ~\n What percentage do you use ? : ");
	scanf("%lf", &percent);


	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++)
			if (0 < test2[0][j] - test2[i][j] && test2[0][j] - test2[i][j] <= percent && test2[i][j] >= 0) {
				ica_test1[(int)test3[i][j]][(int)test3[0][j]]++;
				temp_sai2[(int)test3[i][j]][j]++;
			}

	// 置換可能領域を画像で確認
	for (i = 0; i < 64; i++) {
		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (temp_sai2[i][j] == 0)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/ORIGIN/%d[%d].bmp", (int)percent, i);
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
	}

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai3[a][b] = 0;

	for (a = 0; a < 64; a++)
		for (b = a; b < 64; b++)
			for (j = 0; j < 1024; j++)
				if (temp_sai2[a][j] == 1 && temp_sai2[b][j] == 1)
					temp_sai3[a][b]++;
		
			for (j = 0; j < 1024; j++)
				y_rank_pm[(int)test3[0][j]]++;
			/// ////////////////////////////////////////////////////////////////////////////////////////////

			fprintf(fp2, "\n\n Use image  :  %s\n\n\n", filename);
			fprintf(fp2, "\n\n  MSE value for the entire image \n\n\n  Number of basis used : 0, 1, 2, 3\n\n\n  (* The number of basis is not the whole number, but the number using the best basis in a small area )\n\n----------------------------------------------------------------------------------\n\n");

			sum = 0;
			for (j = 0; j < 1024; j++)
				sum += all_mse[0][j];
			fprintf(fp2, "\n\n- - - - - - - - Number of basis used : 0 - - - - - - - \n\n\n");
			fprintf(fp2, " MSE : %lf  (value)\n\n", sum / 1024.0);

			sum = 0;
			for (j = 0; j < 1024; j++)
				sum += all_mse[1][j];
			fprintf(fp2, "\n\n- - - - - - - - Number of basis used : 1 - - - - - - - \n\n\n");
			fprintf(fp2, " MSE : %lf  (value)\n\n", sum / 1024.0);

			sum = 0;
			for (j = 0; j < 1024; j++)
				sum += all_mse[2][j];
			fprintf(fp2, "\n\n- - - - - - - - Number of basis used : 2 - - - - - - - \n\n\n");
			fprintf(fp2, " MSE : %lf  (value)\n\n", sum / 1024.0);

			sum = 0;
			for (j = 0; j < 1024; j++)
				sum += all_mse[3][j];
			fprintf(fp2, "\n\n- - - - - - - - Number of basis used : 3 - - - - - - - \n\n\n");
			fprintf(fp2, " MSE : %lf  (value)\n\n", sum / 1024.0);

		

		for (i = 0; i < 64; i++)
			for (j = 0; j < 1024; j++) {
				if (mse_ica0[i][j] - mse_ica[i][j] > 0) {
					total_mse[0][i] += mse_ica0[i][j] - mse_ica[i][j]; // 0 -> 1の時の各基底のMSE改善・損失量
					total_mse[2][i]++;
				}
				else
					total_mse[1][i] += mse_ica0[i][j] - mse_ica[i][j];
			}

		
		sum = 0; sum1 = 0; //sum1 -> マイナス




		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);



		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);
		printf("\n");
		fprintf(fp2, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp2, "\n\n  Percentage of improvement and loss of each basis to area \n\n\n  Number of basis used : 0, 1 \n\n\n  [basis number]  :  Percentage ( improvement or loss )  \n\n\n  Persentage threshold : %lf \n\n----------------------------------------------------------------------------------\n\n", percent);

		/*for (j = 0; j < 1024; j++) {
			fprintf(fp2, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
			for (i = 0; i < 64; i++) {
				if (test2[i][j] > 0)
					total_test[2][i]++;
				else
					total_test[3][i]++;

				if (percent < fabs(test2[i][j]) && fabs(test2[i][j]) < percent + 10) {
					fprintf(fp2, " [%2d] : %3.10lf  ( Percentage )\n\n", i, test2[i][j]);
					if (test2[i][j] > 0)
						total_test[0][i]++;
					else
						total_test[1][i]++;
				}
			}
		}

		fprintf(fp2, "\n\n -------------- Basis usage count ---------------------- \n\n\n");
		for (i = 0; i < 64; i++) {
				fprintf(fp2, " [%2d] : %3d / %3d  %lf  ( improvement )   %3d / %3d  %lf  ( loss )\n\n", i, (int)total_test[0][i], (int)total_test[2][i], total_test[0][i] / total_test[2][i] * 100,(int)total_test[1][i], (int)total_test[3][i], total_test[1][i] / total_test[3][i] * 100);
		}*/

		for (b = 0; b < 64; b++) {
			temp1[b] = 0;
			temp2[b] = 0;
			temp3[b] = 0;
			temp4[b] = 0;
			temp5[b] = 0;
		}


		

		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);
		printf("\n");

		fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp5, "\n\n  Relationship between basis and replaceable basis\n\n\n  Number of basis used : 0, 1 \n\n\n  [basis number] : Number of substitutable basis for basis number \n\n\n *Total on the right : Number of replacements (Be done)\n\n  Total below : Number of replacements (To do)\n\n\n  Of the amount of improvement  : 0 ~ %lf\n\n----------------------------------------------------------------------------------\n\n", percent);


		fprintf(fp5, " +----+");
		for (i = 0; i < 65; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n | ## |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "[%2d]|", i);
		fprintf(fp5, " ## |                                                                                    ");

		for (j = 0; j < 64; j++) {
			sum = 0;
			fprintf(fp5, "\n +----+");
			for (i = 0; i < 65; i++)
				fprintf(fp5, "----+");
			fprintf(fp5, "\n |[%2d]|", j);
			for (i = 0; i < 64; i++) {
				if (j == i)
					fprintf(fp5, " ## |");
				else{
					if (i <= j)
						fprintf(fp5, "%4d|", (int)temp_sai3[i][j]);
					else
						fprintf(fp5, "    |");
				}
			}
			fprintf(fp5, "[%2d]|", j);
		}
		fprintf(fp5, "\n +----+");
		for (i = 0; i < 65; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n | ## |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "[%2d]|", i);
		fprintf(fp5, " ## |");

		fprintf(fp5, "\n +----+");
		for (i = 0; i < 65; i++)
			fprintf(fp5, "----+");


		fprintf(fp5, "\n\n\n\n\n\n\n");
		////fprintf(fp5, "\n\n- - - - - - - - Magnitude of coefficient ~ 1.1 - - - - - - - \n\n\n");
		//for (b = 0; b < 64; b++) {
		//	//fprintf(fp5, "[%2d] : %d   [%lf%]\n\n", b,temp1[b], ((double)temp1[b]/(double)QQ )*100);
		//	if (((double)temp1[b] / (double)QQ) * 100 > percent) {
		//		temp4[b]++;
		//		test_per[0][b]++;
		//	}
		//}
		//fprintf(fp, "\n\n- - - - - - - - Magnitude of coefficient ~ 1.1 - - - - - - - \n\n\n");
		////fprintf(fp5, "\n- - - Commonly used basis (over %lf percent) - - - \n\n\n",percent);
		//for (b = 0; b < 64; b++) {
		//	if (temp4[b] == 1) {
		//		fprintf(fp, "[%2d] : %lf       ", b, ((double)temp1[b] / (double)QQ) * 100);
		//		//fprintf(fp5, "%2d : %lf\n\n", b, ((double)temp1[b] / (double)QQ) * 100);
		//	}
		//	temp4[b] = 0;
		//}
		//fprintf(fp, "\n\n");

		////fprintf(fp5, "\n\n- - - - - - - - Magnitude of coefficient 1.1 ~ 2 - - - - - - - \n\n\n");
		//for (b = 0; b < 64; b++) {
		//	//fprintf(fp5, "[%2d] : %d   [%lf%]\n\n", b, temp2[b], ((double)temp2[b] / (double)QQQ) * 100);
		//	if (((double)temp2[b] / (double)QQQ) * 100 > percent) {
		//		temp4[b]++;
		//		test_per[1][b]++;
		//	}
		//}
		//fprintf(fp, "\n\n- - - - - - - - Magnitude of coefficient 1.1 ~ 2 - - - - - - - \n\n\n");
		////fprintf(fp5, "\n- - - Commonly used basis (over %lf percent)- - - \n\n\n",percent);
		//for (b = 0; b < 64; b++) {
		//	if (temp4[b] == 1) {
		//		fprintf(fp, "[%2d] : %lf       ", b, ((double)temp2[b] / (double)QQQ) * 100);
		//		//fprintf(fp5, "%2d : %lf\n\n", b, ((double)temp2[b] / (double)QQQ) * 100);
		//	}
		//	temp4[b] = 0;
		//}
		//fprintf(fp, "\n\n");

		////fprintf(fp5, "\n\n- - - - - - - - Magnitude of coefficient 2 ~ - - - - - - - \n\n\n");
		//for (b = 0; b < 64; b++) {
		//	//fprintf(fp5, "[%2d] : %d   [%lf%]\n\n", b, temp3[b], ((double)temp3[b] / (double)QQQQ) * 100);
		//	if (((double)temp3[b] / (double)QQQQ) * 100 > percent) {
		//		temp4[b]++;
		//		test_per[2][b]++;
		//	}
		//}
		//fprintf(fp, "\n\n- - - - - - - - Magnitude of coefficient 2 ~ - - - - - - - \n\n\n");
		////fprintf(fp5, "\n- - - Commonly used basis (over %lf percent) - - - \n\n\n",percent);
		//for (b = 0; b < 64; b++) {
		//	if (temp4[b] == 1) {
		//		fprintf(fp, "[%2d] : %lf       ", b, ((double)temp3[b] / (double)QQQQ) * 100);
		//		//fprintf(fp5, "%2d : %lf\n\n", b, ((double)temp3[b] / (double)QQQQ) * 100);
		//	}
		//	temp4[b] = 0;
		//}
		//fprintf(fp, "\n\n");

		////fprintf(fp5, "\n\n- - - - - - - - Use 2 basis - - - - - - - \n\n\n");
		//for (b = 0; b < 64; b++) {
		//	//fprintf(fp5, "[%2d] : %d   [%lf%]\n\n", b, temp5[b], ((double)temp5[b] / (double)mk) * 100);
		//	if (((double)temp5[b] / (double)mk) * 100 > percent) {
		//		temp4[b]++;
		//		test_per[3][b]++;
		//	}
		//}
		//fprintf(fp, "\n\n- - - - - - - - Use 2 basis - - - - - - - \n\n\n");
		////fprintf(fp5, "\n- - - Commonly used basis (over %lf percent)- - - \n\n\n",percent);
		//for (b = 0; b < 64; b++) {
		//	if (temp4[b] == 1) {
		//		fprintf(fp, "[%2d] : %lf       ", b, ((double)temp5[b] / (double)mk) * 100);
		//		//fprintf(fp5, "%2d : %lf\n\n", b, ((double)temp5[b] / (double)mk) * 100);
		//	}
		//	temp4[b] = 0;
		//}
		//fprintf(fp, "\n\n\n\n");

		//for (j = 0; j < 1024; j++) {
		//	for (i = 0; i < 64; i++) {
		//		//if (test2[i][j] > 0)
		//		//	if (count_temp[0][j] == 0)
		//		//		total_test[0][i]++;
		//		//	else if(count_temp[0][j] == 1)
		//		//		total_test[4][i]++;
		//		//	else if (count_temp[0][j] == 2)
		//		//		total_test[8][i]++;
		//		//	else if (count_temp[0][j] == 3)
		//		//		total_test[12][i]++;
		//		//	else if (count_temp[0][j] == 4)
		//		//		total_test[16][i]++;
		//		//else
		//		//		if (count_temp[0][j] == 0)
		//		//			total_test[1][i]++;
		//		//		else if (count_temp[0][j] == 1)
		//		//			total_test[5][i]++;
		//		//		else if (count_temp[0][j] == 2)
		//		//			total_test[9][i]++;
		//		//		else if (count_temp[0][j] == 3)
		//		//			total_test[13][i]++;
		//		//		else if (count_temp[0][j] == 4)
		//		//			total_test[17][i]++;

		//		if (percent < fabs(test2[i][j]) && fabs(test2[i][j]) < percent + 10) {
		//			if (test2[i][j] >= 0) {
		//				if (count_temp[0][j] == 0)
		//					total_test[0][i]++;
		//				else if (count_temp[0][j] == 1)
		//					total_test[2][i]++;
		//				else if (count_temp[0][j] == 2)
		//					total_test[4][i]++;
		//				else if (count_temp[0][j] == 3)
		//					total_test[6][i]++;
		//				else if (count_temp[0][j] == 4)
		//					total_test[8][i]++;
		//				total_test[14][i]++;
		//			}

		//			if (test2[i][j] < 0){
		//				if (count_temp[0][j] == 0)
		//					total_test[1][i]++;
		//				else if (count_temp[0][j] == 1)
		//					total_test[3][i]++;
		//				else if (count_temp[0][j] == 2)
		//					total_test[5][i]++;
		//				else if (count_temp[0][j] == 3)
		//					total_test[7][i]++;
		//				else if (count_temp[0][j] == 4)
		//					total_test[9][i]++;
		//				total_test[15][i]++;
		//			}

		//		}
		//	}
		//}


		//fprintf(fp, "\n               0 basis                    ~ 1.1                   1.1 ~ 2.0                 2.0 ~                   2 basis                total");
		//fprintf(fp, "\n +----+-------------------------+------------------------+------------------------+------------------------+------------------------+------------------+----+\n");
		//for (b = 0; b < 64; b++) {
		//	fprintf(fp, " | %2d | ", b);
		//	for (i = 0; i < 5; i++) {
		//		if (total_test[i * 2][b] == 0)
		//			fprintf(fp, "           ");
		//		else
		//			fprintf(fp, " %3d (%3d%%)", (int)total_test[i * 2][b], (int)((total_test[i*2][b]/total_test[14][b])*100));

		//		if (total_test[i * 2+1][b] == 0)
		//			fprintf(fp, " :           |"); 
		//		else
		//			fprintf(fp, " : %3d (%3d%%)|", (int)total_test[i * 2 + 1][b], (int)((total_test[i * 2+1][b] / total_test[15][b])* 100));
		//	}
		//	fprintf(fp, "  %3d   :  %3d    |", (int)total_test[14][b], (int)total_test[15][b]);
		//	fprintf(fp, " %2d | ", b);
		//	fprintf(fp, "\n +----+-------------------------+------------------------+------------------------+------------------------+------------------------+------------------+----+\n");
		//}
		//

		//for (b = 0; b < 1024; b++)
		//	cost_ica[b] = result_ica[1][b] - ica_test3[0][b];

		//gnuplot3(cost_ica, cost_dct);

		//fprintf(fp, "\n\n\n");
		//for (b = 0; b < 1024; b++) {
		//	fprintf(fp, "%2d ",count_temp[0][b]);
		//	if (b % 32 == 0 && b != 0)
		//		fprintf(fp, "\n");
		//}


		fprintf(fp, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp, "\n\n  Core basis investigation \n\n\n  Number of basis used : 0, 1 \n\n\n  MSE difference used : %lf\n\n----------------------------------------------------------------------------------\n\n", percent);

		for (j = 0; j < 1024; j++) {
			fprintf(fp, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
			for (i = 0; i < 64; i++) {
				if (test2[0][j] - test2[i][j] <= percent && test2[i][j] >= 0) {
					if (count_temp[0][j] == 0) {
						total_test[0][(int)test3[i][j]]++;
						//total_test[10][0]++;
					}
					else if (count_temp[0][j] == 1) {
						total_test[1][(int)test3[i][j]]++;
						//total_test[10][1]++;
					}
					else if (count_temp[0][j] == 2) {
						total_test[2][(int)test3[i][j]]++;
						//total_test[10][2]++;
					}
					else if (count_temp[0][j] == 3) {
						total_test[3][(int)test3[i][j]]++;
						//total_test[10][3]++;
					}
					else if (count_temp[0][j] == 4) {
						total_test[4][(int)test3[i][j]]++;
						//total_test[10][4]++;
					}
					if (count_temp[0][j] != 0)
						fprintf(fp, " [%2d] : %4.5lf  ( MSE Value )\n\n", (int)test3[i][j], mse_ica[(int)test3[i][j]][j]);

				}
			}
			if (count_temp[0][j] == 0) {
				//total_test[0][(int)test3[i][j]]++;
				total_test[10][0]++;
			}
			else if (count_temp[0][j] == 1) {
				//total_test[1][(int)test3[i][j]]++;
				total_test[10][1]++;
			}
			else if (count_temp[0][j] == 2) {
				//total_test[2][(int)test3[i][j]]++;
				total_test[10][2]++;
			}
			else if (count_temp[0][j] == 3) {
				//total_test[3][(int)test3[i][j]]++;
				total_test[10][3]++;
			}
			else if (count_temp[0][j] == 4) {
				//total_test[4][(int)test3[i][j]]++;
				total_test[10][4]++;
			}
		}

		fprintf(fp, "\n\n\n\n\n\n");

		fprintf(fp, "\n               0 basis                  ~ 1.1                1.1 ~ 2.0               2.0 ~                2 basis             ");
		fprintf(fp, "\n +----+-----------------------+----------------------+----------------------+----------------------+----------------------+----+\n");
		for (b = 0; b < 64; b++) {
			fprintf(fp, " | %2d | ", b);
			for (i = 0; i < 5; i++) {
				fprintf(fp, "      %3d / %3d (%3d%%)|", (int)total_test[i][b], (int)total_test[10][i], (int)((total_test[i][b] / total_test[10][i]) * 100));
			}
			fprintf(fp, " %2d | ", b);
			fprintf(fp, "\n +----+-----------------------+----------------------+----------------------+----------------------+----------------------+----+\n");

		}
	printf("<ica fin>\n\n");
	/////////////////////////////////ica 終了/////////////////////////////////////////


	// //////////////////////////// dct ////////////////////////////////////////
	// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

	//動作確認
	printf("<dct start>\n\n");
	//printf("Do you want to run the DCT ? [ y/n ] : ");
	//scanf("%s", &yn);

	if (yn == 'y') {
		fprintf(fp2, "\n\n\n- - - - - - - - - - - - - - - - ( Reference ) For DCT - - - - - - - - - - - - - - - \n\n\n");
		// 10段階品質があるから10段階分やる
		for (Q = 100; Q > 0; Q -= 10) {
			printf("\r now Q is %d          \n", Q);


			//Q = 100;

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			idct(dcoe, dcoe2, 8); // 普通の再構成

			sum = 0;

			for (a = 0; a < 256; a++) {
				for (b = 0; b < 256; b++) {
					sum += pow((double)origin[a][b] - (double)dcoe2[a][b], 2);
				}
			}

			sum = sum / (256.0 * 256.0);


			fprintf(fp2, " Q = %3d : %lf  (MSE value)\n\n", Q, sum);

			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = dcoe2[a][b];

			sprintf(output, "OUTPUT/DCT%d.bmp", Q);
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

			//for (i = 0; i < 64; i++) {

			//	// 該当係数以外0
			//	// i番目の係数（基底）のみ使用。それ以外の係数は0。
			//	for (j = 0; j < 1024; j++) {
			//		for (a = 0; a < 64; a++) {
			//			if (i == a)
			//				ny[a][j] = dct_ent[a][j];
			//			else
			//				ny[a][j] = 0;
			//		}
			//	}
			//	xtogen2(ny, dcoe);
			//	idct(dcoe, dcoe2, 8); // 普通の再構成

			//	for (j = 0; j < 1024; j++) {

			//		// ブロックごとのMSE
			//		// MSEは（元の値 - 再構成の値）^2をすることで
			//		// 再構成した値が元の値とどれくらいずれているのかを見るための指標
			//		sum = 0.0;
			//		sum2 = 0.0;
			//		mk = j % 32;
			//		ml = j / 32;

			//		// 64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
			//		// （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
			//		for (a = 0; a < 8; a++) {
			//			for (b = 0; b < 8; b++) {
			//				sum += pow(origin[ml * 8 + b][mk * 8 + a] - dcoe2[ml * 8 + b][mk * 8 + a], 2);
			//			}
			//		}
			//		mse_dct[i][j] = sum / 64;//平均
			//	}
			//	// 実行確認用
			//	



			//	//	sprintf(output, "test.bmp");
			//	//	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
			//	//}

			//}

			//	for (i = 0; i < 1024; i++)
			//		for (j = 0; j < 64; j++)
			//		mse_dct[i][j]

			/////////////////////////////////////////////////////////
			//for (b = 0; b < 1024; b++) {
			//	fprintf(fp2, "\n --- [%d] --- \n\n", b);
			//	for (a = 0; a < 64; a++) {
			//		fprintf(fp2, " [%d][%d]  -->  %lf\n", a, b, mse_dct[a][b]);
			//	}
			//}


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

			//// dct係数をソート
			//for (i = 0; i < 64; i++) {
			//	for (j = 0; j < 1024; j++) {
			//		// .val -> 値を取得・属性を変更し記憶
			//		// .abs -> 絶対値を記憶
			//		// .num -> 元々の係数に対応するブロック内番号を記憶
			//		sort_d[i][j].val = dct_ent[i][j];		/* 元々の係数値 */
			//		sort_d[i][j].abs = fabs(dct_ent[i][j]);	/* ソートは係数の絶対値で行う*/
			//		sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
			//	}
			//}

			//for (n = 0; n < 1024; n++) {
			//	for (i = 0; i < 64 - 1; i++) {
			//		max = sort_d[i][n].abs;
			//		k = i;
			//		for (j = i + 1; j < 64; j++) {
			//			if (sort_d[j][n].abs > max) {
			//				max = sort_d[j][n].abs;
			//				k = j;
			//			}
			//		}
			//		temp = sort_d[i][n];
			//		sort_d[i][n] = sort_d[k][n];
			//		sort_d[k][n] = temp;
			//	}
			//}
			//for (i = 0; i < 64; i++)
			//	for (j = 0; j < 1024; j++) {
			//		dct_coe[i][j] = (double)sort_d[i][j].num;//dct_coe -> ソート後のdct係数順位
			//		dcoe_temp[i][j] = sort_d[i][j].val;
			//	}
			//////////////////////////////

			//		// dctのMSEをソート
			//for (i = 0; i < 64; i++) {
			//	for (j = 0; j < 1024; j++) {
			//		// .val -> 値を取得・属性を変更し記憶
			//		// .abs -> 絶対値を記憶
			//		// .num -> 元々の係数に対応するブロック内番号を記憶
			//		sort_d[i][j].val = mse_dct[i][j];		/* 元々の係数値 */
			//		sort_d[i][j].abs = fabs(mse_dct[i][j]);	/* ソートは係数の絶対値で行う*/
			//		sort_d[i][j].num = i;					/* numに元々の係数に対応する番号を記憶 */
			//	}
			//}

			//for (n = 0; n < 1024; n++) {
			//	for (i = 0; i < 64 - 1; i++) {
			//		max = sort_d[i][n].abs;
			//		k = i;
			//		for (j = i + 1; j < 64; j++) {
			//			if (sort_d[j][n].abs > max) {
			//				max = sort_d[j][n].abs;
			//				k = j;
			//			}
			//		}
			//		temp = sort_d[i][n];
			//		sort_d[i][n] = sort_d[k][n];
			//		sort_d[k][n] = temp;
			//	}
			//}

			//for (i = 0; i < 64; i++)
			//	for (j = 0; j < 1024; j++)
			//		mse_dct[i][j] = sort_d[i][j].val;//dct_coe -> ソート後のmse

			/// //////////////////////////////


			//　最小ＭＳＥの係数順位のグラフ出力


			////////////////出力///////////////////

			//////////////////出力終了///////////////////////
		} // dctの最初に戻る
		printf("\r [ Execution finished ]          ");
		printf("\n\n");
	}
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
