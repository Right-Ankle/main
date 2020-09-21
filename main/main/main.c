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
	static int i, j, n, m, k, l, mk, ml, Q, QQ, QQQ, QQQQ, b, a, c, seg[64 * 64], y_rank[64][1024], seg0[64 * 64], seg1[64 * 64], ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai11[256 * 256], temp_sai22[256 * 256], temp_sai2[64][1024], temp_sai3[256][256], temp_sai4[64 * 64], ica[64], temp_temp[64];
	static double sum, sum0, sum1, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[1024], mse_ica[64][1024], mse_ica0[64][1024], mse_ica1[64][1024], cost_ica[1024], cost_dct[1024], mse_ica2[1024], result_dct[2][1024], result_ica[2][1024], result[2][1024], lambda = 1024.0;
	static double result_coe, coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 }, test[5][1024], test2[1024], test3[1024], ica_test[64][64][1024], ica_test2[2][64][1024], ica_test3[2][1024], ica_test4[2][1024];
	static double avg[1024], y0[64][1024], y1[64][1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dct_cost[64][1024], mse_cost[64][1024], ica_bent[1024], dct_bent[1024], ica_ent[64][1024], dct_ent[64][1024], dcoe_temp[64][1024] = { 0 };
	static unsigned char dammy[256][256] = { 0 };
	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
	static unsigned char  ica_sai[256][256] = { 0 }, ica_sai0[256][256] = { 0 }, ica_sai1[256][256] = { 0 };
	static struct pca pcaStr = { 0 };
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	char g[50];
	char output[1000], in[50], out[50];
	struct tt** sort_d, temp;
	double* temp_1;
	double* temp_2;
	double* temp_3;

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
	for (j = 0; j < 1024; j++) {
		sprintf(g, "mkdir OUTPUT\\ICA\\COEFFICIENT\\coe[%d]", j);
		system(g);
		if (j % 128 == 0)
			printf(" @");
	}
	_mkdir("OUTPUT\\DCT"); //実験中のICAとDCTの再構成画像を出力
	_mkdir("OUTPUT\\MSE"); //MSE比較後、基底ごとの領域で分割した画像を出力
	_mkdir("OUTPUT\\test"); //MSE比較後、基底ごとの領域で分割した画像を出力
	//_mkdir("DEFAULT"); //MSE比較後、基底ごとの領域で分割した画像を出力
	for (j = 0; j < 1024; j++) {
		sprintf(g, "mkdir OUTPUT\\test\\MSE[%d]", j);
		system(g);
		if (j % 128 == 0)
			printf(" @");
	}
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


	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	//for (i = 0; i < 20 - 3; i++) {
	//	for (j = 0; j < 4; j++) {
	//		if (filename[i + j] == "." && filename[i + j] == "b" && filename[i + j] == "m" && filename[i + j] == "p") {
	//			filename[i + j] = "";
	//		}
	//	}
	//}


	/* 一次元配列から二次元配列に変換 */
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin[i][j] = ori_temp[i * 256 + j];

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai[a * 256 + b] = origin[a][b];

	//sprintf(output, "DEFAULT/GEN.bmp");
	//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	//ソート用
	sort_d = (struct tt**)malloc(sizeof(struct tt*) * 64);
	for (i = 0; i < 64; i++) {
		sort_d[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
	}

	temp_1 = (double*)malloc(sizeof(double) * 1024);
	temp_2 = (double*)malloc(sizeof(double) * 1024);
	temp_3 = (double*)malloc(sizeof(double) * 1024);


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


	//for (i = 0; i < 32; i++) {
	//	for (j = 0; j < 32; j++) {
	//		for (b = 0; b < 8; b++)
	//			for (a = 0; a < 8; a++)
	//				for (n = 0; n < 8; n++)
	//					for (m = 0; m < 8; m++)
	//						seg[64 * 8 * b + 8 * a + n * 64 + m] = ori_temp[256 * 8 * i + 8 * j + a + 256 * b];
	//		sprintf(output, "ORIGIN/ORIGIN[%d].bmp", i * 32 + j);
	//		img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
	//		sprintf(output, "test/MSE[%d]/ORIGIN[%d].bmp", i * 32 + j, i * 32 + j);
	//		img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
	//	}
	//}



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



	// 普通の再構成処理（ICAで分解した"y"と"w"から"origin"を再構成）
	//（関係ない）

	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			ny[j][i] = 0.0;

	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②



	for (k = 0; k < 32; k++) {
		for (l = 0; l < 32; l++) {
			sum = 0.0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					sum += pow(origin[i + (8 * k)][j + (8 * l)] - ica_sai[i + (8 * k)][j + (8 * l)], 2);
				}
			}
			mse_ica2[k * 32 + l] = sum / 64;//平均
			//mse_ica2[k * 32 + l] = test2[k * 32 + l];
			test3[k * 32 + l] = 0.0;
		}
	}

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai[a * 256 + b] = ica_sai[a][b];

	//sprintf(output, "OUTPUT\\DEFAULT/ICA.bmp");
	//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	gnuplot3(test2, test3);

	for (b = 0; b < 1024; b++) {
			fprintf(fp2, " [%d] -->  %lf\n", b, test2[b]);
	}


	//b_entropy_ica(ny, ica_bent);
	//b_mse(origin, ica_sai, mse_ica2);
	//make_cost(ica_bent, mse_ica2, cost_ica);


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

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai[a * 256 + b] = origin[a][b];

	for(i=0;i<1024;i++){
		k = i % 32;
		l = i / 32;
		if ((mse_ica2[i] - result_ica[1][i]) < 1000) {
			for (b = 0; b < 8; b++) {
				for (a = 0; a < 8; a++) {
					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
				}
			}
		}
	}

	sprintf(output, "OUTPUT\\under10.bmp");
	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++)
			temp_sai[a * 256 + b] = origin[a][b];

	for (i = 0; i < 1024; i++) {
		k = i % 32;
		l = i / 32;
		if ((mse_ica2[i] - result_ica[1][i]) < 500) {
			for (b = 0; b < 8; b++) {
				for (a = 0; a < 8; a++) {
					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
				}
			}
		}
	}

	sprintf(output, "OUTPUT\\under5.bmp");
	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

	fprintf(fp2, "\n---------------------\n");
	for (b = 0; b < 1024; b++) {
		fprintf(fp2, " [%d] -->  %lf  :  %lf     -------->   %lf\n", b, test2[b] , result_ica[1][b], test2[b]-result_ica[1][b]);
	}

	for (b = 0; b < 1024; b++) {
		test3[b] = test2[b] - result_ica[1][b];
	}

	for (b = 0; b < 1024; b++)
		test2[b] = test3[b];
	gnuplot3(test2, test3);

	// ica全部とDCT全部を比較中
	for (i = 0; i < 1024; i++) {
		for (a = 0; a < 64; a++) {
			mse_ica1[a][i] = sort_d[a][i].num;
			mse_ica0[a][i] = sort_d[a][i].val;
			if (result_ica[0][i] == a)
				ny[a][i] = y[a][i];
			else
				ny[a][i] = 0;
		}
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
			y_rank[sort_d[i][n].num][n] = i;
		}
	}

	for (i = 0; i < 1024; i++) {
		for (a = 0; a < 64; a++) {
			if (a == sort_d[0][i].num)
				y0[a][i] = y[a][i];
			else
				y0[a][i] = 0;
		}
	}

	for (i = 0; i < 1024; i++) {
		for (a = 0; a < 64; a++) {
			if (a == sort_d[1][i].num)
				y1[a][i] = y[a][i];
			else
				y1[a][i] = 0;
		}
	}

	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	seki5(nw, y0, x); // x -> nw * ny
	xtogen(x, ica_sai0, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai0, avg); // ica_sai -> 再構成済②

	seki5(nw, y1, x); // x -> nw * ny
	xtogen(x, ica_sai1, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai1, avg); // ica_sai -> 再構成済②

	for (a = 0; a < 256; a++)
		for (b = 0; b < 256; b++) {
			temp_sai[a * 256 + b] = ica_sai[a][b];
			temp_sai11[a * 256 + b] = ica_sai0[a][b];
			temp_sai22[a * 256 + b] = ica_sai1[a][b];

		}

	printf("+ - - - - - Now Running - - - - +\n");
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			for (b = 0; b < 8; b++)
				for (a = 0; a < 8; a++)
					for (n = 0; n < 8; n++)
						for (m = 0; m < 8; m++) {
							seg[64 * 8 * b + 8 * a + n * 64 + m] = temp_sai[256 * 8 * i + 8 * j + a + 256 * b];
							seg0[64 * 8 * b + 8 * a + n * 64 + m] = temp_sai11[256 * 8 * i + 8 * j + a + 256 * b];
							seg1[64 * 8 * b + 8 * a + n * 64 + m] = temp_sai22[256 * 8 * i + 8 * j + a + 256 * b];

						}

			sum = 0;
			sum0 = 0;
			sum1 = 0;
			for (a = 0; a < 8; a++) {
				for (b = 0; b < 8; b++) {
					sum += pow(origin[i * 8 + b][j * 8 + a] - ica_sai[i * 8 + b][j * 8 + a], 2);
					sum0 += pow(origin[i * 8 + b][j * 8 + a] - ica_sai0[i * 8 + b][j * 8 + a], 2);
					sum1 += pow(origin[i * 8 + b][j * 8 + a] - ica_sai1[i * 8 + b][j * 8 + a], 2);
				}
			}
			sum = sum / 64;//平均
			sum0 = sum0 / 64;//平均
			sum1 = sum1 / 64;//平均

			test2[i * 32 + j] = fabs(sum0 - sum1);
			test3[i * 32 + j] = fabs(sum - sum1);
			test2[i * 32 + j] = sum;

			QQ = 0;
			QQQ = 0;
			QQQQ = 0;

			for (b = 0; b < 64; b++)
				if (result_ica[0][i * 32 + j] == sort_d[b][i * 32 + j].num)
					QQ = b;

			for (b = 0; b < 64; b++)
				if (mse_ica1[b][i * 32 + j] == sort_d[0][i * 32 + j].num)
					test3[i * 32 + j] = b;

			for (b = 0; b < 64; b++)
				if (mse_ica1[b][i * 32 + j] == sort_d[1][i * 32 + j].num)
					test3[i * 32 + j] = b;

			//sprintf(output, "test/MSE[%d]/BEST[No.%d][%d].bmp", i * 32 + j, QQ, (int)sum);
			//img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
			//sprintf(output, "test/MSE[%d]/USE0[Rank.%d][%d].bmp", i * 32 + j, QQQ, (int)sum0);
			//img_write_gray(seg0, output, 64, 64); // outputに出力画像を書き出す
			//sprintf(output, "test/MSE[%d]/USE1[Rank.%d][%d].bmp", i * 32 + j, QQQQ, (int)sum1);
			//img_write_gray(seg1, output, 64, 64); // outputに出力画像を書き出す

			test3[i * 32 + j] = fabs(test2[i * 32 + j] - test3[i * 32 + j]);

		}
		if (i % 2 == 0)
			printf(" @");
	}
	printf("\n\n");

	for (m = 0; m < 2500; m+=100) {
		for (a = 0; a < 256; a++)
			for (b = 0; b < 256; b++)
				temp_sai[a * 256 + b] = origin[a][b];

		for (i = 0; i < 1024; i++) {
			k = i % 32;
			l = i / 32;
			if (test2[i] < m) {
				for (b = 0; b < 8; b++) {
					for (a = 0; a < 8; a++) {
						temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
					}
				}
			}
		}

		sprintf(output, "OUTPUT\\DCT/MSE_over%d.bmp", m);
		img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
			//gnuplot3(test2, test3);
			//gnuplot3(test3, test2);
	}

		for (a = 0; a < 1024; a++)
		for (b = 0; b < 64; b++) {
			QQ = 0;
			for (i = 0; i < 64; i++) {
				if (mse_ica1[i][a] == sort_d[b][a].num)
					QQ = i;
			}
			dcoe_temp[b][a] = mse_ica0[QQ][a];
		}


fprintf(fp3, " [%d] : [Min MSE]  -->  %lf\n", i, result_coe);

	//sprintf(output, "DCT/ICA_sai.bmp");
	//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す



	for (k = 0; k < 32; k++) {
		for (l = 0; l < 32; l++) {
			sum = 0.0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					sum += pow(origin[i + (8 * k)][j + (8 * l)] - ica_sai[i + (8 * k)][j + (8 * l)], 2);
				}
			}
			best_ica[k * 32 + l] = sum / 64;//平均
		}
	}

	// 係数確認
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

	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 64; i++) {
			fprintf(fp4, "%lf, ", y[i][j]);
		}
		fprintf(fp4, "\n");
	}

	for (j = 0; j < 1024; j++) {
		test[0][j] = fabs(y[sort_d[0][j].num][j]); // 最大係数値
		sum = 0;
		for (i = 0; i < 64; i++)
			sum += fabs(y[i][j]);
		test[1][j] = sum / 64.0; // 平均値
		test[2][j] = fabs(y[sort_d[0][j].num][j]) - (sum / 64.0); //最大値と平均値の差

		//ジニ係数
		sum2 = 0;
		sum = sum / 64;
		for (i = 0; i < 64; i++)
			for (k = 0; k < 64; k++)
				sum2 += fabs(fabs(y[i][j]) - fabs(y[k][j]));
		sum2 = sum2 / 64.0;
		sum2 = sum2 / 64;
		test[3][j] = sum2 / (sum * 2); // 同じ位置の平均差はカウントしないため，64*63となる

		for (b = 0; b < 64; b++)
			if (result_ica[0][j] == sort_d[b][j].num)
				test[4][j] = (double)b; //最小MSEの係数順位
	}

	//相関係数
	fprintf(fp2, " *************************************************************\n\n\n");
	fprintf(fp2, " ** Comparision **\n");

	fprintf(fp2, " *************************************************************\n\n\n");
	fprintf(fp2, " ** Comparision **\n");
	for (i = 0; i < 4; i++) {
		for (k = 0; k < 1024; k++) {
			//test2[k] = test[i][k];
			test3[k] = test[4][k];
		}



		//result_coe = cor_coe(test2, test3);
		//gnuplot3(test2, test3);
		//fprintf(fp3, " [%d] : [Min MSE]  -->  %lf\n", i, result_coe);
	}
	sum = 0;
	sum1 = 0;
	i = 0;
	l = 0;
	for (k = 0; k < 1024; k++) {
		if (k % 10 == 0)
			fprintf(fp2, "\n~~~\n\n", k, test[0][k]);
		if (test[2][k] > 4.0) {
			fprintf(fp2, " [%d]  -->  %4.4lf            %5.4lf<---------- +\n", k, test[2][k], test[4][k]);
			if (test[4][k] == 0)
				sum++;
			temp_1[i] = test[2][k];
			temp_2[i] = test[4][k];
			i++;
		}
		else if (test[2][k] < 1.0) {
			fprintf(fp2, " [%d]  -->  %4.4lf            %5.4lf<---------- -\n", k, test[2][k], test[4][k]);
			if (test[4][k] == 0)
				sum1++;
			temp_1[i] = test[2][k];
			temp_2[i] = test[4][k];
			i++;
			l++;
		}
		else {
			if (test[4][k] == 0)
				sum1++;
			fprintf(fp2, " [%d]  -->  %4.4lf            %5.4lf \n", k, test[2][k], test[4][k]);

		}
	}
	l = i - l;
	//fprintf(fp, " [over 5]  -->  %3.0lf\n", (double)l);
	//fprintf(fp, " [0/4over] : %3.0lf / %3.0lf  ---> %3.3lf \n", sum, (double)l, sum/(double)l);
	//fprintf(fp, " [0/all] : %3.0lf / %3.0lf  ---> %3.3lf \n", sum, sum1 + sum, sum / (sum + sum1));

	for (k = 0; k < 1024; k++) {
		if (k % 10 == 0)
			fprintf(fp2, "\n~~~\n\n", k, test[1][k]);
		fprintf(fp2, " [%d]  -->  %4.4lf            %2.0lf\n", k, test[3][k], test[4][k]);
	}

	//係数10以上使用のみ
	double sum11, sum22;
	sum0 = sum1 = sum2 = sum11 = sum22 = result_coe = 0;
	for (j = 0; j < i; j++) {
		sum1 += temp_1[j];
		sum2 += temp_2[j];
	}

	sum1 /= (double)i;
	sum2 /= (double)i;

	for (j = 0; j < i; j++) {
		sum0 += (temp_1[j] - sum1) * (temp_2[j] - sum2);
		sum11 += pow(temp_1[j] - sum1, 2);
		sum22 += pow(temp_2[j] - sum2, 2);
	}

	//result_coe = (sum0 / i) / (sqrt(sum11 / i) * sqrt(sum22 / i));

	//fprintf(fp, " [coe]  -->  %2.4lf\n", result_coe);

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
	printf("Method 2 start - ->\n");
	printf("+ - - - - - Now Running - - - - +\n");
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
		if (j % 64 == 0)
			printf(" @");
	}
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

		for (j = 0; j < 1024; j++) {
			ica_test3[0][j] = sort_d[0][j].val;//mse
			ica_test3[1][j] = (double)sort_d[0][j].num;//基底番号

			ica_test4[0][j] = ica_test3[1][j];//1つ目の基底番号
			ica_test4[1][j] = ica_test2[1][(int)ica_test3[1][j]][j];// 2つ目の基底番号
		}


			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = origin[a][b];

			for (i = 0; i < 1024; i++) {
				k = i % 32;
				l = i / 32;
				if (dcoe_temp[0][i] == ica_test3[0][i]) {
					for (b = 0; b < 8; b++) {
						for (a = 0; a < 8; a++) {
							temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
						}
					}
				}
			}
			// Method 1:2 の1個目と2個目の基底の係数順位を比較中
			sprintf(output, "OUTPUT\\MSE_test.bmp");
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

			fprintf(fp, "\n\n  [Block_num] --> [use 1][use 2] : mse = mse_value \n\n[nolmal]                                                 [random]\n\n------------------\n\n");

			for (b = 0; b < 1024; b++) {
				fprintf(fp, " [%4d] -->  [%2d][%2d] : mse = %5.4f          ", b, (int)result_ica[0][b], (int)dct_coe[0][b], dcoe_temp[0][b]);
				fprintf(fp, " [%4d] -->  [%2d][%2d] : mse = %5.4f          ", b, (int)ica_test4[0][b], (int)ica_test4[1][b], ica_test3[0][b]);
				if (dcoe_temp[0][b] != ica_test3[0][b])
					fprintf(fp, " [%f]\n", dcoe_temp[0][b] - ica_test3[0][b]);
				else
					fprintf(fp, "\n");
				fprintf(fp, "  nomal -->  [%2d][%2d]  :   rank[%2d][%2d]                        ", (int)result_ica[0][b], (int)dct_coe[0][b], y_rank[(int)result_ica[0][b]][b], y_rank[(int)dct_coe[0][b]][b]);
				fprintf(fp, "  random -->  [%2d][%2d]  :   rank[%2d][%2d]\n\n",(int)ica_test4[0][b], (int)ica_test4[1][b], y_rank[(int)ica_test4[0][b]][b], y_rank[(int)ica_test4[1][b]][b]);
			}
			// method1 -> 0位以外使用領域
			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = origin[a][b];

			for (i = 0; i < 1024; i++) {
				k = i % 32;
				l = i / 32;
				if (y_rank[(int)result_ica[0][i]][i] == 0 || y_rank[(int)dct_coe[0][i]][i] == 0 || y_rank[(int)result_ica[0][i]][i] == 1 || y_rank[(int)dct_coe[0][i]][i] == 1) {
					for (b = 0; b < 8; b++) {
						for (a = 0; a < 8; a++) {
							temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
						}
					}
				}
			}
			sprintf(output, "OUTPUT\\Mt1.bmp");
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

			// method2 -> 0位以外使用領域
			for (a = 0; a < 256; a++)
				for (b = 0; b < 256; b++)
					temp_sai[a * 256 + b] = origin[a][b];

			for (i = 0; i < 1024; i++) {
				k = i % 32;
				l = i / 32;
				if (y_rank[(int)ica_test4[0][i]][i] == 0 || y_rank[(int)ica_test4[1][i]][i] == 0 || y_rank[(int)ica_test4[0][i]][i] == 1 || y_rank[(int)ica_test4[1][i]][i] == 1) {
					for (b = 0; b < 8; b++) {
						for (a = 0; a < 8; a++) {
							temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
						}
					}
				}
			}
			sprintf(output, "OUTPUT\\Mt2.bmp");
			img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す

			printf("Method 2 end\n\n");


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
			for (i = 0; i < 32; i++) {
				for (j = 0; j < 32; j++) {
					QQ = 0;
					//for (b = 0; b < 64; b++)
					//	if (mse_dct[0][i * 32 + j] == dct_coe[b][i * 32 + j])
					//		QQ = b;
					/*test2[i * 32 + j] = mse_dct[][i * 32 + j];*/

					//for (b = 0; b < 64; b++)
					//	dcoe_temp[b][i * 32 + j] = mse_dct[b][i * 32 + j];
						//dcoe_temp[b][i * 32 + j] = mse_dct[(int)dct_coe[b][i * 32 + j]][i * 32 + j];

				}
				//printf("%d\n", i);
			}
			for (j = 0; j < 1024; j++)
				test3[j] = mse_dct[0][j];

			result_coe = cor_coe(test2, test3);
			//gnuplot3(test2, test3);
			fprintf(fp3, " [%d] : [Min MSE]  -->  %lf\n", i, result_coe);



			/*gnuplot(test2, test3);*/

			//for (j = 0; j < 64; j++) {

			//	for (a = 0; a < 256; a++)
			//		for (b = 0; b < 256; b++)
			//			temp_sai[a * 256 + b] = origin[a][b];

			//	// mse結果の比較
			//	for (i = 0; i < 1024; i++) {
			//		if (best_dct[i] <= best_ica[i]) { // dctのほうが小さければ
			//			result[0][i] = 0.0;
			//			result[1][i] = best_dct[i]; //基底番号を格納
			//			count[(int)result_dct[0][i]]++;
			//		}
			//		else {
			//			result[0][i] = 1.0;
			//			result[1][i] = best_ica[i];
			//			count[(int)result_ica[0][i]]++;
			//			k = i % 32;
			//			l = i / 32;

			//			if (j != result_ica[0][i]) {
			//				for (b = 0; b < 8; b++) {
			//					for (a = 0; a < 8; a++) {
			//						temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
			//					}
			//				}
			//			}
			//			else {
			//				for (b = 0; b < 8; b++)
			//					for (a = 0; a < 8; a++)
			//						for (n = 0; n < 8; n++)
			//							for (m = 0; m < 8; m++)
			//								seg[64 * 8 * b + 8 * a + n * 64 + m] = ori_temp[256 * 8 * l + 8 * k + a + 256 * b];

			//				QQ = 0;

			//				for (b = 0; b < 64; b++)
			//					if (result_ica[0][i] == sort_d[b][i].num)
			//						QQ = b;

			//				//sprintf(output, "test/ICA[%d]/ORIGIN[%d][%d].bmp", j, l * 32 + k, QQ);
			//				//img_write_gray(seg, output, 64, 64); // outputに出力画像を書き出す
			//			}
			//		}
			//	}

			//	sprintf(output, "MSE/MSE[%d].bmp", j);
			//	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
			//}

			//sum = 0;
			//sum2 = 0;

			//for (a = 0; a < 256; a++)
			//	for (b = 0; b < 256; b++)
			//		temp_sai[a * 256 + b] = origin[a][b];

			//// bent結果の比較
			//for (i = 0; i < 1024; i++) {
			//	if (cost_dct[i] >= cost_ica[i]) { // dctのほうが小さければ
			//		result[0][i] = 0.0;
			//		result[1][i] = best_dct[i]; //基底番号を格納
			//		k = i % 32;
			//		l = i / 32;
			//		for (b = 0; b < 8; b++) {
			//			for (a = 0; a < 8; a++) {
			//				temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
			//			}
			//		}
			//	}
			//	else {
			//		result[0][i] = 1.0;
			//		result[1][i] = best_ica[i];
			//		count[(int)result_ica[0][i]]++;
			//	}
			//	sprintf(output, "RD.bmp", j);
			//	img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す
			//}






			//for (i = 0; i < 1024; i++) {
			//	//if (best_dct[i] - best_ica[i] < 1) {
			//	//	fprintf(fp, "%d, ", 0);
			//	//	sum++;
			//	//}
			//	//else if (best_dct[i] - best_ica[i] < 5) {
			//	//	count[i] = i;
			//	//	fprintf(fp, "%d, ", 5);
			//	//	sum2++;
			//	//}
			//	//else
			//	fprintf(fp, "%d, ", (int)result[0][i]);
			//	if (i % 32 == 31) {
			//		fprintf(fp, "\n");
			//	}
			//}
			////fprintf(fp, "%d : %d",(int)sum,(int)sum2);
			//fprintf(fp, "\n\n");

			//if (Q == 10) {
			//for (i = 0; i < 1024; i++) {
			//	fprintf(fp2, "%2d, ", (int)result_ica[0][i]);
			//	if (i % 32 == 31) {
			//		fprintf(fp2, "\n");
			//	}
			//	//fprintf(fp2, "%4.2lf : %4.2lf  -->  %4.2lf", best_dct[i], best_ica[i], best_dct[i] - best_ica[i]);
			//	//fprintf(fp2, "\n");
			//}
			////}

			for (j = 0; j < 1024; j++) {
				for (i = 0; i < 64; i++) {
					ny[i][j] = y[i][j];
				}
			}

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					// .val -> 値を取得・属性を変更し記憶
					// .abs -> 絶対値を記憶
					// .num -> 元々の係数に対応するブロック内番号を記憶
					sort_d[i][j].val = ny[i][j];		/* 元々の係数値 */
					sort_d[i][j].abs = fabs(ny[i][j]);	/* ソートは係数の絶対値で行う*/
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


			for (i = 0; i < 1024; i++)
				count[i] = 0;

			for (m = 10; m < 11; m++) {
				sprintf(g, "mkdir OUTPUT\\coe\\coefficient[%d]", m);
				system(g);

				for (a = 0; a < 256; a++)
					for (b = 0; b < 256; b++) {
						temp_sai[a * 256 + b] = origin[a][b];
					}

				for (i = 0; i < 64; i++)
					temp_temp[i] = 99;

				c = 0; ml = 0;

				for (i = 0; i < 1024; i++) {
					for (j = 0; j < 64; j++) {
						if (result_ica[0][i] == sort_d[j][i].num) {
							max = sort_d[j][i].num;
							//fprintf(fp3, "%2d, ", j);
							//count[j]++;
							k = i % 32;
							l = i / 32;
							c = 0;
							if (j <= m) {
								for (b = 0; b < 8; b++)
									for (a = 0; a < 8; a++)
										temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;

								/*for (mk = 0; mk < ml + 1; mk++) {
									if (temp_temp[mk] == max)
										c++;
								}
								if (0 == c) {
									ICA_temp(origin, pcaStr, y, w, avg, 100, 0.002, m, (int)max);
									temp_temp[ml] = (int)max;
									ml++;
								}*/

							}
						}
					}
					//if (i % 32 == 31) {
					//	fprintf(fp3, "\n");
					//}
				}

				for (i = 0; i < 64; i++)
					count3[i] = 0;

				/*for (i = 0; i < 1024; i++) {
					for (j = 0; j < 64; j++) {
						if (result_ica[0][i] == sort_d[j][i].num) {
							max = sort_d[j][i].num;
							fprintf(fp3, "%2d, ", j);
							count3[j]++;
						}
					}
					if (i % 32 == 31) {
						fprintf(fp3, "\n");
					}
				}*/

				//gnuplot2(count3);

					//sprintf(output, "coe/coefficient[%d]/coefficient[%d].bmp", m, m);
					//img_write_gray(temp_sai, output, 256, 256); // outputに出力画像を書き出す


					/*sprintf(g, "cp ICA\\ICA[%d] coe\\coefficient[]", m);
					system(g);*/


			}

			//fprintf(fp3, "\n");
			//for (j = 0; j < 64; j++)
			//	fprintf(fp3, " [%4d] (%3d) \n", j, count[j]);


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
			printf(" All finish");

}
