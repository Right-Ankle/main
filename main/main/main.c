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
	printf("%dbit exe\n", sizeof(void*) * 8);//対応プログラムの確認（32bit or 64bit）
	FILE* fp, * fp2, * fp3, * fp4, * fp5, * fp6, * fp7, * fp8, * fp9, * fp10; //ファイルポインタの定義

	/// メソッド化予定 ///

	////// char //////
	char g[50];//フォルダ作成用
	char yn;//実行処理の選択用
	char output[1000];//画像出力用
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
	static unsigned char origin_30[256][256] = { 0 };	//原画像（256*256のみ対応）

	static int temp_sai[256 * 256];


	static unsigned char nica_basis[64][64]; //ica基底変換用
	static unsigned char dcoe2[256][256] = { 0 }; //dct再構成用
	static unsigned char  ica_sai[256][256] = { 0 }; //ica再構成用
	static unsigned char block_ica[64] = { 0 }; //ica 小領域
	static unsigned char block_ica_temp[64] = { 0 }; //ica 小領域
	static unsigned char dct_ica_sai[256][256];

	////// int //////
	static int a, b, c, d, i, j, k, l, m, n, o, mk, ml; //計算用
	static int ori_temp[256 * 256] = { 0 }; // 原画像変換用
	static int no_op[1024] = { 0 }; // 小領域flag
	static int no_op_0[1024] = { 0 };
	static int no_op_1[1024] = { 0 };
	static int no_op_2[1024] = { 0 };
	static int no_op_3[1024] = { 0 };
	static int no_op_all[1024];
	static int no_op_ica[1024];
	static int Q;//圧縮レート
	static int QQ, QQQ, QQQQ;
	double min2 = 100000;
	double sum2 = 0;
	int hist[50000];
	int hist2[50000];
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
	int excel_temp = 4;
	double ica_dc[1024];
	double ica_basis_ent[64];
	double basis_temp[64];
	double ica_group[64][64];
	double ica_group_temp[64];
	double ica_group_result[64];
	double ica_group_num[64];
	double excel_basis[7];//0->ica基底，1->DCT単独，2->DCT領域，3->ICA領域，4->ICA領域数，5->ICAのDC "制限基底数=(1-2-5)/((3/4)+0)"
	double basis_limits[64];
	double ica_mse[1024];
	double dct_mse[1024];
	double psnr_temp2;
	static double comb[1024][64][2] = { 0 };//0->画質，1->情報量
	static double comb2[1024][64][64][2] = { 0 };//0->画質，1->情報量
	double comb_result3[64][64][64][3] = { 0 };//0->累積画質，1->累積情報量
	double comb_result2[64][64][3] = { 0 };//0->累積画質，1->累積情報量
	double comb_result[64][3] = { 0 };//0->累積画質，1->累積情報量
	double comb_sort3[64][64][64] = { 0 };
	double comb_sort2[64][64] = { 0 };
	double comb_sort[64] = { 0 };
	double comb_after_sort[100][6] = { 0 };//0->累積画質，1->累積情報量，2,3,4->基底番号（基底２の4番目は99)
	double comb_basis[64] = { 0 };// 0or1  0->基底を使ってない　1->基底を使っている


	////// double //////
	static double sum = 0, min = 0, max = 0;//計算用
	static double threshold = 0, percent = 0;//閾値で使用
	static double b_ica_ent[1024]; //各ica基底の情報量
	static double b_dct_ent[1024]; //各ica基底の情報量
	static double mse_dct[2][10][1024]; //mse
	static double full_mse[2][65][1024];
	static double full_ssim[2][65][1024];
	static double ssim_dct[2][10][1024];
	static unsigned char ssim_temp1[8][8];
	static unsigned char ssim_temp2[8][8];
	double dcoe[256][256] = { 0 }, ica_basis[65][1024], ica_basis2[65][1024];
	double avg[1024], y[64][1024], w[64][64], ny[64][1024], nny[64][1024], nnny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp[64][1024] = { 0 }, bunrui[4][1024];
	static double true_profit[64]; //１領域の改善量 - 係数の情報量 - DC情報量 = 真の利益
	static double mse_profit[1024];
	static double ent_profit[1024];
	static double basis_profit[1024];
	static double mse_sum[64];//基底ごとの画質の良さ
	static double ent_sum[64];//基底ごとの情報量の良さ
	static double basis0_ent = 0;

	//stract関数用
	static struct pca pcaStr = { 0 };


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

	//_mkdir("coe"); //原画像のブロック画像を拡大して出力
	//	for (j = 0; j < 1024; j++) {
	//	sprintf(g, "mkdir OUTPUT\\ICA\\%d", j);
	//	system(g);
	//}
	printf("mkdir end\n");
	i = 1;
	sum = -((i / (double)(1000)) * (log((i / (double)(1000))) / log(2)));
	//printf("\n\n%lf", sum);

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
	static char filename16[20] = { '3', '0', '.', 'b', 'm', 'p' };

	printf("\n******************\n 1, barbara\n 2, cameraman \n 3, mandrill \n 4, earth \n 5, Airplane \n 6, saiboat \n 7, boat \n 8, text \n 9, building \n ****************** \n\n filename plz .... : ");
	scanf("%d", &i);
	if (i == 2)
		strcpy(filename, filename2);
	else if (i == 3)
		strcpy(filename, filename15);
	else if (i == 4)
		strcpy(filename, filename14);
	else if (i == 5)
		strcpy(filename, filename3);
	else if (i == 6)
		strcpy(filename, filename6);
	else if (i == 7)
		strcpy(filename, filename9);
	else if (i == 8)
		strcpy(filename, filename13);
	else if (i == 9)
		strcpy(filename, filename10);

	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	/* 一次元配列から二次元配列に変換 */
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin[i][j] = ori_temp[i * 256 + j];

	strcpy(filename, filename16);
	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin_30[i][j] = ori_temp[i * 256 + j];

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

	//gnuplot(y);

	static double** xxx;
	xxx = (double**)malloc(sizeof(double*) * 64);

	// 構造体1つ1つにもメモリを確保
	for (i = 0; i < 64; i++)
		// 64個に1024バイト
		xxx[i] = (double*)malloc(sizeof(double) * 1024);

	gentox(origin, x); //256*256 -> 64*1024に変換

	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			xxx[j][i] = x[j][i];//原画像をコピー

	new_pre(xxx, avg, 1024);	//中心化 （xx -> 元の画素値 - 平均値, avg = ブロックごとの平均値）



	//printf("%lf", w[0][0]);
	/* histの初期化 */
	for (i = 0; i < 50000; i++) {
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
		printf("\n%d : [%lf]", i, avg[i]);
		//hist[(int)(x[i][n]) + 1]++;	//ステップ幅1
		hist2[(int)((avg[i] - min2)) + 1]++;	//ステップ幅1
	}

	/* エントロピーの計算 */
	for (i = 0; i < 50000; i++)
		if (hist2[i] > 0) {
			sum += -((hist2[i] / (double)(1024)) * (log((hist2[i] / (double)(1024))) / log(2)));
		}
	for (i = 0; i < 1024; i++)
		ica_dc[i] = sum / (1024 * 64);

	// ICA_BASIS 出力よう
	wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底
	//fprintf(fp2, "P5\n64 64\n255\n");
	//fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA基底出力, 64*64 0~255

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


		///////////////////////////////////////////////////////////////////////////////// テスト領域 //////////////////////////////////////////////////////////////////////////////////////////////////////
	//for (i = 0; i < 64; i++)
	//	printf("\n%d", ica_fre[i][0]);

	b_entropy_ica(y, b_ica_ent);
	step = 100;
	static double min3 = 0;
	double sum1 = 0;
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_ica_ent[i];
	sum /= 1024 * 64;
	//printf("\nica all  : %lf", sum); // icaブロックごとの情報量の総和

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



	//printf("\n------------------------dct_fre start--------");
	for (Q = 0; Q < 10; Q++) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 1024; j++) {
				dct_fre_temp[Q][i] += dct_fre[Q][i][j];
			}
		}
	}
	//gnuplot2_2(dct_fre_temp);
	//for (i = 0; i < 64; i++)
	//	printf("\n%d  %d  %d  %d", dct_fre_temp[9][i], dct_fre_temp[8][i], dct_fre_temp[7][i], dct_fre_temp[6][i]);

	Q = 100;

	//printf("\nnow Q = %d\n", Q);
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
			dct_ent[j][i] = 0;
			dct_ent[j][i] = sum1 / (1024 * 64);
		}

	}

	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++)
			ny[j][i] = y[j][i]; // ny -> yy(ica係数コピー)

	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	sprintf(output, "OUTPUT\\ICA_SAI_average.bmp"); //再構成画像bmpで出力
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			temp_sai[i * 256 + j] = (int)ica_sai[i][j];
	img_write_gray(temp_sai, output, 256, 256);


	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_dct_ent[i];
	sum /= 256 * 256;
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
			ica_ent[j][i] = sum1 / (64 * 1024);
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
	//printf("\n1y : %lf\n", sum1 / 64);

	for (a = 0; a < 1024; a++)
		b_ica_ent[a] = 0;

	b_entropy_ica(y, b_ica_ent);
	sum = 0;
	for (a = 0; a < 1024; a++)
		sum += b_ica_ent[a];
	sum /= 256 * 256;
	//printf("\n%lf", sum);

	//////////////////////////////////////////////////// ssim で優先度を付ける ////////////////////////////////////

		// 1 -> 64 までのssim調査

	fprintf(fp5, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp5, "\n\n  DCT vs ICA  \n\n    Area with a small number of basis\n  Number of basis used : 1 ~ 64 \n\n----------------------------------------------------------------------------------\n\n");

	for (j = 0; j < 1024; j++) {
		for (c = 0; c < 64; c++) { //MSE優先度の格納カウント

			threshold = 0;
			QQ = 99;

			for (n = 0; n < 64; n++) { //調査対象基底

				for (a = 0; a < 64; a++)
					ny[a][j] = y[a][j]; //係数の初期化

				if (c != 0) {
					for (a = 0; a < c; a++) {
						if ((int)full_ssim[0][a][j] != 99)
							ny[(int)full_ssim[0][a][j]][j] = 0; //選出済みの基底の係数を0
					}
				}

				if (ny[n][j] != 0) {
					ny[n][j] = 0; // 調査対象の基底の係数値を0

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
							ssim_temp1[a][b] = block_ica[a * 8 + b];
							ssim_temp2[a][b] = origin[ml * 8 + a][mk * 8 + b];
						}
					}

					sum = b_SSIM(ssim_temp2, ssim_temp1, 8, 8);

					if (threshold < sum) {//抜いた場合にssimが一番高くなる基底を抜く（一番いらない）
						threshold = sum;
						QQ = n;
					}
				}
			}
			full_ssim[1][c + 1][j] = threshold; //格納基底のMSE
			full_ssim[0][c][j] = (double)QQ; //0~63 いらない順で基底を格納
		}
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	//gnuplot(temp_array);

	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = y[n][j];

	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	for (j = 0; j < 1024; j++) {
		sum = 0.0;
		mk = j % 32;
		ml = j / 32;
		for (a = 0; a < 8; a++) {
			for (b = 0; b < 8; b++) {
				ssim_temp1[a][b] = (double)ica_sai[ml * 8 + a][mk * 8 + b];
				ssim_temp2[a][b] = (double)origin[ml * 8 + a][mk * 8 + b];
			}
		}
		full_ssim[1][0][j] = b_SSIM(ssim_temp2, ssim_temp1, 256, 256);//基底すべて用いた場合のMSE
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = 0;

	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

	for (j = 0; j < 1024; j++) {
		sum = 0.0;
		mk = j % 32;
		ml = j / 32;
		for (a = 0; a < 8; a++) {
			for (b = 0; b < 8; b++) {
				ssim_temp1[a][b] = (double)ica_sai[ml * 8 + a][mk * 8 + b];
				ssim_temp2[a][b] = (double)origin[ml * 8 + a][mk * 8 + b];
			}
		}
		full_ssim[1][64][j] = b_SSIM(ssim_temp2, ssim_temp1, 256, 256);//基底すべて用いた場合のMSE
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////








	///////////////////////////////////////////////////////////////////////////////// テスト領域 //////////////////////////////////////////////////////////////////////////////////////////////////////


	// 1 -> 64 までのMSE調査

	fprintf(fp6, "\n\n Use image  :  %s\n\n\n", filename);
	fprintf(fp6, "\n\n  DCT vs ICA  \n\n    Area with a small number of basis\n  Number of basis used : 1 ~ 64 \n\n----------------------------------------------------------------------------------\n\n");

	for (j = 0; j < 1024; j++) {
		for (c = 0; c < 64; c++) { //MSE優先度の格納カウント

			threshold = 10000;
			QQ = 99;

			for (n = 0; n < 64; n++) { //調査対象基底

				for (a = 0; a < 64; a++)
					ny[a][j] = y[a][j]; //係数の初期化

				if (c != 0) {
					for (a = 0; a < c; a++) {
						if ((int)full_mse[0][a][j] != 99)
							ny[(int)full_mse[0][a][j]][j] = 0; //選出済みの基底の係数を0
					}
				}

				if (ny[n][j] != 0) {
					ny[n][j] = 0; // 調査対象の基底の係数値を0

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
							sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2); //MSE
						}
					}

					if (threshold > sum / 64.0) {//MSEの減少が一番小さい基底を抜く
						threshold = sum / 64.0;
						QQ = n;
					}
				}
			}
			full_mse[1][c + 1][j] = threshold; //格納基底のMSE
			full_mse[0][c][j] = (double)QQ; //0~63 いらない順で基底を格納
		}
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	//gnuplot(temp_array);

	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = y[n][j]; 

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
		full_mse[1][0][j] = sum / 64;//基底すべて用いた場合のMSE
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");

	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = 0; 

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
		full_mse[1][64][j] = sum / 64;//基底使わない場合のMSE
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

	yn = 'n';
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
		for (Q = 70; Q > 0; Q -= 10) {
			printf("\r now Q is %d          \n", Q);


			//Q = 100;

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			idct(dcoe, dcoe2, 8); // 普通の再構成

			//for (i = 0; i < 1024; i++)
			//	no_op[i] = 1;
			//img_out(dcoe2, no_op, Q);

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
						ssim_temp1[a][b] = (double)dcoe2[ml * 8 + a][mk * 8 + b];
						ssim_temp2[a][b] = (double)origin[ml * 8 + a][mk * 8 + b];
					}
				}
				ssim_dct[0][(Q / 10) - 1][j] = b_SSIM(ssim_temp2, ssim_temp1, 8, 8);
				mse_dct[0][(Q / 10) - 1][j] = sum / 64;//平均

				//sum = sum / (256.0 * 256.0); mse_ica

				i = 0;

				for (b = 0; b < 64; b++)
					if (dcoe_temp[b][j] != 0)
						i++;

				mse_dct[1][(Q / 10) - 1][j] = (double)i;
				ssim_dct[1][(Q / 10) - 1][j] = (double)i;
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
					fprintf(fp6, "\n\n    ICA NUM : %2d\n\n    ICA mse : %lf\n", (int)bunrui[2][j], bunrui[3][j]);
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
				a = 1;
			}
			else if (yn == 'd')//ssimを用いた優先度による領域分割
			{
				for (j = 0; j < 1024; j++) {
					//for (a = 9; a > 0; a -= 1) {
					a = Q / 10 - 1; // Q = 30

					for (b = 0; b < 65; b++) {
						ica_basis[b][j] = 0;
						ica_basis2[b][j] = 99;
					}

					for (b = 0; b < 65; b++)
						if (ssim_dct[0][a][j] < full_ssim[1][b][j]) {
							bunrui[3][j] = full_ssim[1][b][j];
							bunrui[2][j] = 64.0 - b;
						}
					if (mse_dct[0][a][j] > full_ssim[1][0][j]) {
						bunrui[3][j] = full_ssim[1][0][j];
						bunrui[2][j] = 64.0;
					}

					bunrui[0][j] = ssim_dct[1][a][j];
					bunrui[1][j] = ssim_dct[0][a][j];

					if (bunrui[0][j] > bunrui[2][j] && bunrui[1][j] < bunrui[3][j]) {// 
						no_op[j] = 1; // no_op 1 ならica
						QQ++;

						if (bunrui[2][j] == 0)
							ica_basis[64][j] = 1; // 基底0
						else {
							for (b = 63; b > 63 - bunrui[2][j]; b--) {
								ica_basis[(int)full_ssim[0][b][j]][j] = 1;
								ny[(int)full_ssim[0][b][j]][j] = y[(int)full_ssim[0][b][j]][j];//重要な順で格納
								//printf("%d\n", (int)ica_basis[65-a][j]);
							}
						}
						//printf("%d\n", j);
						ica_basis2[64][j] = bunrui[2][j];

						for (b = 63; b > 63 - bunrui[2][j]; b--) {
							ica_basis2[(int)full_ssim[0][b][j]][j] = 1;
							nny[(int)full_ssim[0][b][j]][j] = y[(int)full_ssim[0][b][j]][j];
						}

					}
					else {
						ica_basis[64][j] = 2;
						for (b = 0; b < 64; b++)
							ica_basis[b][j] = 3;
					}
					fprintf(fp5, "\n\n -------------------- [ area No.%d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", j);
					fprintf(fp5, "\n\n    DCT NUM : %2d (%3d)\n\n    DCT ssim : %lf\n", (int)ssim_dct[1][a][j], (a + 1) * 10, ssim_dct[0][a][j]);
					fprintf(fp5, "\n\n    ICA NUM : %2d\n\n    ICA ssim : %lf\n", (int)bunrui[2][j], bunrui[3][j]);
				}

				fprintf(fp5, "\n\n -------------------- [ Rate %d ] ----------------------------------------------------------------------------------------------------------------------------------- \n\n\n", Q);
				fprintf(fp5, "\n\n    DCT : %d / 1024\n    ICA : %d / 1024\n", 1024 - QQ, QQ);
			}

			//img_out(origin, no_op, Q);
			//txt_out(bunrui, filename, Q);
			//txt_out2(ica_basis, filename, Q);
			//group(ica_basis2, filename, Q);
			//dct(origin, dcoe, 8); // 係数を出力
			//quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			//idct(dcoe, dcoe2, 8); // 普通の再構成
			//b_entropy_dct(dcoe);
			a = 0;
			b = 0;
			for (i = 0; i < 64; i++)
				basis_temp[i] = 0;
			for (i = 0; i < 1024; i++) 
				if (ica_basis2[64][i] != 99 && ica_basis2[64][i]!=0) {
					basis_temp[(int)ica_basis2[64][i]]++;
					b += (int)ica_basis2[64][i];
					a++;
				}
			printf("\n\n ICA_Block = %d, use basis average = %lf\n", a, (double)b / (double)a);

			gnuplot5(basis_temp, Q);

			//printf("\ncount:%d", a);
			//mp(y, avg, w, mpans);

			//segmentation_ent_out(origin, y, avg, w, mpans, block_flag, Q);	//領域分割

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
			//fprintf(fp7, "%lf,", sum);

			sum = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							sum += dct_ent[i][j];
					}
				}
			}
			//fprintf(fp7, "%lf,", sum);

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
			//fprintf(fp7, "%lf,", sum);
			//fprintf(fp7, "%lf,", (double)a);

			sum = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					sum += ica_dc[j];
				}
			}
			//fprintf(fp7, "%lf,", sum);
			sum = 0;
			for (i = 0; i < 64; i++)
				sum += test_basis[i];
			//fprintf(fp7, "%lf,", sum);
			sum = 0;
			for (j = 0; j < 1024; j++) {
				sum += test_area[j];
			}
			//fprintf(fp7, "%lf,", sum / a);

			//fprintf(fp7, ",=(B%d-C%d-F%d)/((D%d/E%d)+$B$2)", excel_temp, excel_temp, excel_temp, excel_temp, excel_temp);

			//fprintf(fp7, ",=(B%d-C%d-F%d)/((D%d/E%d)+$B$2)", excel_temp, excel_temp, excel_temp, excel_temp, excel_temp);


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

			seki5(nw, ny, x); // x -> nw * ny
			xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

			img_out2(dcoe2, ica_sai, no_op, Q + 4);
			img_out(origin, no_op, Q + 9);
			//情報量
			fprintf(fp7, "\n");
			fprintf(fp7, "%d,", Q);
			sum = 0;

			for (j = 0; j < 1024; j++) {
				for (i = 0; i < 64; i++) {
					if (dcoe_temp[i][j] != 0)
						sum += dct_ent2[i][j];
				}
			}

			fprintf(fp7, "%lf,", sum);

			sum = 0;
			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							sum += dct_ent2[i][j];
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
							sum += ica_ent2[i][j];
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

			for (i = 0; i < 256; i++)
				for (j = 0; j < 256; j++) {
					k = i / 8;
					m = j / 8;
					if (no_op[32 * k + m] == 0) {
						dct_ica_sai[i][j] = dcoe2[i][j];
					}
					else if (no_op[32 * k + m] == 1) {
						dct_ica_sai[i][j] = ica_sai[i][j];
					}
				}


			fprintf(fp7, "%lf,", sum / a);
			psnr_temp2 = psnr(origin, dct_ica_sai);
			fprintf(fp7, ",=(C%d+D%d+F%d)", excel_temp, excel_temp, excel_temp);
			fprintf(fp7, ",=(C%d+D%d+F%d+$B$2*G%d)", excel_temp, excel_temp, excel_temp, excel_temp);
			fprintf(fp7, ",%lf", psnr_temp2);
			fprintf(fp7, ",,=(B%d-J%d)", excel_temp, excel_temp);
			fprintf(fp7, ",,=(N%d/$B$2)", excel_temp);

			psnr_temp2 = psnr(origin, dcoe2);
			fprintf(fp7, ",,%lf", psnr_temp2);
			psnr_temp2 = SSIM(origin, dct_ica_sai, 256, 256);
			fprintf(fp7, ",,%lf", psnr_temp2);

			// / ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			excel_temp++;
		} // dctの最初に戻る

	}


	for (i = 0; i < 64; i++)
		free(xxx[i]);
	free(xxx);



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


	printf(" All finish");

	printf("\nPress any button...");
	getchar();
	getchar();
}