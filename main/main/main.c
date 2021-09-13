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
	double**** comb3_0; //0->画質，1->情報量
	double**** comb3_1;//0->画質，1->情報量
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
	double mse_dct[2][10][1024]; //mse
	double full_mse[2][65][1024];
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


	//printf("%lf", w[0][0]);
	/* histの初期化 */
	for (i = 0; i < 10000; i++) {
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
			dct_ent[j][i] = sum1 / (1024 * 64);
		}

	}



	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②



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

				if (c != 0)
					for (a = 0; a < c; a++)
						ny[(int)full_mse[0][a][j]][j] = 0; //選出済みの基底の係数を0

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
		full_mse[1][0][j] = sum / 64;//基底すべて用いた場合のMSE
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
		for (Q = 100; Q > 0; Q -= 10) {
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
			a = 0;
			//for (i = 0; i < 1024; i++) {
			//	if (ica_basis2[64][i] != 99) {
			//		printf("\n %lf", ica_basis2[64][i]);
			//		a++;
			//	}
			//}
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
						if (no_op[j] == 1)
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

								for (b = 0; b < 64; b++)
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

			QQQ = (int)(excel_basis[1] - excel_basis[2] - excel_basis[5]) / ((excel_basis[3] / excel_basis[4]) + excel_basis[0]);//基底制限数
			//printf("\n basis_limits = %d", QQQ);

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
				ica_mse[i] = 0;
				dct_mse[i] = 0;
			}



			//////////////////////////////////////////////////////////////////////////////

			//gnuplot2_2(dct_fre_temp);

			for (j = 0; j < 1024; j++)
				no_op[j] = 0;

			for (j = 0; j < 1024; j++) {
				if (ica_basis2[64][j] == 0) {//mseだから低い方が画質高い
					no_op[j] = 1;
				}
			}
			basis0_ent = 0;
			for (j = 0; j < 1024; j++) {
				if (ica_basis2[64][j] == 0) {
					for (i = 0; i < 64; i++) {
						if (dcoe_temp[i][j] != 0)
							basis0_ent += dct_ent2[i][j];
					}
					basis0_ent -= ica_dc[j];
				}
			}
			printf("\nbasis_ent0 = %lf", basis0_ent);


			for (j = 0; j < 1024; j++)
				no_op[j] = 0;

			//// 基底数1の全領域にフラグ
			//for (j = 0; j < 1024; j++) { //基底数が1の領域のみ有効 
			//	if (ica_basis2[64][j] == 1) {
			//		no_op[j] = 1;
			//	}
			//}

			for (j = 0; j < 1024; j++) {
				if (ica_basis2[64][j] == 1) {//mseだから低い方が画質高い
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

			block_mse(origin, ica_sai, ica_mse);
			block_mse(origin, dcoe2, dct_mse);



			for (a = 0; a < 64; a++)
				true_profit[a] = 0;

			for (i = 0; i < 64; i++) {
				zig[i] = 0;
				mse_sum[i] = 0;
				ent_sum[i] = 0;
			}

			for (i = 0; i < 1024; i++) {
				mse_profit[i] = 0;
				ent_profit[i] = 0;
				basis_profit[i] = 99;
			}

			sum = 0;
			sum2 = 0;

			for (j = 0; j < 1024; j++) {
				if (no_op[j] == 1) {
					mse_profit[j] = dct_mse[j] - ica_mse[j];
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

			//0の情報量ok


			//fprintf(fp9, "\n\n\n\n%d,area,psnr,basis,ent,*,*,*,*,*,*,*", Q);
			for (j = 0; j < 64; j++) {
				for (i = 0; i < 1024; i++) {
					if (no_op[i] == 1)
						if (j == (int)basis_profit[i]) {
							//fprintf(fp9, "\n,%d,%lf,%d,%lf", i, psnr_profit[i], (int)basis_profit[i], ent_profit[i]);
							mse_sum[j] += mse_profit[i];
							ent_sum[j] += ent_profit[i];
							zig[j]++;
						}
				}
			}
			//0の情報量ok

			//sum = 0;
			//fprintf(fp9, "\n\n\n\n%d,basis,psnr_ave,ent_ave,area_num",Q);
			//for (i = 0; i < 64; i++) {
			//	if (zig[i] != 0) {
			//		fprintf(fp9, "\n,%d,%lf,%lf,%d", i, psnr_sum[i], ent_sum[i], (int)zig[i]);
			//		c++;
			//	}
			//}

			//img_out2(dcoe2, ica_sai, no_op, Q + 8);

			// 画質の良さを基に基底を並び替え
			/////////////////////////////////////////////////////////////////////////////

			for (j = 0; j < 1024; j++) {
				no_op_0[j] = 0;
				no_op_1[j] = 0;
				no_op_2[j] = 0;
				no_op_3[j] = 0;
				no_op_all[j] = 0;
				no_op_ica[j] = 0;
				if (ica_basis2[64][j] != 99)
					no_op_ica[j] = 1;
				if (ica_basis2[64][j]!=99 && bunrui[0][j] - bunrui[2][j] >= 3 && bunrui[1][j] - bunrui[3][j]>=50) {//選出基底3津の時 
					//1vs2vs3個で画質比較
					if (ica_basis2[64][j] < 4)
						no_op_all[j] = 1;
					if (ica_basis2[64][j] == 3)
						no_op_3[j] = 1;
					if (ica_basis2[64][j] == 2)
						no_op_2[j] = 1;
					if (ica_basis2[64][j] == 1)
						no_op_1[j] = 1;
					if(ica_basis2[64][j] ==0)
						no_op_0[j] = 1;
				}
			}
			img_out(origin, no_op_0, Q);
			img_out(origin, no_op_1, Q + 1);
			img_out(origin, no_op_2, Q + 2);
			img_out(origin, no_op_3, Q + 3);
			img_out(origin, no_op_ica, Q + 5);
			img_out4(origin, no_op_ica, no_op_all, Q + 4);


			if (yn == 'n') {
				////////複数基底を見当中/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//0の情報量ok

				// 動的配列の宣言
				//comb3_0 = (double****)malloc(sizeof(double***) * 1024);
				//comb3_1 = (double****)malloc(sizeof(double***) * 1024);
				//for (a = 0; a < 1024; a++) {
				//	comb3_0[a] = (double***)malloc(sizeof(double**) * 64);
				//	comb3_1[a] = (double***)malloc(sizeof(double**) * 64);
				//	for (b = 0; b < 64; b++) {
				//		comb3_0[a][b] = (double**)malloc(sizeof(double*) * 64);
				//		comb3_1[a][b] = (double**)malloc(sizeof(double*) * 64);
				//		for (c = 0; c < 64; c++) {
				//			comb3_0[a][b][c] = (double*)malloc(sizeof(double) * 64);
				//			comb3_1[a][b][c] = (double*)malloc(sizeof(double) * 64);
				//		}
				//	}
				//}

				//0の情報量ok

				for (j = 0; j < 1024; j++)
					for (a = 0; a < 64; a++) {
						comb[j][a][0] = 100000;
						comb[j][a][1] = 0;
						for (b = 0; b < 64; b++) {
							comb2[j][a][b][0] = 100000;
							comb2[j][a][b][1] = 0;
							for (c = 0; c < 64; c++) {
								comb3_0[j][a][b][c] = 100000;
								comb3_1[j][a][b][c] = 0;
							}
						}
					}

				//基底1個で画質＋の領域の画質・情報改善を全探索で算出
				for (a = 0; a < 64; a++) {

					for (j = 0; j < 1024; j++)
						for (i = 0; i < 64; i++)
							ny[i][j] = 0; //係数初期化

					for (j = 0; j < 1024; j++) {

						for (i = 0; i < 64; i++)
							ny[i][j] = 0; //係数初期化

						if (ica_basis2[64][j] != 99 && ica_basis2[64][j] != 0) {//基底1個領域を対象

							ny[a][j] = y[a][j];

							for (m = 0; m < 64; m++)
								xx[m] = 0.0;

							seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
							xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
							avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

							sum = 0.0;
							mk = j % 32;
							ml = j / 32;

							// 64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
							// （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
							for (m = 0; m < 8; m++) {
								for (l = 0; l < 8; l++) {
									sum += pow(origin[ml * 8 + l][mk * 8 + m] - block_ica[l * 8 + m], 2);
								}
							}

							comb[j][a][0] = sum / 64;

							//情報量の調査
							sum = 0;

							for (l = 0; l < 64; l++) {
								if (dcoe_temp[l][j] != 0)
									sum += dct_ent2[l][j]; //dct単独
							}

							for (l = 0; l < 64; l++) {
								if (ny[l][j] != 0) {
									sum -= ica_ent2[l][j];//対象領域をICA_Block
								}
							}
							sum -= ica_dc[j];//ica_dc

							comb[j][a][1] = sum;
						}
					}
				}


				//基底2個で画質＋の領域の画質・情報改善を全探索で算出
				for (a = 0; a < 64 - 1; a++) {
					for (b = a + 1; b < 64; b++) {

						for (j = 0; j < 1024; j++)
							for (i = 0; i < 64; i++)
								ny[i][j] = 0; //係数初期化

						for (j = 0; j < 1024; j++) {

							for (i = 0; i < 64; i++)
								ny[i][j] = 0; //係数初期化

							if (ica_basis2[64][j] != 99 && ica_basis2[64][j] != 0) {//基底2個領域を対象

								ny[a][j] = y[a][j];
								ny[b][j] = y[b][j];

								for (m = 0; m < 64; m++)
									xx[m] = 0.0;

								seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
								xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
								avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

								sum = 0.0;
								mk = j % 32;
								ml = j / 32;

								// 64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
								// （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
								for (m = 0; m < 8; m++) {
									for (l = 0; l < 8; l++) {
										sum += pow(origin[ml * 8 + l][mk * 8 + m] - block_ica[l * 8 + m], 2);
									}
								}

								comb2[j][a][b][0] = sum / 64;

								//情報量の調査
								sum = 0;

								for (l = 0; l < 64; l++) {
									if (dcoe_temp[l][j] != 0)
										sum += dct_ent2[l][j]; //dct単独
								}

								for (l = 0; l < 64; l++) {
									if (ny[l][j] != 0) {
										sum -= ica_ent2[l][j];//対象領域をICA_Block
									}
								}
								sum -= ica_dc[j];//ica_dc

								comb2[j][a][b][1] = sum;
							}
						}
					}
				}


				//0dame

				//基底3個で画質＋の領域の画質・情報改善を全探索で算出

				for (a = 0; a < 64 - 2; a++) {
					for (b = a + 1; b < 64 - 1; b++) {
						for (d = b + 1; d < 64; d++) {

							for (j = 0; j < 1024; j++)
								for (i = 0; i < 64; i++)
									ny[i][j] = 0; //係数初期化

							for (j = 0; j < 1024; j++) {

								for (i = 0; i < 64; i++)
									ny[i][j] = 0; //係数初期化

								if (ica_basis2[64][j] != 99 && ica_basis2[64][j] != 0) {

									ny[a][j] = y[a][j];
									ny[b][j] = y[b][j];
									ny[d][j] = y[d][j];

									for (m = 0; m < 64; m++)
										xx[m] = 0.0;

									seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
									xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
									avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②

									sum = 0.0;
									mk = j % 32;
									ml = j / 32;

									//64個の2乗の平均からそのブロックが平均してどれくらい ずれているのかを見る
								   // （ちなみに、1ブロックにつき64パターン＊全1024ブロック）
									for (m = 0; m < 8; m++) {
										for (l = 0; l < 8; l++) {
											sum += pow(origin[ml * 8 + l][mk * 8 + m] - block_ica[l * 8 + m], 2);
										}
									}

									comb3_0[j][a][b][d] = sum / 64;

									//情報量の調査
									sum = 0;


									for (l = 0; l < 64; l++) {
										if (dcoe_temp[l][j] != 0)
											sum += dct_ent2[l][j]; //dct単独
									}

									for (l = 0; l < 64; l++) {
										if (ny[l][j] != 0) {
											sum -= ica_ent2[l][j];//対象領域をICA_Block
										}
									}
									sum -= ica_dc[j];//ica_dc


									comb3_1[j][a][b][d] = sum;

								}
							}
						}
					}
				}

				//累積（基底3つ）
				for (a = 0; a < 64 - 2; a++) {
					for (b = a + 1; b < 64 - 1; b++) {
						for (c = b + 1; c < 64; c++) {
							comb_result3[a][b][c][0] = 0;
							comb_result3[a][b][c][1] = 0;
							comb_result3[a][b][c][2] = 0;
							comb_sort3[a][b][c] = 0;
						}
					}
				}

				for (a = 0; a < 64 - 2; a++) {
					for (b = a + 1; b < 64 - 1; b++) {
						for (c = b + 1; c < 64; c++) {
							for (j = 0; j < 1024; j++) {
								//画質・情報量

								//2個
								max = comb2[j][a][b][0];//MSEだから小さい順
								k = a;
								l = b;
								if (max > comb2[j][b][c][0]) {
									k = b;
									l = c;
								}
								if (max > comb2[j][a][c][0]) {
									k = a;
									l = c;
								}
								//1個
								max = comb[j][a][0];//MSEだから小さい順
								m = a;
								if (max > comb[j][b][0])
									m = b;
								if (max > comb[j][c][0])
									m = c;

								//1vs2vs3個で画質比較
								if (comb3_0[j][a][b][c] < comb2[j][k][l][0] && comb3_0[j][a][b][c] < comb[j][m][0] && dct_mse[j] > comb3_0[j][a][b][c] && comb3_1[j][a][b][c]>-0.00001) {
									sum = dct_mse[j] - comb3_0[j][a][b][c];
									comb_result3[a][b][c][0] += sum;
									comb_result3[a][b][c][1] += comb3_1[j][a][b][c];
									comb_result3[a][b][c][2]++;
								}
								else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0] && comb2[j][k][l][1] > -0.00001) {
									sum = dct_mse[j] - comb2[j][k][l][0];
									comb_result3[a][b][c][0] += sum;//dctからの画質改善量を累積
									comb_result3[a][b][c][1] += comb2[j][k][l][1];
									comb_result3[a][b][c][2]++;
								}
								else if (dct_mse[j] > comb[j][m][0] && comb[j][m][1] > -0.00001) {
									sum = dct_mse[j] - comb[j][m][0];
									comb_result3[a][b][c][0] += sum;//dctからの画質改善量を累積
									comb_result3[a][b][c][1] += comb[j][m][1];
									comb_result3[a][b][c][2]++;
								}
							}
						}
					}
				}

				//累積（基底2つ）
				k = 0;
				for (a = 0; a < 64 - 1; a++) {
					for (b = a + 1; b < 64; b++) {
						comb_result2[a][b][0] = 0;
						comb_result2[a][b][1] = 0;
						comb_result2[a][b][2] = 0;
						comb_sort2[a][b] = 0;
					}
				}

				//累積（基底2つ）
				for (a = 0; a < 64 - 1; a++) {
					for (b = a + 1; b < 64; b++) {
						for (j = 0; j < 1024; j++) {
							//1個
							k = a;
							if (comb[j][a][0] > comb[j][b][0])
								k = b;

							//1vs2個の画質比較
							if (dct_mse[j] > comb2[j][a][b][0] && comb[j][k][0] > comb2[j][a][b][0] && comb2[j][a][b][1] > -0.00001) {
								//画質・情報量
								sum = dct_mse[j] - comb2[j][a][b][0];
								comb_result2[a][b][0] += sum;//dctからの画質改善量を累積
								comb_result2[a][b][1] += comb2[j][a][b][1];
								comb_result2[a][b][2]++;
							}
							else if (dct_mse[j] > comb[j][k][0] && comb[j][k][1] > -0.00001) {
								sum = dct_mse[j] - comb[j][k][0];
								comb_result2[a][b][0] += sum;
								comb_result2[a][b][1] += comb[j][k][1];
								comb_result2[a][b][2]++;
							}
						}
					}
				}

				//累積（基底1つ）
				for (a = 0; a < 64; a++) {
					comb_result[a][0] = 0;
					comb_result[a][1] = 0;
					comb_result[a][2] = 0;
					comb_sort[a] = 0;
				}

				for (a = 0; a < 64; a++)
					for (j = 0; j < 1024; j++)
						if (dct_mse[j] > comb[j][a][0] && comb[j][a][1] > -0.00001) {
							sum = dct_mse[j] - comb[j][a][0];
							comb_result[a][0] += sum;
							comb_result[a][1] += comb[j][a][1];
							comb_result[a][2]++;
						}



				//画質順でソート
				for (i = 0; i < 100; i++) {
					comb_after_sort[i][0] = 0;
					comb_after_sort[i][1] = 0;
					comb_after_sort[i][2] = 99;
					comb_after_sort[i][3] = 99;
					comb_after_sort[i][4] = 99;
					comb_after_sort[i][5] = 0;
				}

				// 0dame

				for (a = 0; a < 64 - 2; a++)
					for (b = a + 1; b < 64 - 1; b++)
						for (c = b + 1; c < 64; c++)
							comb_sort3[a][b][c] = comb_result3[a][b][c][0];//copy

				for (a = 0; a < 64 - 1; a++)
					for (b = a + 1; b < 64; b++)
						comb_sort2[a][b] = comb_result2[a][b][0];//copy

				for (a = 0; a < 64; a++)
					comb_sort[a] = comb_result[a][0];//copy


				for (i = 0; i < 10; i++) {//3個のソート
					max = 0;
					k = l = m = n = o = 99;
					for (a = 0; a < 64 - 2; a++)
						for (b = a + 1; b < 64 - 1; b++)
							for (c = b + 1; c < 64; c++)
								if (comb_sort3[a][b][c] > max) {//改善画質累積の比較のため，最大を探索
									max = comb_sort3[a][b][c];//3sort
									k = a;
									l = b;
									m = c;
								}
					if (k != 99) {
						comb_after_sort[i][0] = comb_result3[k][l][m][0];
						comb_after_sort[i][1] = comb_result3[k][l][m][1];
						comb_after_sort[i][2] = (double)k;
						comb_after_sort[i][3] = (double)l;
						comb_after_sort[i][4] = (double)m;
						comb_after_sort[i][5] = comb_result3[k][l][m][2];
						comb_sort3[k][l][m] = 0;//99番基底が入らないか？
					}
				}


				for (i = 10; i < 20; i++) {//２個のソート
					max = 0;
					k = l = m = n = o = 99;
					for (a = 0; a < 64 - 1; a++)
						for (b = a + 1; b < 64; b++)
							if (comb_sort2[a][b] > max) {//改善画質累積の比較のため，最大を探索
								max = comb_sort2[a][b];//2sort
								n = a;
								o = b;
							}
					if (n != 99) {
						comb_after_sort[i][0] = comb_result2[n][o][0];
						comb_after_sort[i][1] = comb_result2[n][o][1];
						comb_after_sort[i][2] = (double)n;
						comb_after_sort[i][3] = (double)o;
						comb_after_sort[i][4] = 99;
						comb_after_sort[i][5] = comb_result2[n][o][2];
						comb_sort2[n][o] = 0;//99番基底が入らないか？
					}
				}

				for (i = 20; i < 30; i++) {//1個のソート
					max = 0;
					k = l = m = n = o = 99;
					for (a = 0; a < 64; a++)
						if (comb_sort[a] > max) {//改善画質累積の比較のため，最大を探索
							max = comb_sort[a];//2sort
							n = a;
						}
					if (n != 99) {
						comb_after_sort[i][0] = comb_result[n][0];
						comb_after_sort[i][1] = comb_result[n][1];
						comb_after_sort[i][2] = (double)n;
						comb_after_sort[i][3] = 99;
						comb_after_sort[i][4] = 99;
						comb_after_sort[i][5] = comb_result[n][2];
						comb_sort[n] = 0;//99番基底が入らないか？
					}
				}




				//基底選出
				k = 0;
				c = 0;
				d = 0;
				sum = 0;
				for (a = 0; a < 30; a++) {
					//printf("\n %lf, %lf",comb_after_sort[a][0], comb_after_sort[a][1]);

					if (a >= 20)
						c = 1;
					else if (a >= 10 && a < 20)
						c = 2;
					else
						c = 3;

					if (k == 0) {
						if ((basis0_ent + comb_after_sort[a][1]) > (ica_basis_ent[0] * (double)c)) { // 基底0の改善情報量 + 基底１（対象基底）の改善情報量 + これまでの情報量 > 基底の情報量 * いくつ使っているか
							printf("\n [%d , %d, %d]  %lf + %lf >>  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], basis0_ent, comb_after_sort[a][1], ica_basis_ent[0] * (double)c, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							for (b = 0; b < 5; b++)
								comb_after_sort[0][b] = comb_after_sort[a][b];//選出基底以外全て初期化
							k++;
						}
						else {
							printf("\n [%d , %d, %d]  %lf + %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], basis0_ent, comb_after_sort[a][1], ica_basis_ent[0] * (double)c, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							comb_after_sort[a][0] = 0;//選出基底以外全て初期化
							comb_after_sort[a][1] = 0;//選出基底以外全て初期化
							comb_after_sort[a][2] = 99;//選出基底以外全て初期化
							comb_after_sort[a][3] = 99;//選出基底以外全て初期化
							comb_after_sort[a][4] = 99;//選出基底以外全て初期化
						}
					}
					else {
						printf("\n [%d , %d, %d]  %lf + %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], basis0_ent, comb_after_sort[a][1], ica_basis_ent[0] * (double)c, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
					}

					if (a == 9)
						printf("\n");
					if (a == 19)
						printf("\n");
				}

				//選出基底のうち，領域内で使用する基底を決定
				if (k == 1) {
					a = (int)comb_after_sort[0][2];
					b = (int)comb_after_sort[0][3];
					c = (int)comb_after_sort[0][4];
				}
				else {
					a = 99;
					b = 99;
					c = 99;
				}

				printf("\n%d, %d, %d", a, b, c);



				for (j = 0; j < 1024; j++) {
					no_op_0[j] = 0;
					for (i = 0; i < 64; i++)
						ny[i][j] = 0;
					no_op[j] = 0;
					if (ica_basis2[64][j] == 0) {
						no_op[j] = 1;
						no_op_0[j] = 1;
					}
				}


				for (j = 0; j < 1024; j++) {
					no_op_1[j] = 0;
					no_op_2[j] = 0;
					no_op_3[j] = 0;
					if (a != 99 && b != 99 && c != 99) {//選出基底3津の時
						//基底3で画質最大
						//2個
						max = comb2[j][a][b][0];//MSEだから小さい順
						k = a;
						l = b;
						if (max > comb2[j][b][c][0]) {
							k = b;
							l = c;
						}
						if (max > comb2[j][a][c][0]) {
							k = a;
							l = c;
						}
						//1個
						max = comb[j][a][0];//MSEだから小さい順
						m = a;
						if (max > comb[j][b][0])
							m = b;
						if (max > comb[j][c][0])
							m = c;

						//1vs2vs3個で画質比較
						if (comb3_0[j][a][b][c] < comb2[j][k][l][0] && comb3_0[j][a][b][c] < comb[j][m][0] && dct_mse[j] > comb3_0[j][a][b][c] && comb3_1[j][a][b][c]>-0.00001) {
							ny[a][j] = y[a][j];
							ny[b][j] = y[b][j];
							ny[c][j] = y[c][j];
							no_op[j] = 1;
							no_op_3[j] = 1;
						}
						else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0] && comb2[j][k][l][1] > -0.00001) {
							ny[k][j] = y[k][j];
							ny[l][j] = y[l][j];
							no_op[j] = 1;
							no_op_2[j] = 1;
						}
						else if (dct_mse[j] > comb[j][m][0] && comb[j][m][1] > -0.00001) {
							ny[m][j] = y[m][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
						}
					}
					else if (a != 99 && b != 99 && c == 99) {//選出基底2津の時
						//1個
						k = a;
						if (comb[j][a][0] > comb[j][b][0])
							k = b;

						//1vs2個の画質比較
						if (dct_mse[j] > comb2[j][a][b][0] && comb[j][k][0] > comb2[j][a][b][0] && comb2[j][a][b][1] > -0.00001) {
							//画質・情報量
							ny[a][j] = y[a][j];
							ny[b][j] = y[b][j];
							no_op[j] = 1;
							no_op_2[j] = 1;
						}
						else if (dct_mse[j] > comb[j][k][0] && comb[j][k][1] > -0.00001) {
							ny[k][j] = y[k][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
						}
					}
					else if (a != 99 && b == 99 && c == 99) {
						if (comb[j][a][0] < dct_mse[j] && comb[j][a][1] > -0.00001) {
							ny[a][j] = y[a][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
						}
					}

				}
				img_out(origin, no_op_0, Q);
				img_out(origin, no_op_1, Q+1);
				img_out(origin, no_op_2, Q+2);
				img_out(origin, no_op_3, Q+3);

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				seki5(nw, ny, x); // x -> nw * ny
				xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
				avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

				img_out2(dcoe2, ica_sai, no_op, Q + 4);

				//情報量
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


				fprintf(fp10, "%lf,", sum / a);
				psnr_temp2 = psnr(origin, dct_ica_sai);
				fprintf(fp10, ",=(C%d+D%d+F%d)", excel_temp, excel_temp, excel_temp);
				fprintf(fp10, ",=(C%d+D%d+F%d+$B$2*G%d)", excel_temp, excel_temp, excel_temp, excel_temp);
				fprintf(fp10, ",%lf", psnr_temp2);
				fprintf(fp10, ",,=(B%d-J%d)", excel_temp, excel_temp);
				fprintf(fp10, ",,=(N%d/$B$2)", excel_temp);
				excel_temp++;
				psnr_temp2 = psnr(origin, dcoe2);
				fprintf(fp10, ",,%lf", psnr_temp2);
				psnr_temp2 = SSIM(origin, dct_ica_sai, 256, 256);
				fprintf(fp10, ",,%lf", psnr_temp2);

				img_out(ica_sai, no_op, Q + 4);//全体のICA領域

				for (i = 0; i < 1024; i++) {
					for (j = 0; j < 64; j++)
						ny[j][i] = 0;
					no_op[i] = 0;
					if (ica_basis2[64][i] == 0) {
						no_op[i] = 1;
					}
				}
				seki5(nw, ny, x); // x -> nw * ny
				xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
				avg_inter(ica_sai, avg); // ica_sai -> 再構成済②
				//img_out2(dcoe2, ica_sai, no_op, Q + 2);
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
				psnr_temp2 = SSIM(origin, dct_ica_sai, 256, 256);
				fprintf(fp10, ",,%lf", psnr_temp2);



				for (a = 0; a < 1024; a++) {
					for (b = 0; b < 64; b++) {
						for (c = 0; c < 64; c++) {
							free(comb3_1[a][b][c]);
							free(comb3_0[a][b][c]);
						}
						free(comb3_1[a][b]);
						free(comb3_0[a][b]);
					}
					free(comb3_1[a]);
					free(comb3_0[a]);
				}
				free(comb3_1);
				free(comb3_0);

			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		} // dctの最初に戻る

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


	printf(" All finish");

	printf("\nPress any button...");
	getchar();
	getchar();
}