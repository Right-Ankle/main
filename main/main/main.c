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

	///宣言//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp, * fp2, * fp3, * fp4, * fp5, * fp6, * fp7, * fp8, *fp9, *fp10; //ファイルポインタの定義

	/// メソッド化予定 ///
	static int temp_basis[64 * 64]; // 基底出力用
	static int seg[64 * 64]; //単ブロック画像の出力用

	////// char //////
	char g[50];//フォルダ作成用
	char yn;//実行処理の選択用
	char output[1000];//画像出力用
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
	static unsigned char origin_basis[64][64]; //原画像のica基底
	static unsigned char diff_basis[10][64][64]; //差分画像のica基底
	static unsigned char nica_basis[64][64]; //ica基底変換用
	static unsigned char dcoe2[256][256] = { 0 }; //dct再構成用
	static unsigned char  ica_sai[256][256] = { 0 }; //ica再構成用
	static unsigned char block_ica[64] = { 0 }; //ica 小領域
	static unsigned char block_ica_temp[64] = { 0 }; //ica 小領域
	static unsigned char ddct[256][256];

	////// int //////
	static int a, b, c, d, i, j, k, l, m, n, mk, ml; //計算用
	static int mpans[1024][64] = { 0 }; // mp法による基底順序（各小領域 * 基底順序）
	static int block_flag[1024] = { 0 }; // 領域分割結果（先行手法）
	static int  y_rank[64][1024] = { 0 }; // 係数順位（係数順位 * 各小領域）
	static int ori_temp[256 * 256] = { 0 }; // 原画像変換用
	static int img_name = 0; // 出力画像名
	static int no_op[1024] = { 0 }; // 小領域flag
	static int op_test[1024] = { 0 };//flagのtest (基底1専用)各基底の最適領域
	static int Q;//圧縮レート
	static int QQ, QQQ, QQQQ, temp_sai2[64][1024] = { 0 }, semi[2][64] = { 0 };
	double min2 = 100000;
	double sum2 = 0;
	int hist[50000];
	int hist2[50000];
	double sum1_1 = 0;
	double sum2_2 = 0;
	int test_basis[64];
	int test_area[1024];
	double dct_ent[64][1024];
	double ica_ent[64][1024];
	double dct_ent2[64][1024];
	double ica_ent2[64][1024];
	double out_ent[10];
	int ica_fre[64][1024];//基底試用頻度
	int dct_fre[10][64][1024];//基底使用頻度
	int dct_fre_temp[10][64];
	int ica_fre_temp[64];
	int zig[64];
	int excel_temp=4;
	double ica_dc[1024];
	double ica_basis_ent[64];
	double basis_temp[64];
	double ica_group[64][64];
	double ica_group_temp[64];
	double ica_group_result[64];
	double ica_group_num[64];
	double ica_imp_basisnum = 0;
	double excel_basis[7];//0->ica基底，1->DCT単独，2->DCT領域，3->ICA領域，4->ICA領域数，5->ICAのDC "制限基底数=(1-2-5)/((3/4)+0)"
	double basis_limits[64];
	double ica_psnr[1024];
	double dct_psnr[1024];
	double using_ent[2][64]; //0->基底領域の情報量, 1->領域の情報量
	////// double //////
	static double temp_array[64][1024] = { 0 };//計算用配列
	static double sum = 0, min = 0, max = 0;//計算用
	static double threshold = 0, percent = 0;//閾値で使用
	static double basis_mse[64][64];//origin基底と差分基底の類似度（すべての基底で照合）
	static double diff_basis_mse[10][10][64];//レートごとの差分基底の類似度
	static double b_ica_ent[1024]; //各ica基底の情報量
	static double b_dct_ent[1024]; //各ica基底の情報量
	static double mse_dct[2][10][1024], mse_ica[64][1024], mse_ica0[64][1024], mse_ica1[64][1024]; //mse
	static double result_ica[2][1024], result_ica0[2][1024], result_mse[64][1024], y3[3][1024], full_mse[2][65][1024], mp_mse[2][65][1024];
	static double coe[256][256] = { 0 }, basis2[64][1024] = { 0 }, dcoe[256][256] = { 0 }, test2[64][1024], test3[64][1024], ica_test5[64][64][64], ica_test1[64][64], average2[1024], ica_basis[65][1024], ica_basis2[65][1024];
	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nny[64][1024], nnny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp[64][1024] = { 0 }, all_mse[4][1024], bunrui[4][1024];
	static double basis1_ent[64][2]; //最適基底数１の各基底の情報　0->情報量の改善量の累積，1->領域数
	static double true_profit[64]; //１領域の改善量 - 係数の情報量 - DC情報量 = 真の利益
	static double psnr_profit[1024];
	static double ent_profit[1024];
	static double basis_profit[1024];
	static double psnr_sum[64];//基底ごとの画質の良さ
	static double ent_sum[64];//基底ごとの情報量の良さ
	static double psnr_temp[64];//計算用
	static double sort_basis1[64];//基底1個で画質改善量の大きい基底から順に格納
	static double sort_basis_temp[64];//基底1個で画質改善量の大きい基底から順に格納
	static double basis0_ent=0;

	//stract関数用
	static struct pca pcaStr = { 0 };
	struct tt** sort_d, temp;
	double* temp_1;
	double* temp_2;
	double* temp_3;
	double* temp_4;
	double* temp_5;
	double* temp_6;


	// 出力用フォルダの初期化
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
	i = 1;
	sum = -((i / (double)(1000)) * (log((i / (double)(1000))) / log(2)));
	printf("\n\n%lf", sum);

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

	printf("\n (1, barbara  2, cameraman  3, mandrill  4, earth  5, Airplane)\n filename plz .... : ");
	scanf("%d", &i);
	if (i == 2)
		strcpy(filename, filename2);
	else if (i == 3)
		strcpy(filename, filename15);
	else if (i == 4)
		strcpy(filename, filename14);
	else if (i == 5)
		strcpy(filename, filename3);

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

	if ((fp7 = fopen("OUTPUT\\ica_basis_map.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp8 = fopen("OUTPUT\\ica_basis_result.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp9 = fopen("OUTPUT\\true_profit.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp10 = fopen("OUTPUT\\ICA_limits_result.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}



	/////////////////宣言処理 終了///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ///////////////////////// ica //////////////////////////////////
	// ICA基底・係数
	// origin = 画素値(256*256),  y = ica係数(ブロックで64個で1024ブロック分),  w = ica基底(64個の計算法の中にそれぞれ64個の計算法がある)
	// ICAに"origin"を入れることで"y"(計算後の値)と"w"(計算の仕方)の結果が出力される
	// 基底は計算方法。係数は 8*8の画素ブロックを構成するのに 64個の基底がそれぞれ どれくらい使われているのか（含まれているか）の値。
	// ブロックとは 256*256画素のうち縦8横8のブロック。一画像につき(256/8) 32*32 = 1024ブロック
	pcaStr = new_pca(origin);
	ICA(origin, pcaStr, y, w, avg, 100, 0.002);


	printf("%lf", w[0][0]);
	/* histの初期化 */
	for (i = 0; i < 100000; i++) {
		hist[i] = 0;
		hist2[i] = 0;
	}
	int step = 100;
	sum = 0;

	/* hist2の作成 */
	min2 = avg[0];
	for (i = 0; i < 1024; i++)
		if (avg[i] < min2)
			min2 = avg[i]; // histの左端

	for (i = 0; i < 1024; i++) {
		//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
		hist2[(int)((avg[i] - min2)) + 1]++;	//ステップ幅1
	}

	/* エントロピーの計算 */
	for (i = 0; i < 100000; i++)
		if (hist2[i] > 0) {
			sum += -((hist2[i] / (double)(1024)) * (log((hist2[i] / (double)(1024))) / log(2)));
		}
	for (i = 0; i < 1024; i++)
		ica_dc[i] = sum / (1024*64);

	// ICA_BASIS 出力よう
	wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底
	fprintf(fp2, "P5\n64 64\n255\n");
	fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA基底出力, 64*64 0~255
	sprintf(output, "OUTPUT\\ICA_BASIS_origin.bmp"); //ICA基底bmpで出力
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			temp_basis[i * 64 + j] = nica_basis[i][j];
	img_write_gray(temp_basis, output, 64, 64);

	// 計算用にコピー
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			ny[j][i] = y[j][i]; // ny -> yy(ica係数コピー)

	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			nw[j][i] = w[j][i]; // nw-> w(ica基底コピー)

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			for (Q = 0; Q < 10; Q++)
				dct_fre[Q][i][j] = 0;
			ica_fre[i][j] = 0;
		}
		for (Q = 0; Q < 10; Q++)
			dct_fre_temp[Q][i] = 0;
	}

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			if (y[i][j] != 0)
				ica_fre[i][j]++;
		}
		ica_fre_temp[i] = 0;
	}

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			ica_fre_temp[i] += ica_fre[i][j];
		}
	}

	//gnuplot2_2(ica_fre_temp);

	// ジグザグスキャンに変換////////////////////////////////////////////////////////////////////////////
	//l = 0;//ジグザグスキャンの順番
	//m = 0;//zig[]の進む順番
	//for (i = 0; i < 64; i++)
	//	zig[i] = 0;
	//for (i = 1; i < 4; i++) {
	//	for (k = 0; k < 1; k++) {
	//		zig[l] = m;
	//		l+=8;
	//		m++;
	//	}
	//	for (j = 0; j < 2 * i - 1; j++) {
	//		zig[l] = m;
	//		l -= 7;
	//		m++;
	//	}
	//	for (k = 0; k < 1; k++) {
	//		zig[l] = m;
	//		l += 1;
	//		m++;
	//	}
	//	for (j = 0; j < 2 * i; j++) {
	//		zig[l] = m;
	//		l += 7;
	//		m++;
	//	}
	//}
	//for (k = 0; k < 1; k++) {
	//	zig[l] = m;
	//	l+=8;
	//	m++;
	//}
	//for (j = 0; j < 7; j++) {
	//	zig[l] = m;
	//	l -= 7;
	//	m++;
	//}

	//for (i = 3; i > 0; i--) {
	//	for (k = 0; k < 1; k++) {
	//		zig[l] = m;
	//		l+=8;
	//		m++;
	//	}
	//	for (j = 0; j < 2 * i; j++) {
	//		zig[l] = m;
	//		l += 7;
	//		m++;
	//	}
	//	for (k = 0; k < 1; k++) {
	//		zig[l] = m;
	//		l += 1;
	//		m++;
	//	}
	//	for (j = 0; j < 2 * i - 1; j++) {
	//		zig[l] = m;
	//		l -= 7;
	//		m++;
	//	}
	//}
	//zig[l] = m;
	//l+=8;
	//m++;
	//zig[l] = m;
	//ジグザグスキャン終了//////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////// テスト領域 //////////////////////////////////////////////////////////////////////////////////////////////////////
	for (i = 0; i < 64; i++)
		printf("\n%d", ica_fre[i][0]);

	b_entropy_ica(y, b_ica_ent);
	step = 100.0;
	static double min3 = 0;
	double sum1 = 0;
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_ica_ent[i];
	sum /= 1024*64;
	printf("\nica all  : %lf", sum); // icaブロックごとの情報量の総和

	Q = 100;
	for (Q = 100; Q > 0; Q -= 10) {
		dct(origin, dcoe, 8); // 係数を出力
		quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる

		for (i = 0; i < 256; i += 8) {
			for (j = 0; j < 256; j += 8) {
				m = 0;
				for (k = 0; k < 8; k++) {
					for (l = 0; l < 8; l++) {
						x[m][n] = dcoe[i + k][j + l]; //256*256 -> 64*1024

						m++;
					}
				}
				n++;
			}
		}

		//fprintf(fp9, "\n\n");
		//for (i = 0; i < 64; i++) {
		//	if (i % 8 == 0)
		//		fprintf(fp9, "\n");
		//	fprintf(fp9, "%lf,", x[i][1]);
		//}

		for (i = 0; i < 64; i++)
			for (j = 0; j < 1024; j++) {
				if (x[i][j] != 0)
					dct_fre[(Q / 10 - 1)][i][j]++;
			}
	}



	printf("\n------------------------dct_fre start--------");
	for (Q = 0; Q < 10; Q++) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 1024; j++) {
				dct_fre_temp[Q][i] += dct_fre[Q][i][j];
			}
		}
	}
	//gnuplot2_2(dct_fre_temp);
	for (i = 0; i < 64; i++)
		printf("\n%d  %d  %d  %d", dct_fre_temp[9][i], dct_fre_temp[8][i], dct_fre_temp[7][i], dct_fre_temp[6][i]);

	Q = 100;

		printf("\nnow Q = %d\n", Q);
		dct(origin, dcoe, 8); // 係数を出力
		quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
		idct(dcoe, dcoe2, 8); // 普通の再構成

		b_entropy_dct(dcoe, b_dct_ent);

		for (i = 0; i < 256; i += 8) {
			for (j = 0; j < 256; j += 8) {
				m = 0;
				for (k = 0; k < 8; k++) {
					for (l = 0; l < 8; l++) {
						x[m][n] = dcoe[i + k][j + l]; //256*256 -> 64*1024

						m++;
					}
				}
				n++;
			}
		}

		for (j = 0; j < 64; j++) {

			for (i = 0; i < 50000; i++) {
				hist[i] = 0;
			}

			sum1 = 0;
			min3 = x[j][0];

			for (i = 0; i < 1024; i++)
				if (x[j][i] < min3)
					min3 = x[j][i]; // histの左端


			for (i = 0; i < 1024; i++) {
				//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
				hist[(int)((x[j][i] - min3)) + 1]++;	//ステップ幅1
			}

			for (i = 0; i < 50000; i++)
				if (hist[i] > 0) {
					sum1 += -((hist[i] / (double)(1024)) * (log((hist[i] / (double)(1024))) / log(2)));


				}
			for (i = 0; i < 1024; i++) {
				dct_ent[j][i] = sum1 / (1024*64);
			}

		}
	


	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②



	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_dct_ent[i];
	sum /= 256*256;
	//printf("\n%lf", sum);

	ent_count(dcoe, avg);


	/* histの初期化 */



	/* hist2の作成 */

	for (j = 0; j < 64; j++) {

		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
		}

		sum1 = 0;
		min3 = y[j][0];

		for (i = 0; i < 1024; i++)
			if (y[j][i] < min3)
				min3 = y[j][i]; // histの左端


		for (i = 0; i < 1024; i++) {
			//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
			hist[(int)((y[j][i] - min3) * step) + 1]++;	//ステップ幅1
		}

		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				sum1 += -((hist[i] / (double)(1024)) * (log((hist[i] / (double)(1024))) / log(2)));
			}
		for (i = 0; i < 1024; i++) {
			ica_ent[j][i] = sum1 / (64*1024);
		}

	}//基底ごとに比率を変える．

	/// 0コメのブロックの係数を全てゼロ//////////////////////////////






	///////////////////////////// icaのtest /////////////////////

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 1024; j++) {
			b = i;

			// 該当係数以外0
			// i番目の係数（基底）のみ使用。それ以外の係数は0。
			for (a = 0; a < 64; a++) {
				if (b == a)
					ny[a][j] = y[a][j];
				else
					ny[a][j] = 0;
			}


		}
		b_entropy_ica(ny, b_ica_ent);
		sum = 0;
		for (a = 0; a < 1024; a++)
			sum += b_ica_ent[a];
		sum /= 256 * 256;
		//printf("\n%lf", sum);
		sum1 += sum;
	}
	printf("\n1y : %lf\n", sum1/64);

	for (a = 0; a < 1024; a++)
		b_ica_ent[a] = 0;

	b_entropy_ica(y, b_ica_ent);
	sum = 0;
	for (a = 0; a < 1024; a++)
		sum += b_ica_ent[a];
	sum /= 256 * 256;
	printf("\n%lf", sum);

	


	///////////////////////////////////////////////////////////////////////////////// テスト領域 //////////////////////////////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////////////////

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
			basis2[i][j] = (double)sort_d[i][j].num;
			dcoe_temp[i][j] = sort_d[i][j].val;
		}

	// 生成画像確認
	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 64; i++)
			ny[i][j] = 0;

		ny[(int)basis2[0][j]][j] = y[(int)basis2[0][j]][j]; //一つ目の基底選択
		ny[(int)basis2[1][j]][j] = y[(int)basis2[1][j]][j]; // ２つ目の基底選択
	}

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
			ny[(int)y3[2][j]][j] = y[(int)y3[2][j]][j]; //  3つ目の基底選択
		}
		// 初期化（必ず行う）
		for (a = 0; a < 64; a++)
			xx[a] = 0.0;

		seki5(nw, ny, x); // x -> nw * ny
		xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
		avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

		// 画像出力
		img_name = "Basis3";
		for (i = 0; i < 1024; i++)
			no_op[i] = 1;
		img_out(ica_sai, no_op, img_name);

	}
	///////// 3 fin//////////////////
		//動作確認

	QQ = 0;
	QQQ = 0;
	QQQQ = 0;
	mk = 0;

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

		for (a = 0; a < 1024; a++) {
			no_op[a] = 1;
			if (test3[0][a] != i)
				no_op[a] = 0;
		}

		//sprintf((char)img_name, "ORIGIN/[%d].bmp", i); // 格納変数はcharを使用する
		//img_out(origin, no_op, img_name);

	}

			/// ////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////// /////////////////  各領域の最適基底・準最適基底の調査
		printf("\n");

		//printf(" ~ Investigation of Optimal basis ~\n What percentage do you use ? : ");
		//scanf("%lf", &percent);

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
		for (a = 0; a < 1024; a++) {
			no_op[a] = 1;
			if (no_op[j] != 0 || (result_ica[1][j] - dcoe_temp[0][j]) > 100.0)
				no_op[a] = 0;
		}

		//sprintf(img_name, "DCT/not_replaceable[%d].bmp", (int)percent);// 格納変数はcharにして
		//img_out(origin, no_op, img_name);

		//
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

			// 画像出力
			for (a = 0; a < 1024; a++) {
				no_op[a] = 1;
			}
			//sprintf(img_name, "test/[%d].bmp", QQ);// 格納変数はcharにして
			//img_out(ica_sai, no_op, img_name);

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

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					if (temp_sai2[i][j] == 0)
						average2[j]++;

			// 置換可能領域を画像で確認
			for (a = 0; a < 1024; a++) {
				no_op[a] = 1;
				if (average2[j] != 0)
					no_op[a] = 0;
			}

			//sprintf(img_name, "DCT/not_replaceable[%d].bmp", (int)percent);  // 格納変数はcharにして
			//img_out(origin, no_op, img_name);

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

			for (a = 0; a < 1024; a++) {
				no_op[a] = 1;
			}

			//sprintf(img_name, "MSE/[%d].bmp", (int)percent);
			//img_out(origin, no_op, img_name);

		}

		// 1 -> 64 までのMSE調査

		fprintf(fp6, "\n\n Use image  :  %s\n\n\n", filename);
		fprintf(fp6, "\n\n  DCT vs ICA  \n\n    Area with a small number of basis\n  Number of basis used : 1 ~ 64 \n\n----------------------------------------------------------------------------------\n\n");

		for (j = 0; j < 1024; j++) {

			for (b = 0; b < 1024; b++)
				for (a = 0; a < 64; a++)
					ny[a][b] = y[a][b];

			for (c = 0; c < 64; c++) {

				threshold = 10000;
				QQ = 0;
				//printf("a");

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

						if (threshold > sum / 64.0) {//MSEの減少が一番小さい基底を抜く
							threshold = sum / 64.0;
							QQ = n;
						}

					}
				}
				full_mse[1][c+1][j] = threshold;
				full_mse[0][c][j] = (double)QQ;//0~63 いらない順で基底を格納
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
				temp_array[i][j] = full_mse[1][i][j];
			}
			//gnuplot(temp_array);
		}

		for (j = 0; j < 1024; j++)
			for (n = 0; n < 64; n++)
			ny[n][j] = y[n][j]; // iつ目の基底選択

			seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	for (j = 0; j < 1024; j++) {
		sum = 0.0;
		mk = j % 32;
		ml = j / 32;
		for (a = 0; a < 8; a++) {
			for (b = 0; b < 8; b++) {
				sum += pow(origin[ml * 8 + b][mk * 8 + a] - ica_sai[ml * 8 + b][mk * 8 + a], 2);
			}
		}
		full_mse[1][0][j] = sum / 64;//平均
	}

		/*	printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
		}
		printf("\r [ Execution finished ]          ");
		printf("\n\n");*/



		//printf("What percentage do you use ? : ");
		//scanf("%lf", &percent);
		printf("\n");


	printf("<ica fin>\n\n");
	/////////////////////////////////ica 終了/////////////////////////////////////////


	// //////////////////////////// dct ////////////////////////////////////////
	// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

	//動作確認
	printf("<dct start>\n\n");
	//printf("Do you want to run the DCT ? [ y/n ] : ");
	//scanf("%s", &yn);

	for (i = 0; i < 1024; i++)
		block_flag[i] = 0;

	if (yn == 'n') {
		//printf("Do you want to run the MSE or MP ? [ y/n ] : ");
		//scanf("%s", &yn);
		yn = 'y';
		ent_count_basis(w, ica_basis_ent);
		excel_basis[0] = ica_basis_ent[0];
		fprintf(fp7, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp7, "\nQ,DCT_only,DCT_area,ICA_area,ICA_Num,ICA_DC,Basis_Type,Basis_Num,,Result");
		fprintf(fp8, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp8, "\nQ,DCT_only,DCT_area,ICA_area,ICA_Num,ICA_DC,Basis_Type,Basis_Num,,Result");
		fprintf(fp10, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp10, "\nQ,DCT_only,DCT_area,ICA_area,ICA_Num,ICA_DC,Basis_Type,Basis_Num,,No_Options,With_Options");
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


			//////////////////出力終了///////////////////////

			QQ = 0;
			int mp_count = 0;

			for (j = 0; j < 1024; j++) {
				no_op[j] = 0;
				for (i = 0; i < 4; i++)
					bunrui[i][j] = 0;
				for (i = 0; i < 64; i++)
					ny[i][j] = 0;
				for (i = 0; i < 64; i++)
					nny[i][j] = 0;
			}



			if (yn == 'y') {
				for (j = 0; j < 1024; j++) {
					//for (a = 9; a > 0; a -= 1) {
					a = Q / 10 - 1; // Q = 30

					for (b = 0; b < 65; b++) {
						ica_basis[b][j] = 0;
						ica_basis2[b][j] = 99;
					}

					for (b = 0; b < 65; b++)
						if (mse_dct[0][a][j] > full_mse[1][b][j]) {
							bunrui[3][j] = full_mse[1][b][j];
							bunrui[2][j] = 64.0 - b;
						}
					if (mse_dct[0][a][j] < full_mse[1][0][j]) {
						bunrui[3][j] = full_mse[1][0][j];
						bunrui[2][j] = 64.0;
					}

					bunrui[0][j] = mse_dct[1][a][j];
					bunrui[1][j] = mse_dct[0][a][j];

					if (bunrui[0][j] > bunrui[2][j] && bunrui[1][j] > bunrui[3][j]) {// 
						no_op[j] = 1; // no_op 1 ならica
						QQ++;

						if (bunrui[2][j] == 0)
							ica_basis[64][j] = 1; // 基底0
						else {
							for (b = 63; b > 63 - bunrui[2][j]; b--) {
								ica_basis[(int)full_mse[0][b][j]][j] = 1;
								ny[(int)full_mse[0][b][j]][j] = y[(int)full_mse[0][b][j]][j];//重要な順で格納
								//printf("%d\n", (int)ica_basis[65-a][j]);
							}
						}
						//printf("%d\n", j);
						ica_basis2[64][j] = bunrui[2][j];

						for (b = 63; b > 63 - bunrui[2][j]; b--) {
							ica_basis2[(int)full_mse[0][b][j]][j] = 1;
							nny[(int)full_mse[0][b][j]][j] = y[(int)full_mse[0][b][j]][j];
						}

					}
					else {
						ica_basis[64][j] = 2;
						for (b = 0; b < 64; b++)
							ica_basis[b][j] = 3;
					}
					fprintf(fp6, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
					fprintf(fp6, "\n\n    DCT NUM : %2d (%3d)\n\n    DCT mse : %lf\n", (int)mse_dct[1][a][j], (a + 1) * 10, mse_dct[0][a][j]);
					fprintf(fp6, "\n\n    ICA NUM : %2d\n\n    ICA mse : %lf\n", (int)bunrui[1][j], bunrui[0][j]);
				}

				fprintf(fp6, "\n\n -------------------- [ Rate %d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", Q);
				fprintf(fp6, "\n\n    DCT : %d / 1024\n    ICA : %d / 1024\n", 1024 - QQ, QQ);

				//ent_out(origin, y, avg, w, ny, no_op, Q);

				// // ICA_Blockの使用基底を画像出力
				//if(Q==40)
				//	for(j=0;j<1024;j++)
				//		if (no_op[j] == 1) {
				//			for (a = 0; a < 4096; a++)
				//				temp_basis[a] = 0;
				//				sprintf(output, "OUTPUT\\ICA_BASIS[%d].bmp", j); //ICA基底bmpで出力
				//				for (i = 0; i < 64; i++) {
				//					mk = i % 8;
				//					ml = i / 8;
				//					if (ny[i][j] != 0) {
				//						for (a = 0; a < 8; a++)
				//							for (b = 0; b < 8; b++)
				//								temp_basis[ml * 64 * 8 + mk * 8 + 64 * a + b] = nica_basis[ml * 8 + a][mk * 8 + b];
				//						img_write_gray(temp_basis, output, 64, 64);
				//					}
				//				}
				//		}
			}
			else if (yn == 'n')
			{

				// mp法の確認
				if (mp_count == 0) {

					mp(y, avg, w, mpans);

					for (j = 0; j < 1024; j++) {

						for (b = 0; b < 1024; b++)
							for (a = 0; a < 64; a++)
								ny[a][b] = y[a][b];

						for (a = 0; a < 65; a++) { // i番目

							if (a != 0) {
								for (n = 0; n < a; n++) {
									ny[mpans[j][63 - n]][j] = 0;
								}
							}

							// 初期化（必ず行う）
							for (b = 0; b < 64; b++)
								xx[b] = 0.0;

							seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
							xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
							avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

							sum = 0.0;
							mk = j % 32;
							ml = j / 32;

							for (c = 0; c < 8; c++) {
								for (b = 0; b < 8; b++) {
									sum += pow(origin[ml * 8 + b][mk * 8 + c] - block_ica[b * 8 + c], 2);
								}
							}

							mp_mse[1][a][j] = sum / 64.0;
							if (a == 64)
								mp_mse[0][64][j] = (double)99;
							else
								mp_mse[0][a][j] = (double)mpans[j][63 - a];
						}
					}
					mp_count++;
				}

				for (j = 0; j < 1024; j++) {
					//for (a = 9; a > 0; a -= 1) {
					a = Q / 10 - 1; // Q = 30

					for (b = 0; b < 65; b++) {
						ica_basis[b][j] = 0;
						ica_basis2[b][j] = 99;
					}

					for (b = 0; b < 65; b++)
						if (mse_dct[0][a][j] > mp_mse[1][b][j]) {
							bunrui[3][j] = mp_mse[1][b][j];
							bunrui[2][j] = 64.0 - b;
						}
					if (mse_dct[0][a][j] < mp_mse[1][0][j]) {
						bunrui[3][j] = mp_mse[1][0][j];
						bunrui[2][j] = 64.0;
					}

					bunrui[0][j] = mse_dct[1][a][j];
					bunrui[1][j] = mse_dct[0][a][j];

					if (bunrui[0][j] > bunrui[2][j] && bunrui[1][j] > bunrui[3][j]) {
						no_op[j] = 1;
						QQ++;

						if (bunrui[2][j] == 0)
							ica_basis[64][j] = 1; // 基底0
						else {
							for (b = 63; b > 63 - bunrui[2][j]; b--) {
								ica_basis[(int)mp_mse[0][b][j]][j] = 1;
								ny[(int)mp_mse[0][b][j]][j] = y[(int)mp_mse[0][b][j]][j];
								//printf("%d\n", (int)ica_basis[65-a][j]);
							}
						}
						//printf("%d\n", j);
						ica_basis2[64][j] = bunrui[2][j];

						for (b = 63; b > 63 - bunrui[2][j]; b--) {
							ica_basis2[(int)mp_mse[0][b][j]][j] = 1;
							nny[(int)mp_mse[0][b][j]][j] = y[(int)mp_mse[0][b][j]][j];
						}

					}
					else {
						ica_basis[64][j] = 2;
						for (b = 0; b < 64; b++)
							ica_basis[b][j] = 3;
					}
					fprintf(fp6, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
					fprintf(fp6, "\n\n    DCT NUM : %2d (%3d)\n\n    DCT mse : %lf\n", (int)mse_dct[1][a][j], (a + 1) * 10, mse_dct[0][a][j]);
					fprintf(fp6, "\n\n    ICA NUM : %2d\n\n    ICA mse : %lf\n", (int)bunrui[1][j], bunrui[0][j]);
				}

				fprintf(fp6, "\n\n -------------------- [ Rate %d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", Q);
				fprintf(fp6, "\n\n    DCT : %d / 1024\n    ICA : %d / 1024\n", 1024 - QQ, QQ);

				ent_out(origin, y, avg, w, ny, no_op, Q);
				img_out(origin, no_op, (a + 1) * 10);
			}
			else if (yn == 'd')
			{
				ent_out(origin, y, avg, w, ny, no_op, Q);
			}
			//img_out(origin, no_op, Q);
			//txt_out(bunrui, filename, Q);
			//txt_out2(ica_basis, filename, Q);
			//group(ica_basis2, filename, Q);
			//dct(origin, dcoe, 8); // 係数を出力
			//quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			//idct(dcoe, dcoe2, 8); // 普通の再構成
			//b_entropy_dct(dcoe);

			//mp(y, avg, w, mpans);

			//segmentation_ent_out(origin, y, avg, w, mpans, block_flag, Q);	//領域分割
			for (i = 0; i < 1024; i++)
				block_flag[i] = 0;

			for (j = 0; j < 1024; j++)
				op_test[j] = 0;

			for (j = 0; j < 1024; j++)
				op_test[j] = no_op[j];

			//segmentation_RD_single(origin, y, avg, w, mpans, block_flag, Q);
			//for (i = 0; i < 1024; i++)
			//	block_flag[i] = 0;
		//b_entropy_ica(ny);
		// 
				// ヒストグラム作成時　使用フラグのない領域はカウントしない////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (i = 0; i < 64; i++) // 使用基底種類数を調査
				test_basis[i] = 0;

			sum = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (ny[i][j] != 0) {
							out_ent[(Q / 10) - 1] += ica_ent[i][j];
						}
					}
				}
				else {
					for (i = 0; i < 64; i++) {
						out_ent[(Q / 10) - 1] += dct_ent[i][j];
					}
				}
			}

			k = 0;
			for (i = 0; i < 64; i++)
				if (test_basis[i] == 1)
					k++;
			//printf("\n%d\n", k);

			seki5(nw, ny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			//img_out2(dcoe2, ica_sai, no_op, Q);

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					ica_fre[i][j] = 0;
				}
			}
			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					if (no_op[j] == 1) {
						if (ny[i][j] != 0)
							ica_fre[i][j]++;
					}
				}
				ica_fre_temp[i] = 0;
			}

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					ica_fre_temp[i] += ica_fre[i][j];
				}
			}
			//gnuplot2(ica_fre_temp, Q);

			//for (i = 0; i < 64; i++) {
			//	if (i % 8 == 0)
			//		fprintf(fp7, "\n");
			//	fprintf(fp7, "%lf,", (double)ica_fre_temp[i]);
			//}

			fprintf(fp7, "\n");
			fprintf(fp7, "%d,", Q);
			sum = 0;



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
				for (i = 0; i < 64; i++) {
					if (dcoe_temp[i][j] != 0)
						sum += dct_ent[i][j];
				}
			}
			fprintf(fp7, "%lf,", sum);

			sum = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							sum += dct_ent[i][j];
					}
				}
			}
			fprintf(fp7, "%lf,", sum);

			sum = 0;
			a = 0;
			for (i = 0; i < 64; i++)
				test_basis[i] = 0;

			for (i = 0; i < 1024; i++)
				test_area[i] = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (ny[i][j] != 0) {
							sum += ica_ent[i][j];
							test_area[j]++;
							if (test_basis[i] == 0)
								test_basis[i] = 1;
						}
					}
					a++;
				}
			}
			fprintf(fp7, "%lf,", sum);
			fprintf(fp7, "%lf,", (double)a);

			sum = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					sum += ica_dc[j];
				}
			}
			fprintf(fp7, "%lf,", sum);
			sum = 0;
			for (i = 0; i < 64; i++)
				sum += test_basis[i];
			fprintf(fp7, "%lf,", sum);
			sum = 0;
			for (j = 0; j < 1024; j++) {
				sum += test_area[j];
			}
			fprintf(fp7, "%lf,", sum / a);

			fprintf(fp7, ",=(B%d-C%d-F%d)/((D%d/E%d)+$B$2)", excel_temp, excel_temp, excel_temp, excel_temp, excel_temp);


			//icaの情報量の比率を変更     比率=(Max差/Max) +1 　基底ごとの情報量=(全体の情報量*(比率/比率の総和))/1024

			max = 0;
			for (i = 0; i < 64; i++)
				if (ica_fre_temp[i] > max)
					max = ica_fre_temp[i];

			for (i = 0; i < 64; i++)
				basis_temp[i] = 0;

			for (i = 0; i < 64; i++)
				basis_temp[i] = ((max - ica_fre_temp[i]) / max) + 1;

			sum = 0;
			for (i = 0; i < 64; i++)
				sum += basis_temp[i];

			sum2 = 0;
			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					sum2 += ica_ent[i][j];

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					ica_ent2[i][j] = 0;

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++)
					ica_ent2[i][j] = (sum2 * (basis_temp[i] / sum) / 1024);
			}
			//.///////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//dctの情報量の比率を変更     比率=(Max差/Max) +1 　基底ごとの情報量=(全体の情報量*(比率/比率の総和))/1024
			max = 0;
			for (i = 0; i < 64; i++)
				if (dct_fre_temp[((Q / 10) - 1)][i] > max)
					max = dct_fre_temp[((Q / 10) - 1)][i];

			for (i = 0; i < 64; i++)
				basis_temp[i] = 0;

			for (i = 0; i < 64; i++)
				basis_temp[i] = ((max - dct_fre_temp[((Q / 10) - 1)][i]) / max) + 1;

			sum = 0;
			for (i = 0; i < 64; i++)
				sum += basis_temp[i];

			sum2 = 0;
			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					sum2 += dct_ent[i][j];

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					dct_ent2[i][j] = 0;

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++)
					dct_ent2[i][j] = (sum2 * (basis_temp[i] / sum) / 1024);
			}
			//.///////////////////////////////////////////////////////////////////////////////////////////////////////////////

			fprintf(fp8, "\n");
			fprintf(fp8, "%d,", Q);
			sum = 0;



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
				for (i = 0; i < 64; i++) {
					if (dcoe_temp[i][j] != 0)
						sum += dct_ent2[i][j];
				}
			}
			fprintf(fp8, "%lf,", sum);
			excel_basis[1] = sum;

			sum = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							sum += dct_ent2[i][j];
					}
				}
			}
			fprintf(fp8, "%lf,", sum);
			excel_basis[2] = sum;

			sum = 0;
			a = 0;
			for (i = 0; i < 64; i++)
				test_basis[i] = 0;

			for (i = 0; i < 1024; i++)
				test_area[i] = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (ny[i][j] != 0) {
							sum += ica_ent2[i][j];
							test_area[j]++;
							if (test_basis[i] == 0)
								test_basis[i] = 1;
						}
					}
					a++;
				}
			}
			fprintf(fp8, "%lf,", sum);
			fprintf(fp8, "%lf,", (double)a);
			excel_basis[3] = sum;
			excel_basis[4] = a;

			sum = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					sum += ica_dc[j];
				}
			}
			fprintf(fp8, "%lf,", sum);
			excel_basis[5] = sum;

			sum = 0;
			for (i = 0; i < 64; i++)
				sum += test_basis[i];
			fprintf(fp8, "%lf,", sum);
			sum = 0;
			for (j = 0; j < 1024; j++) {
				sum += test_area[j];
			}
			fprintf(fp8, "%lf,", sum / a);

			fprintf(fp8, ",=(B%d-C%d-F%d)/((D%d/E%d)+$B$2)", excel_temp, excel_temp, excel_temp, excel_temp, excel_temp);
			// / ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			// / /////////////////////////累積改善量で基底を制限中/////////////////////////////////////////////////

			seki5(nw, nny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			//img_out2(dcoe2, ica_sai, no_op, Q + 1);

			for (i = 0; i < 64; i++) {//初期化
				ica_group_temp[i] = 0;
				for (j = 0; j < 64; j++)
					ica_group[i][j] = 0;
			}

			for (i = 0; i < 64; i++) {
				QQQ = 0;
				for (j = 0; j < 1024; j++)
					if (no_op[j] == 1)
						if (ica_basis2[64][j] == i)
							QQQ++;
				ica_group_num[i] = QQQ;//基底数分類ごとの領域数
				ica_group_result[i] = 99;
			}
//			//調査中//////
//			for (i = 0; i < 64; i++)
//				zig[i] = 0;
//			for (i = 0; i < 64; i++)
//				zig[i] = (int)ica_group_num[i];
//			gnuplot2(zig, Q);
///// //////////

			for (i = 0; i < 64; i++) {
				for (j = 0; j < 1024; j++) {
					if (i == ica_basis2[64][j])
						for (a = 0; a < 64; a++)
							if (ica_basis2[a][j] == 1)
								ica_group[i][a] = 1;//基底数分類ごとの使用基底フラグ
				}
			}


			for (a = 0; a < 64; a++) {//基底数分類番号
				for (i = 0; i < 64; i++) {//対象基底
					sum = 0;
					QQQ = 0; //対象基底の領域数
					for (b = 0; b < 64; b++)
						ica_group_temp[i] = 0;

					for (j = 0; j < 1024; j++) {
						if(no_op[j]==1)
							if (ica_basis2[64][j] == a && nny[i][j] != 0) {

								for (b = 0; b < 64; b++)
									for (c = 0; c < 1024; c++)
										nnny[b][c] = nny[b][c];

								//対象基底を０
								nnny[i][j] = 0;

								// 初期化（必ず行う）
								for (b = 0; b < 64; b++)
									xx[b] = 0.0;

								//再構成
								seki5_Block(nw, nnny, xx, j); // xx64 -> nw * ny
								xtogen_Block(xx, block_ica_temp, avg, j); // ica_sai -> 再構成済①
								avg_inter_Block(block_ica_temp, avg, j); // ica_sai -> 再構成済②

								for (b = 0; b < 64;b++)
									for (c = 0; c < 1024; c++)
										nnny[b][c] = nny[b][c];

								// 初期化（必ず行う）
								for (b = 0; b < 64; b++)
									xx[b] = 0.0;

								seki5_Block(nw, nnny, xx, j); // xx64 -> nw * ny
								xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
								avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

								//画質
								sum2 = 0.0;

								for (c = 0; c < 8; c++) {
									for (b = 0; b < 8; b++) {
										sum2 += pow(block_ica_temp[b * 8 + c] - block_ica[b * 8 + c], 2);
									}
								}

								sum += (sum2 / 64);

								QQQ++;
							}
					}

					if (QQQ != 0)
						ica_group_temp[i] = sum / QQQ;//画質低下
				}

				sum = 0;
				sum2 = 0;
				for (i = 0; i < 64; i++)
					if (ica_group_temp[i] > sum) {
						sum = ica_group_temp[i];
						sum2 = (double)i;
					}
				if (sum == 0) {
					ica_group_result[a] = -1;//基底数分類で一番重要な基底番号
				}
				else {
					ica_group_result[a] = sum2;//基底数分類で一番重要な基底番号
				}

			}

			//調査中//////
			for (i = 0; i < 64; i++)
				zig[i] = 0;
			for (i = 0; i < 64; i++)
				zig[i] = (int)ica_group_result[i];
			//gnuplot2(zig, Q);
			/// //////////

			for (i = 0; i < 64; i++)
				basis_limits[i] = 99;

			QQQ = (excel_basis[1] - excel_basis[2] - excel_basis[5]) / ((excel_basis[3] / excel_basis[4]) + excel_basis[0]);//基底制限数
			printf("\n basis_limits = %d", QQQ);

			//for (i = 0; i < QQQ; i++) {
			//	while (basis_limits[i] == 99 && sum2!=99) {
			//		sum = 0;
			//		sum2 = 99;

			//		for (a = 0; a < 64; a++)
			//			if (ica_group_num[a] > sum) {
			//				sum = ica_group_num[a];//分類領域数が大きいやつ
			//				sum2 = (double)a;//分類番号
			//			}



			//		if (sum2 != 99) {//分類番号が99じゃない
			//			sum = 0;
			//			for (a = 0; a <= i; a++)//同じ基底が格納されていないか探索
			//				if (basis_limits[a] == ica_group_result[(int)sum2])
			//					sum = 1;

			//			if (sum == 0)
			//				basis_limits[i] = ica_group_result[(int)sum2];//制限下での使用基底を格納

			//			ica_group_num[(int)sum2] = 0;
			//			//printf("\n%lf", ica_group_result[(int)sum2]);
			//		}
			//	}
			//}
			
			for (i = 0; i < QQQ; i++) {
				b = 0;
				for (k = 0; k < 64; k++) {
					if (b == 0) {
						sum = 0;
						sum2 = 99;

						for (a = 1; a < 64; a++)//基底0個の分類を避けるためa=1から始める
							if (ica_group_num[a] > sum) {
								sum = ica_group_num[a];//分類領域数が大きいやつ
								sum2 = (double)a;//分類番号
							}

						if (sum2 != 99) {//分類番号が99じゃない
							sum = 0;
							for (a = 0; a <= i; a++)//同じ基底が格納されていないか探索
								if (basis_limits[a] == ica_group_result[(int)sum2])
									sum = 1;

							if (sum == 0) {
								basis_limits[i] = ica_group_result[(int)sum2];//制限下での使用基底を格納
								b = 1;
							}
							ica_group_num[(int)sum2] = 0;
							//printf("\n%lf", ica_group_result[(int)sum2]);
						}
					}
				}
			}




			////調査中//////
			//for (i = 0; i < 64; i++)
			//	zig[i] = 0;
			//for (i = 0; i < 64; i++)
			//	zig[i] = (int)basis_limits[i];
			//gnuplot2(zig, Q);
			///// //////////



			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					nnny[i][j] = 0;

			for (a = 0; a < 64; a++)
				for (i = 0; i < 64; i++)
					for (j = 0; j < 1024; j++)
						if (i == (int)basis_limits[a] && ica_basis2[64][j] <= a) {
							nnny[i][j] = y[i][j];
						}

			// 全てに適用
			seki5(nw, nnny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			//img_out2(dcoe2, ica_sai, no_op, Q + 2);

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++)
					nnny[i][j] = 0;

			//削減前に合わせる
			for (a = 0; a < 64; a++)
				for (i = 0; i < 64; i++)
					for (j = 0; j < 1024; j++)
						if (i == (int)basis_limits[a]) {
							nnny[i][j] = nny[i][j];
						}

			seki5(nw, nnny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			//img_out2(dcoe2, ica_sai, no_op, Q+3);


			//img_out(origin, no_op, Q);

			// ////////情報量の計算////////////

			seki5(nw, nnny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			for (i = 0; i < 1024; i++) {
				ica_psnr[i] = 0;
				dct_psnr[i] = 0;
			}



			//////////////////////////////////////////////////////////////////////////////

			//gnuplot2_2(dct_fre_temp);
			for (j = 0; j < 1024; j++)
				no_op[j] = 0;

			for (j = 0; j < 1024; j++) {
				if (ica_basis2[64][j] == 0 && mse_dct[0][(Q / 10) - 1][j] > full_mse[1][0][j]) {//mseだから低い方が画質高い
					no_op[j] = 1;
				}
			}
			basis0_ent = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							basis0_ent += dct_ent2[i][j];
					}
					basis0_ent -= ica_dc[j];
				}
			}



			for (j = 0; j < 1024; j++)
				no_op[j] = 0;

			//// 基底数1の全領域にフラグ
			//for (j = 0; j < 1024; j++) { //基底数が1の領域のみ有効 
			//	if (ica_basis2[64][j] == 1) {
			//		no_op[j] = 1;
			//	}
			//}

			for (j = 0; j < 1024; j++) {
				if (ica_basis2[64][j] == 1 && mse_dct[0][(Q / 10) - 1][j] > full_mse[1][0][j]) {//mseだから低い方が画質高い
					no_op[j] = 1;
				}
			}

			for (i = 0; i < 64; i++)
				for (j = 0; j < 1024; j++) {
					if (no_op[j] == 1)
						nnny[i][j] = nny[i][j];
					else
						nnny[i][j] = 0;
				}

			seki5(nw, nnny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			block_psnr(origin, ica_sai, ica_psnr);
			block_psnr(origin, dcoe2, dct_psnr);



			for (a = 0; a < 64; a++)
				true_profit[a] = 0;

			for (i = 0; i < 64; i++) {
				zig[i] = 0;
				psnr_sum[i] = 0;
				ent_sum[i] = 0;
			}

			for (i = 0; i < 1024; i++) {
				psnr_profit[i] = 0;
				ent_profit[i] = 0;
				basis_profit[i] = 99;
			}

			sum = 0;
			sum2 = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					psnr_profit[j] = ica_psnr[j] - dct_psnr[j];
				}
			}

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							ent_profit[j] += dct_ent2[i][j];
					}

					for (i = 0; i < 64; i++) {
						if (nnny[i][j] != 0) {
							ent_profit[j] -= ica_ent2[i][j];
							basis_profit[j] = i;
						}
					}
					ent_profit[j] -= ica_dc[j];
				}
			}


			//fprintf(fp9, "\n\n\n\n%d,area,psnr,basis,ent,*,*,*,*,*,*,*", Q);
			for (j = 0; j < 64; j++) {
				for (i = 0; i < 1024; i++) {
					if (no_op[i] == 1)
						if (j == (int)basis_profit[i]) {
							//fprintf(fp9, "\n,%d,%lf,%d,%lf", i, psnr_profit[i], (int)basis_profit[i], ent_profit[i]);
							psnr_sum[j] += psnr_profit[i];
							ent_sum[j] += ent_profit[i];
							zig[j]++;
						}
				}
			}

			sum = 0;
			fprintf(fp9, "\n\n\n\n%d,basis,psnr_ave,ent_ave,area_num",Q);
			for (i = 0; i < 64; i++) {
				if (zig[i] != 0)
					fprintf(fp9, "\n,%d,%lf,%lf,%d", i, psnr_sum[i] , ent_sum[i] , (int)zig[i]);
					c++;
			}

			// 画質の良さを基に基底を並び替え
			for (i = 0; i < 64; i++) {

				psnr_temp[i] = psnr_sum[i];
				sort_basis1[i] = 99;
			}

			for (a = 0; a < 64; a++) { //対象基底
				sum = 0;
				for (i = 0; i < 64; i++) {
					if (sum < psnr_temp[i]) {
						sort_basis1[a] = (double)i;
						sum = psnr_temp[i];
					}
				}
				psnr_temp[(int)sort_basis1[a]] = 0;
			}

			for (a = 0; a < 64; a++) {
				if (sort_basis1[a] != 99) {
					//printf("\n%d", (int)sort_basis1[a]);
					c++;
				}
			}

			for (i = 0; i < 64; i++) {
				sort_basis_temp[i] = sort_basis1[i];
			}

			sum = 0;
			c = 1;
			for (a = 0; a < 64; a++) {
				if (basis0_ent + ent_sum[(int)sort_basis1[a]] + sum > ica_basis_ent[0] * (double)c) { // 基底0の改善情報量 + 基底１（対象基底）の改善情報量 + これまでの情報量 > 基底の情報量 * いくつ使っているか
					sum += ent_sum[(int)sort_basis1[a]];
					c++;
				}
				else {
					sort_basis_temp[a] = 99;
				}
			}
			fprintf(fp4, "\n\n Q = %d", Q);
			for (a = 0; a < 64; a++) {
				if (sort_basis_temp[a] != 99) {
					printf("\n%d", (int)sort_basis_temp[a]);
					fprintf(fp4, "\n %d", (int)sort_basis_temp[a]);
				}
			}
			//基底１の結果
			for (j = 0; j < 1024; j++) {
				no_op[j] = 0;
			}
			for (a = 0; a < 64; a++) {
				for (j = 0; j < 1024; j++) {
					if (ica_basis2[64][j] == 0 || (ica_basis2[64][j] == 1 && basis_profit[j] == sort_basis_temp[a])) {//mseだから低い方が画質高い
						no_op[j] = 1;
					}
				}
			}

			img_out(origin, no_op, Q);




			//for (j = 0; j < 1024; j++) {
			//	no_op[j] = 0;
			//}
			//for (j = 0; j < 1024; j++) {
			//	if (ica_basis2[64][j] == 1 && mse_dct[0][(Q / 10) - 1][j] > full_mse[1][0][j]) {//mseだから低い方が画質高い
			//		no_op[j] = 1;
			//	}
			//}

			seki5(nw, nnny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②





			//img_out2(dcoe2, ica_sai, no_op, Q + 4);

			//基底0の情報量
			fprintf(fp10, "\n");
			fprintf(fp10, "%d,", Q);
			sum = 0;

			for (j = 0; j < 1024; j++) {
				for (i = 0; i < 64; i++) {
					if (dcoe_temp[i][j] != 0)
						sum += dct_ent2[i][j];
				}
			}

			fprintf(fp10, "%lf,", sum);

			sum = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							sum += dct_ent2[i][j];
					}
				}
			}
			fprintf(fp10, "%lf,", sum);



			sum = 0;
			a = 0;
			for (i = 0; i < 64; i++)
				test_basis[i] = 0;

			for (i = 0; i < 1024; i++)
				test_area[i] = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					for (i = 0; i < 64; i++) {
						if (nnny[i][j] != 0) {
							sum += ica_ent2[i][j];
							test_area[j]++;
							if (test_basis[i] == 0)
								test_basis[i] = 1;
						}
					}
					a++;
				}
			}
			fprintf(fp10, "%lf,", sum);
			fprintf(fp10, "%lf,", (double)a);
			excel_basis[3] = sum;
			excel_basis[4] = a;

			sum = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					sum += ica_dc[j];
				}
			}
			fprintf(fp10, "%lf,", sum);

			sum = 0;
			for (i = 0; i < 64; i++)
				sum += test_basis[i];
			fprintf(fp10, "%lf,", sum);
			sum = 0;
			for (j = 0; j < 1024; j++) {
				sum += test_area[j];
			}
			fprintf(fp10, "%lf,", sum / a);

			fprintf(fp10, ",=(C%d+D%d+F%d)", excel_temp, excel_temp, excel_temp);
			fprintf(fp10, ",=(C%d+D%d+F%d+$B$2*G%d)", excel_temp, excel_temp, excel_temp, excel_temp);
			fprintf(fp10, ",,=(B%d-J%d)", excel_temp, excel_temp);
			fprintf(fp10, ",,=(M%d/$B$2)", excel_temp);
			excel_temp++;

			img_out2(dcoe2, ica_sai, no_op, Q + 5);


		} // dctの最初に戻る
		printf("\r [ Execution finished ]          ");
		printf("\n\n");
	}





	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	fclose(fp7);
	fclose(fp8);
	fclose(fp9);
	fclose(fp10);
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

	printf("\nPress any button...");
	getchar();
	getchar();
}
