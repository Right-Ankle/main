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
	FILE* fp, * fp2, * fp3, * fp4, * fp5, *fp6; //ファイルポインタの定義

	//宣言
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
	//static  double ori_temp2[64][1024] = { 0 };
	static int i, j, n, m, k, l, mk, ml, Q, QQ, QQQ, QQQQ, b, a, c, d, out_count = 0, y_rank[64][1024], seg[64 * 64], y_rank_pm[64], seg0[64 * 64], seg1[64 * 64], ori_temp[256 * 256], temp_sai[256 * 256], temp_sai11[256 * 256], temp_sai22[256 * 256], temp_sai2[64][1024], temp_sai3[256][256], ica[64], temp1[64], temp2[64], temp3[64], temp4[64], temp5[64], temp6[64], count_temp[4][1024], semi[2][64], no_op[1024];
	static double percent, sum, sum0, sum1, sum11, sum22, best_ica[1024], sum2, min, max, mse_dct[2][10][1024], mse_dct2[1024], mse_ica[64][1024], mse_ica0[64][1024], mse_ica1[64][1024], cost_ica[1024], cost_dct[1024], total_mse[3][64], result_dct[2][1024], result_ica[2][1024], result_ica0[2][1024], result_mse[64][1024], y3[3][1024], imp[10][1024], imp_rate[7][1024], full_mse[2][64][1024];
	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, dcoe[256][256] = { 0 }, dct_coe_temp[64][1024], test[5][1024], test2[64][1024], test3[64][1024], ica_test[64][64][1024], ica_test2[2][64][1024], ica_test3[2][1024], ica_test4[2][1024], ica_test5[64][64][64], ica_test0[64][1024], ica_test1[64][64], average2[1024], test_per[4][64], mse100[64][1024];
	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], total_test[20][64], dct_bent[1024], dct_ent[64][1024], dcoe_temp[64][1024] = { 0 }, all_mse[4][1024], bunrui[4];
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
	//	for (j = 0; j < 1024; j++) {
	//	sprintf(g, "mkdir OUTPUT\\ICA\\%d", j);
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

	if ((fp3 = fopen("OUTPUT\\fp3.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp4 = fopen("OUTPUT\\fp4.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp5 = fopen("OUTPUT\\fp5.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp6 = fopen("OUTPUT\\fp6.txt", "w")) == NULL) {
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

			//for (b = 0; b < 8; b++)
			//	for (a = 0; a < 8; a++)
			//		for (l = 0; l < 8; l++)
			//			for (m = 0; m < 8; m++)
			//				seg[64 * 8 * b + 8 * a + l * 64 + m] = block_ica[b * 8 + a];

			//sprintf(output, "OUTPUT/ICA/1/[%d].bmp", j);
			//img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
			

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
	// MSE順ソート後
	for (i = 0; i < 1024; i++) { // use 1 basis ///////////////////////////////////////////////////////////////////////////
		result_ica[0][i] = sort_d[0][i].num;  // 基底番号
		result_ica[1][i] = sort_d[0][i].val;    // MSE値
		//printf("%lf : %lf\n", result_ica[0][i], (double)sort_d[0][i].num);
		all_mse[1][i] = sort_d[0][i].val;

		for (j = 0; j < 64; j++)
			result_mse[j][i] = sort_d[j][i].val;

	}

	//gnuplot(result_mse);

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

	// 生成画像確認
	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 64; i++)
			ny[i][j] = 0;

		ny[(int)dct_coe[0][j]][j] = y[(int)dct_coe[0][j]][j]; //一つ目の基底選択
		ny[(int)dct_coe[1][j]][j] = y[(int)dct_coe[1][j]][j]; // ２つ目の基底選択
	}
	//// 初期化（必ず行う）
	//for (a = 0; a < 64; a++)
	//	xx[a] = 0.0;

	//seki5(nw, ny, x); // x -> nw * ny
	//xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	//avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	//for (a = 0; a < 256; a++)
	//	for (b = 0; b < 256; b++)
	//		temp_sai[a * 256 + b] = ica_sai[a][b];

	//sprintf(output, "OUTPUT/Basis2.bmp");
	//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	//for (a = 0; a < 256; a++)
	//	for (b = 0; b < 256; b++)
	//		temp_sai[a * 256 + b] = ica_sai[a][b];

	//for (i = 0; i < 1024; i++) {

	//	mk = i % 32;
	//	ml = i / 32;
	//	for (b = 0; b < 8; b++)
	//		for (a = 0; a < 8; a++)
	//			for (l = 0; l < 8; l++)
	//				for (m = 0; m < 8; m++)
	//					seg[64 * 8 * b + 8 * a + l * 64 + m] = temp_sai[256 * 8 * ml + 8 * mk + a + 256 * b];

	//	sprintf(output, "OUTPUT/ICA/2/[%d].bmp", i);
	//	img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
	//}

	printf(" end\n\n");
	/////////   end

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
	//printf(" ~ Investigation of replaceable basis ~\n What percentage do you use ? : ");
	//scanf("%lf", &percent);


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
			if (test3[0][j] != i)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/ORIGIN/[%d].bmp", i);
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
	}

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai3[a][b] = 0;

	for (a = 0; a < 64; a++)
		for (b = 0; b < 64; b++)
			for (j = 0; j < 1024; j++)
				if (temp_sai2[a][j] == 1 && temp_sai2[b][j] == 1)
					temp_sai3[a][b]++;
		
			for (j = 0; j < 1024; j++)
				y_rank_pm[(int)test3[0][j]]++;
			/// ////////////////////////////////////////////////////////////////////////////////////////////


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


		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);
		// 改善量調査　基底数1を格納
		percent = 500;
		for (j = 0; j < 1024; j++) {
			for (i = 0; i < 6; i++)
				imp_rate[i][j] = 0;
			imp[0][j] = 1.0; //今後の計算の有無
			imp[1][j] = result_ica[1][j]; //今のMSE値
			imp[2][j] = result_ica0[1][j] - result_ica[1][j]; //一段階前とのMSE改善量
			imp[3][j] += imp[2][j];
			if (imp[1][j] <= percent)
				imp[0][j] = 0;
			//imp[3][j] += imp[2][j]; //今の累計のMSE改善量
			imp[5][j] = result_ica[0][j];
			if (imp[0][j] == 1)
				imp_rate[1][j] = imp[2][j] * 100 / result_ica0[1][j];
		}
		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);

		///////////////////////////////////////////////////////////// /////////////////  2basis
		printf("\n");
		fprintf(fp, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp, "\n\n  Area with 500 or more MSE \n\n\n  Number of basis used : 1, 2 \n\n\n  Area MSE value and basis improvement ( comparison and cumulative total with one step before ) \n\n----------------------------------------------------------------------------------\n\n");
		QQ = 0;
		for (j = 0; j < 1024; j++) {
			imp[4][j]=100000;
			if (imp[0][j] == 1) {
				QQ++;
				for (n = 0; n < 64; n++) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;

					ny[(int)imp[5][j]][j] = y[(int)imp[5][j]][j]; //一つ目の基底選択
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
					if (imp[4][j] > sum / 64.0) {
						imp[4][j] = sum / 64.0;
						imp[6][j] = n;
					}
				}
				imp[2][j] = imp[1][j] - imp[4][j];
				imp[3][j] += imp[2][j];
				imp[1][j] = imp[4][j];
				if (imp[0][j] == 1)
					imp_rate[2][j] = imp[3][j] * 100 / result_ica0[1][j];
			}
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (imp[0][j] != 1)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/Basis2.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		fprintf(fp, " +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp, "----+");

		fprintf(fp, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp, "[%2d]|", i+1);
		fprintf(fp, " ## |                                                                                    ");

		for (j = 0; j < 32; j++) {

			fprintf(fp, "\n +----+");
			for (i = 0; i < 33; i++)
				fprintf(fp, "----+");
			fprintf(fp, "\n |[%2d]|", j+1);
			for (i = 0; i < 32; i++) {
				if (imp[0][i + 32 * j] == 1)
					fprintf(fp, "%4d|", (int)imp[1][i + 32 * j]);
				else
					fprintf(fp, "    |");
			}
			fprintf(fp, "[%2d]|", j+1);
		}
		fprintf(fp, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp, "----+");

		fprintf(fp, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp, "[%2d]|", i+1);
		fprintf(fp, " ## |                                                                                    ");

		fprintf(fp, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp, "----+");

		fprintf(fp, "\n\n\n  count : %4d / 1024\n\n", QQ);

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			fprintf(fp, "\n\n -------------------- [ area No.%d (%2d , %2d)] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j, ml + 1, mk + 1);
			if (imp[0][j] == 1) {
				fprintf(fp, "   MSE Value : %lf  ->  %lf", result_ica[1][j], imp[1][j]);
				fprintf(fp, "\n\n   Amount of improvement from basis 1 : %lf", imp[2][j]);
				fprintf(fp, "\n\n   Cumulative amount of improvement from basis 1  [ improvement rate ] : %lf  [ %lf ]", imp[3][j], (imp[3][j] / result_ica0[1][j])*100);

			}
			if (imp[1][j] <= percent)
				imp[0][j] = 0;
		}


		///////////////////////////////////////////////////////////// /////////////////  3basis
		printf("\n");
		fprintf(fp2, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp2, "\n\n  Area with 500 or more MSE \n\n\n  Number of basis used : 2, 3 \n\n\n  Area MSE value and basis improvement ( comparison and cumulative total with one step before ) \n\n----------------------------------------------------------------------------------\n\n");
		QQ = 0;
		for (j = 0; j < 1024; j++) {
			imp[4][j] = 100000;
			if (imp[0][j] == 1) {
				QQ++;
				for (n = 0; n < 64; n++) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;

					ny[(int)imp[5][j]][j] = y[(int)imp[5][j]][j]; //1つ目の基底選択
					ny[(int)imp[6][j]][j] = y[(int)imp[6][j]][j]; //2つ目の基底選択
					ny[n][j] = y[n][j]; // 3つ目の基底選択

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
					if (imp[4][j] > sum / 64.0) {
						imp[4][j] = sum / 64.0;
						imp[7][j] = n;
					}
				}
				imp[2][j] = imp[1][j] - imp[4][j];
				imp[3][j] += imp[2][j];
				imp[1][j] = imp[4][j];
				if (imp[0][j] == 1)
					imp_rate[3][j] = imp[3][j] * 100 / result_ica0[1][j];
			}
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (imp[0][j] != 1)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/Basis3.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		fprintf(fp2, " +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp2, "----+");

		fprintf(fp2, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp2, "[%2d]|", i + 1);
		fprintf(fp2, " ## |                                                                                    ");

		for (j = 0; j < 32; j++) {

			fprintf(fp2, "\n +----+");
			for (i = 0; i < 33; i++)
				fprintf(fp2, "----+");
			fprintf(fp2, "\n |[%2d]|", j + 1);
			for (i = 0; i < 32; i++) {
				if (imp[0][i + 32 * j] == 1)
					fprintf(fp2, "%4d|", (int)imp[1][i + 32 * j]);
				else
					fprintf(fp2, "    |");
			}
			fprintf(fp2, "[%2d]|", j + 1);
		}
		fprintf(fp2, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp2, "----+");

		fprintf(fp2, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp2, "[%2d]|", i + 1);
		fprintf(fp2, " ## |                                                                                    ");

		fprintf(fp2, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp2, "----+");

		fprintf(fp2, "\n\n\n  count : %4d / 1024\n\n", QQ);

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			fprintf(fp2, "\n\n -------------------- [ area No.%d (%2d , %2d)] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j, ml + 1, mk + 1);
			if (imp[0][j] == 1) {
				fprintf(fp2, "   MSE Value : %lf  ->  %lf", result_ica[1][j], imp[1][j]);
				fprintf(fp2, "\n\n   Amount of improvement from basis 1 : %lf", imp[2][j]);
				fprintf(fp2, "\n\n   Cumulative amount of improvement from basis 1 [ improvement rate ] : %lf  [ %lf ]  ", imp[3][j], (imp[3][j] / result_ica0[1][j]) * 100);

			}
			if (imp[1][j] <= percent)
				imp[0][j] = 0;
		}

		///////////////////////////////////////////////////////////// /////////////////  4basis
		printf("\n");
		fprintf(fp3, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp3, "\n\n  Area with 500 or more MSE \n\n\n  Number of basis used : 3, 4 \n\n\n  Area MSE value and basis improvement ( comparison and cumulative total with one step before ) \n\n----------------------------------------------------------------------------------\n\n");
		QQ = 0;
		for (j = 0; j < 1024; j++) {
			imp[4][j] = 100000;
			if (imp[0][j] == 1) {
				QQ++;
				for (n = 0; n < 64; n++) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;

					ny[(int)imp[5][j]][j] = y[(int)imp[5][j]][j]; //一つ目の基底選択
					ny[(int)imp[6][j]][j] = y[(int)imp[6][j]][j]; //2つ目の基底選択
					ny[(int)imp[7][j]][j] = y[(int)imp[7][j]][j]; //3つ目の基底選択
					ny[n][j] = y[n][j]; // 4つ目の基底選択

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
					if (imp[4][j] > sum / 64.0) {
						imp[4][j] = sum / 64.0;
						imp[8][j] = n;
					}
				}
				imp[2][j] = imp[1][j] - imp[4][j];
				imp[3][j] += imp[2][j];
				imp[1][j] = imp[4][j];
				if (imp[0][j] == 1)
					imp_rate[4][j] = imp[3][j] * 100 / result_ica0[1][j];

			}
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (imp[0][j] != 1)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/Basis4.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		fprintf(fp3, " +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp3, "----+");

		fprintf(fp3, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp3, "[%2d]|", i + 1);
		fprintf(fp3, " ## |                                                                                    ");

		for (j = 0; j < 32; j++) {

			fprintf(fp3, "\n +----+");
			for (i = 0; i < 33; i++)
				fprintf(fp3, "----+");
			fprintf(fp3, "\n |[%2d]|", j + 1);
			for (i = 0; i < 32; i++) {
				if (imp[0][i + 32 * j] == 1)
					fprintf(fp3, "%4d|", (int)imp[1][i + 32 * j]);
				else
					fprintf(fp3, "    |");
			}
			fprintf(fp3, "[%2d]|", j + 1);
		}
		fprintf(fp3, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp3, "----+");

		fprintf(fp3, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp3, "[%2d]|", i + 1);
		fprintf(fp3, " ## |                                                                                    ");

		fprintf(fp3, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp3, "----+");

		fprintf(fp3, "\n\n\n  count : %4d / 1024\n\n", QQ);

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			fprintf(fp3, "\n\n -------------------- [ area No.%d (%2d , %2d)] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j, ml + 1, mk + 1);
			if (imp[0][j] == 1) {
				fprintf(fp3, "   MSE Value : %lf  ->  %lf", result_ica[1][j], imp[1][j]);
				fprintf(fp3, "\n\n   Amount of improvement from basis 1 : %lf", imp[2][j]);
				fprintf(fp3, "\n\n   Cumulative amount of improvement from basis 1  [ improvement rate ] : %lf  [ %lf ]", imp[3][j], (imp[3][j] / result_ica0[1][j]) * 100);

			}
			if (imp[1][j] <= percent)
				imp[0][j] = 0;
		}

		///////////////////////////////////////////////////////////// /////////////////  5basis
		printf("\n");
		fprintf(fp4, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp4, "\n\n  Area with 500 or more MSE \n\n\n  Number of basis used : 4, 5 \n\n\n  Area MSE value and basis improvement ( comparison and cumulative total with one step before ) \n\n----------------------------------------------------------------------------------\n\n");
		QQ = 0;
		for (j = 0; j < 1024; j++) {
			imp[4][j] = 100000;
			if (imp[0][j] == 1) {
				QQ++;
				for (n = 0; n < 64; n++) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;

					ny[(int)imp[5][j]][j] = y[(int)imp[5][j]][j]; //一つ目の基底選択
					ny[(int)imp[6][j]][j] = y[(int)imp[6][j]][j]; //2つ目の基底選択
					ny[(int)imp[7][j]][j] = y[(int)imp[7][j]][j]; //3つ目の基底選択
					ny[(int)imp[8][j]][j] = y[(int)imp[8][j]][j]; //4つ目の基底選択
					ny[n][j] = y[n][j]; // 5つ目の基底選択

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
					if (imp[4][j] > sum / 64.0) {
						imp[4][j] = sum / 64.0;
						imp[9][j] = n;
					}
				}
				imp[2][j] = imp[1][j] - imp[4][j];
				imp[3][j] += imp[2][j];
				imp[1][j] = imp[4][j];
				if (imp[0][j] == 1)
					imp_rate[5][j] = imp[3][j] * 100 / result_ica0[1][j];
			}
			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (imp[0][j] != 1)
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
		}

		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/Basis5.bmp");
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		fprintf(fp4, " +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp4, "----+");

		fprintf(fp4, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp4, "[%2d]|", i + 1);
		fprintf(fp4, " ## |                                                                                    ");

		for (j = 0; j < 32; j++) {
			fprintf(fp4, "\n +----+");
			for (i = 0; i < 33; i++)
				fprintf(fp4, "----+");
			fprintf(fp4, "\n |[%2d]|", j + 1);
			for (i = 0; i < 32; i++) {
				if (imp[0][i + 32 * j] == 1)
					fprintf(fp4, "%4d|", (int)imp[1][i + 32 * j]);
				else
					fprintf(fp4, "    |");
			}
			fprintf(fp4, "[%2d]|", j + 1);
		}
		fprintf(fp4, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp4, "----+");

		fprintf(fp4, "\n | ## |");
		for (i = 0; i < 32; i++)
			fprintf(fp4, "[%2d]|", i + 1);
		fprintf(fp4, " ## |                                                                                    ");

		fprintf(fp4, "\n +----+");
		for (i = 0; i < 33; i++)
			fprintf(fp4, "----+");

		fprintf(fp4, "\n\n\n  count : %4d / 1024\n\n", QQ);

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			fprintf(fp4, "\n\n -------------------- [ area No.%d (%2d , %2d)] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j, ml + 1, mk + 1);
			if (imp[0][j] == 1) {
				fprintf(fp4, "   MSE Value : %lf  ->  %lf", result_ica[1][j], imp[1][j]);
				fprintf(fp4, "\n\n   Amount of improvement from basis 1 : %lf", imp[2][j]);
				fprintf(fp4, "\n\n   Cumulative amount of improvement from basis 1  [ improvement rate ] : %lf  [ %lf ]", imp[3][j], (imp[3][j] / result_ica0[1][j]) * 100);

			}
			if (imp[1][j] <= percent)
				imp[0][j] = 0;
		}

		///////////////////////////////////////////////////////////// /////////////////  各領域の最適基底・準最適基底の調査
		printf("\n");

		printf(" ~ Investigation of Optimal basis ~\n What percentage do you use ? : ");
		scanf("%lf", &percent);

		fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp5, "\n\n  Optimal and semi-optimal basis for each area \n\n\n  Number of basis used : 0, 1 \n\n\n  Optimal -> $$,  Semi-optimal -> ## \n\n\n  Rightmost explanation ->  Number of optimal basis , Semi-optimal sum \n\n\n  Bottom explanation ->  Optimal sum , semi-optimal sum \n\n\n  Use Percentage : 0 ~ %lf\n\n----------------------------------------------------------------------------------\n\n", percent);

		
		fprintf(fp5, " +----+");
		for (i = 0; i < 67; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n | ## |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "[%2d]|", i);
		fprintf(fp5, " ## | op |semi|");

		for (j = 0; j < 1024; j++) {
			QQ = 0;
			QQQ = 0;
			fprintf(fp5, "\n +----+");
			for (i = 0; i < 67; i++)
				fprintf(fp5, "----+");
			fprintf(fp5, "\n |%4d|", j);
			for (i = 0; i < 64; i++) {
				if ((int)test3[0][j] == i) {
					fprintf(fp5, " $$ |");
					semi[0][i]++;
				}
				else if (0 < test2[0][j] - test2[(int)test3[i][j]][j] && test2[0][j] - test2[(int)test3[i][j]][j] <= percent && test2[(int)test3[i][j]][j] >= 0) {
					fprintf(fp5, " ## |");
					no_op[j]++;
					semi[1][i]++;
				}
				else
					fprintf(fp5, "    |");
			}
			fprintf(fp5, "%4d|", j);
			fprintf(fp5, "[%2d]|", (int)test3[0][j]);
			fprintf(fp5, "%4d|", no_op[j]);

		}
		fprintf(fp5, "\n +----+");
		for (i = 0; i < 67; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n | ## |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "[%2d]|", i);
		fprintf(fp5, " ## | ## | ## |");

		fprintf(fp5, "\n +----+");
		for (i = 0; i < 67; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n | op |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "%4d|", semi[0][i]);
		fprintf(fp5, " ## | ## | ## |                                                                                   ");

		fprintf(fp5, "\n +----+");
		for (i = 0; i < 67; i++)
			fprintf(fp5, "----+");

		fprintf(fp5, "\n |semi|");
		for (i = 0; i < 64; i++) {
			fprintf(fp5, "%4d|", semi[1][i]);
		}
		fprintf(fp5, " ## | ## | ## |                                                                                   ");
		fprintf(fp5, "\n +----+");
		for (i = 0; i < 67; i++)
			fprintf(fp5, "----+");
		fprintf(fp5, "\n\n\n\n\n\n");

		// 置き換え不可能領域の画像確認
		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai3[a][b] = origin[a][b];

		for (j = 0; j < 1024; j++) {
			mk = j % 32;
			ml = j / 32;
			if (no_op[j] != 0 || (result_ica[1][j] - dcoe_temp[0][j]) > 100.0) {
				for (a = 0; a < 8; a++)
					for (b = 0; b < 8; b++)
						temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
			}
			else
				temp6[(int)test3[0][j]]++;
		}
		
		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = temp_sai3[a][b];

		sprintf(output, "OUTPUT/DCT/not_replaceable[%d].bmp", (int)percent);
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		//gnuplot2(temp6);
		//gnuplot4(imp_rate);
			

		for (QQ = 1; QQ < 10; QQ++) {
			if (QQ == 0)
				percent = 10;
			else if (QQ == 1)
				percent = 25;
			else if (QQ == 2)
				percent = 50;
			else
				percent = 100;

			for (j = 0; j < 1024; j++) {
				for (i = 0; i < 64; i++)
					ny[i][j] = 0;

				for (i = 0; i < 64; i++) {
					if (0 <= test2[0][j] - test2[i][j] && test2[0][j] - test2[i][j] <= QQ && test2[i][j] >= 0)
						QQQ = (int)test3[i][j];
				}

				ny[QQQ][j] = y[QQQ][j];
				printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
			}
			printf("\r [ Execution finished ]          ");
			printf("\n\n");

			// 初期化（必ず行う）
			for (a = 0; a < 64; a++)
				xx[a] = 0.0;

			seki5(nw, ny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = ica_sai[a][b];

			sprintf(output, "OUTPUT/test/[%d].bmp", QQ);
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		}

		for (QQ = 0; QQ < 4; QQ++) {
			if (QQ == 0)
				percent = 10;
			else if (QQ == 1)
				percent = 25;
			else if (QQ == 2)
				percent = 50;
			else
				percent = 100;

			for (i = 1; i < 64; i++)
				for (j = 0; j < 1024; j++)
					if (0 < test2[0][j] - test2[i][j] && test2[0][j] - test2[i][j] <= percent && test2[i][j] >= 0) {
						ica_test1[(int)test3[i][j]][(int)test3[0][j]]++;
						//temp_sai2[(int)test3[i][j]][j]++;
						average2[j] = 0;
					}

			// 置換可能領域を画像で確認

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					if (temp_sai2[i][j] == 0)
						average2[j]++;

			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai3[a][b] = origin[a][b];

			for (j = 0; j < 1024; j++) {
				mk = j % 32;
				ml = j / 32;
				if (average2[j] != 0)
					for (a = 0; a < 8; a++)
						for (b = 0; b < 8; b++)
							temp_sai3[ml * 8 + b][mk * 8 + a] = 0;
			}

			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = temp_sai3[a][b];

			sprintf(output, "OUTPUT/DCT/replaceable[%d].bmp", (int)percent);
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
		}

		// 1->2改善量
	

		for (QQ = 0; QQ < 4; QQ++) {
			if (QQ == 0)
				percent = 10;
			else if (QQ == 1)
				percent = 25;
			else if (QQ == 2)
				percent = 50;
			else
				percent = 100;

			for (j = 0; j < 1024; j++) {
				for (i = 0; i < 64; i++)
					ny[i][j] = 0;

				for (i = 0; i < 64; i++) {
					if (0 <= test2[0][j] - test2[i][j] && test2[0][j] - test2[i][j] <= percent && test2[i][j] >= 0)
						QQQ = (int)test3[i][j];
				}

				ny[QQQ][j] = y[QQQ][j];
				printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
			}
			printf("\r [ Execution finished ]          ");
			printf("\n\n");

			// 初期化（必ず行う）
			for (a = 0; a < 64; a++)
				xx[a] = 0.0;

			seki5(nw, ny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = ica_sai[a][b];

			sprintf(output, "OUTPUT/MSE/[%d].bmp", (int)percent);
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

		}






		for (b = 0; b < 64; b++) {
			temp1[b] = 0;
			temp2[b] = 0;
			temp3[b] = 0;
			temp4[b] = 0;
			temp5[b] = 0;
		}

		// 1 -> 64 までのMSE調査

		fprintf(fp6, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp6, "\n\n  confirmation of MSE \n\n\n  Number of basis used : 1 ~ 64 \n\n----------------------------------------------------------------------------------\n\n");

		for (j = 0; j < 1024; j++) {

			for (b = 0; b < 1024; b++)
				for (a = 0; a < 64; a++)
					ny[a][b] = y[a][b];

			for (c = 0; c < 64; c++) {

				sum0 = 10000;
				QQ = 0;
				printf("a");

				for (n = 0; n < 64; n++) {
					for (a = 0; a < 64; a++)
						ny[a][j] = y[a][j];

					if (c != 0)
						for (a = 0; a < c; a++)
							ny[(int)full_mse[0][a][j]][j] = 0;

					if (ny[n][j] != 0) {
						ny[n][j] = 0; // iつ目の基底選択

						// 初期化（必ず行う）
						for (a = 0; a < 64; a++)
							xx[a] = 0.0;

						seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
						xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
						avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

						//for (b = 0; b < 8; b++)
						//	for (a = 0; a < 8; a++)
						//		for (l = 0; l < 8; l++)
						//			for (m = 0; m < 8; m++)
						//				seg[64 * 8 * b + 8 * a + l * 64 + m] = block_ica[b * 8 + a];

						//sprintf(output, "OUTPUT/ICA/%d/[%d].bmp", j, c);
						//img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す

						sum = 0.0;
						mk = j % 32;
						ml = j / 32;
						for (a = 0; a < 8; a++) {
							for (b = 0; b < 8; b++) {
								sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
							}
						}

						if (sum0 > sum / 64.0) {
							sum0 = sum / 64.0;
							QQ = n;
						}

					}
				}
				full_mse[1][c][j] = sum0;
				full_mse[0][c][j] = (double)QQ;
			}
			//	for (a = 0; a < c; a++)
			//		ny[(int)full_mse[0][a][j]][j] = 0;

			//	// 初期化（必ず行う）
			//	for (a = 0; a < 64; a++)
			//		xx[a] = 0.0;

			//	seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
			//	xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
			//	avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

			//	for (b = 0; b < 8; b++)
			//		for (a = 0; a < 8; a++)
			//			for (l = 0; l < 8; l++)
			//				for (m = 0; m < 8; m++)
			//					seg[64 * 8 * b + 8 * a + l * 64 + m] = block_ica[b * 8 + a];

			//	sprintf(output, "OUTPUT/ICA/%d/[%d].bmp", j, c);
			//	img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す

			//}




			printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");

		for (j = 0; j < 1024; j++) {
			for (i = 0; i < 64; i++) {
				mse100[i][j] = full_mse[1][i][j];
			}
			//gnuplot(mse100);
		}







		/*	printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");*/

		

		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);
		printf("\n");

		/*fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
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
			for (a = 0; a < 1024; a++)
				sum += temp_sai2[j][a];
			fprintf(fp5, "\n +----+");
			for (i = 0; i < 65; i++)
				fprintf(fp5, "----+");
			fprintf(fp5, "\n |[%2d]|", j);
			for (i = 0; i < 64; i++) {
				if (j == i)
					fprintf(fp5, " ## |");
				else{
					if (50 < (temp_sai3[i][j] / sum) * 100) {
						fprintf(fp5, "%4d|", (int)((temp_sai3[i][j] / sum) * 100));
						temp5[i]++;
					}
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

		fprintf(fp5, "\n | ## |");
		for (i = 0; i < 64; i++)
			fprintf(fp5, "[%2d]|", (int)temp5[i]);
		fprintf(fp5, " ## |");

		fprintf(fp5, "\n +----+");
		for (i = 0; i < 65; i++)
			fprintf(fp5, "----+");


		fprintf(fp5, "\n\n\n\n\n\n\n");*/
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


		//fprintf(fp, "\n\n Use image  :  %s\n\n\n", filename);
		//fprintf(fp, "\n\n  Core basis investigation \n\n\n  Number of basis used : 0, 1 \n\n\n  MSE difference used : %lf\n\n----------------------------------------------------------------------------------\n\n", percent);

		//for (j = 0; j < 1024; j++) {
		//	fprintf(fp, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
		//	for (i = 0; i < 64; i++) {
		//		if (test2[0][j] - test2[i][j] <= percent && test2[i][j] >= 0) {
		//			if (count_temp[0][j] == 0) {
		//				total_test[0][(int)test3[i][j]]++;
		//				//total_test[10][0]++;
		//			}
		//			else if (count_temp[0][j] == 1) {
		//				total_test[1][(int)test3[i][j]]++;
		//				//total_test[10][1]++;
		//			}
		//			else if (count_temp[0][j] == 2) {
		//				total_test[2][(int)test3[i][j]]++;
		//				//total_test[10][2]++;
		//			}
		//			else if (count_temp[0][j] == 3) {
		//				total_test[3][(int)test3[i][j]]++;
		//				//total_test[10][3]++;
		//			}
		//			else if (count_temp[0][j] == 4) {
		//				total_test[4][(int)test3[i][j]]++;
		//				//total_test[10][4]++;
		//			}
		//			if (count_temp[0][j] != 0)
		//				fprintf(fp, " [%2d] : %4.5lf  ( MSE Value )\n\n", (int)test3[i][j], mse_ica[(int)test3[i][j]][j]);

		//		}
		//	}
		//	if (count_temp[0][j] == 0) {
		//		//total_test[0][(int)test3[i][j]]++;
		//		total_test[10][0]++;
		//	}
		//	else if (count_temp[0][j] == 1) {
		//		//total_test[1][(int)test3[i][j]]++;
		//		total_test[10][1]++;
		//	}
		//	else if (count_temp[0][j] == 2) {
		//		//total_test[2][(int)test3[i][j]]++;
		//		total_test[10][2]++;
		//	}
		//	else if (count_temp[0][j] == 3) {
		//		//total_test[3][(int)test3[i][j]]++;
		//		total_test[10][3]++;
		//	}
		//	else if (count_temp[0][j] == 4) {
		//		//total_test[4][(int)test3[i][j]]++;
		//		total_test[10][4]++;
		//	}
		//}

		//fprintf(fp, "\n\n\n\n\n\n");

		//fprintf(fp, "\n               0 basis                  ~ 1.1                1.1 ~ 2.0               2.0 ~                2 basis             ");
		//fprintf(fp, "\n +----+-----------------------+----------------------+----------------------+----------------------+----------------------+----+\n");
		//for (b = 0; b < 64; b++) {
		//	fprintf(fp, " | %2d | ", b);
		//	for (i = 0; i < 5; i++) {
		//		fprintf(fp, "      %3d / %3d (%3d%%)|", (int)total_test[i][b], (int)total_test[10][i], (int)((total_test[i][b] / total_test[10][i]) * 100));
		//	}
		//	fprintf(fp, " %2d | ", b);
		//	fprintf(fp, "\n +----+-----------------------+----------------------+----------------------+----------------------+----------------------+----+\n");

		//}
	printf("<ica fin>\n\n");
	/////////////////////////////////ica 終了/////////////////////////////////////////


	// //////////////////////////// dct ////////////////////////////////////////
	// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

	//動作確認
	printf("<dct start>\n\n");
	//printf("Do you want to run the DCT ? [ y/n ] : ");
	//scanf("%s", &yn);

	if (yn == 'n') {
		//fprintf(fp, "\n\n\n- - - - - - - - - - - - - - - - ( Reference ) For DCT - - - - - - - - - - - - - - - \n\n\n");
		// 10段階品質があるから10段階分やる
		for (Q = 100; Q > 0; Q -= 10) {
			printf("\r now Q is %d          \n", Q);


			//Q = 100;

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			idct(dcoe, dcoe2, 8); // 普通の再構成

			n = 0;
			for (i = 0; i < 256; i += 8) {
				for (j = 0; j < 256; j += 8) {
					m = 0;
					for (k = 0; k < 8; k++) {
						for (l = 0; l < 8; l++) {
							dcoe_temp[m][n] = dcoe[i + k][j + l]; //dct64*1024 -> coe256*256を格納
							m++;
						}
					}
					n++;
				}
			}

			for (j = 0; j < 1024; j++) {

				//for (a = 0; a < 256; a++) {
				//	for (b = 0; b < 256; b++) {
				//		sum += pow((double)origin[a][b] - (double)dcoe2[a][b], 2);
				//	}
				//}
				sum = 0.0;
				mk = j % 32;
				ml = j / 32;
				for (a = 0; a < 8; a++) {
					for (b = 0; b < 8; b++) {
						sum += pow(origin[ml * 8 + b][mk * 8 + a] - dcoe2[ml * 8 + b][mk * 8 + a], 2);
					}
				}
				mse_dct[0][(Q / 10) - 1][j] = sum / 64;//平均

				//sum = sum / (256.0 * 256.0); mse_ica

				i = 0;

				for (b = 0; b < 64; b++)
					if (dcoe_temp[b][j] != 0)
						i++;

				mse_dct[1][(Q / 10) - 1][j] = i;
			}
			//for (a = 0; a < 256; a++)
			//	for (b = 0; b < 256; b++)
			//		temp_sai[a * 256 + b] = dcoe2[a][b];
			
			//for (i = 0; i < 1024; i++) {

			//	mk = i % 32;
			//	ml = i / 32;
			//	for (b = 0; b < 8; b++)
			//		for (a = 0; a < 8; a++)
			//			for (l = 0; l < 8; l++)
			//				for (m = 0; m < 8; m++)
			//					seg[64 * 8 * b + 8 * a + l * 64 + m] = temp_sai[256 * 8 * ml + 8 * mk + a + 256 * b];

			//	

			//	sprintf(output, "OUTPUT/DCT/%d/[%d].bmp", Q, i);
			//	img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
			//}

			//for (a = 0; a < 256; a++)
			//	for (b = 0; b < 256; b++)
			//		temp_sai[a * 256 + b] = dcoe2[a][b];

			//sprintf(output, "OUTPUT/DCT%d.bmp", Q);
			//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

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

	for (j = 0; j < 1024; j++){
		for (a = 9; a > 0; a-=1)
			if (mse_dct[0][a][j] < 300) {
				bunrui[0] = mse_dct[1][a][j];
				bunrui[1] = mse_dct[0][a][j];
				Q = a;
			}
		for (a = 0; a < 64; a++)
			if (full_mse[1][a][j] < 300) {
				bunrui[2] = 63.0 - a;
				bunrui[3] = full_mse[1][a][j];
			}
		fprintf(fp6, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);

			fprintf(fp6, "\n\n    DCT NUM : %2d (%3d)\n\n    DCT mse : %lf\n", (int)bunrui[0], (Q+1)*10, bunrui[1]);
			fprintf(fp6, "\n\n    ICA NUM : %2d\n\n    ICA mse : %lf\n", (int)bunrui[2], bunrui[3]);
	}


	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
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
