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

	FILE* fp, * fp2, * fp3, * fp4, * fp5, * fp6; //ファイルポインタの定義

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
	static unsigned char ddct[256][256];

	////// int //////
	static int a, b, c, d, i, j, k, l, m, n, mk, ml; //計算用
	static int mpans[1024][64] = { 0 }; // mp法による基底順序（各小領域 * 基底順序）
	static int block_flag[1024] = { 0 }; // 領域分割結果（先行手法）
	static int  y_rank[64][1024] = { 0 }; // 係数順位（係数順位 * 各小領域）
	static int ori_temp[256 * 256] = { 0 }; // 原画像変換用
	static int img_name = 0; // 出力画像名
	static int no_op[1024] = { 0 }; // 小領域flag
	static int Q;//圧縮レート
	static int QQ, QQQ, QQQQ, temp_sai2[64][1024] = { 0 }, semi[2][64] = { 0 };


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
	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp[64][1024] = { 0 }, all_mse[4][1024], bunrui[4][1024];

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

	/////////////////宣言処理 終了///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	yn = 'n';
	if (yn == 'y') {
		/////////////////////原画像とdct再構成との差分をica基底にする////////////////////////////////////////////////////
		for (i = 0; i < 1024; i++)
			block_flag[i] = 1;

		for (i = 0; i < 64; i++)
			for (j = 0; j < 64; j++)
				basis_mse[i][j] = 0;

		pcaStr = new_pca(origin);
		ICA(origin, pcaStr, y, w, avg, 100, 0.002);

		// ICA_BASIS_origin
		wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底
		fprintf(fp2, "P5\n64 64\n255\n");
		fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA基底出力, 64*64 0~255
		sprintf(output, "OUTPUT\\ICA_BASIS_origin.bmp"); //ICA基底bmpで出力
		for (i = 0; i < 64; i++)
			for (j = 0; j < 64; j++) {
				temp_basis[i * 64 + j] = nica_basis[i][j];
				origin_basis[i][j] = nica_basis[i][j];
			}
		img_write_gray(temp_basis, output, 64, 64);

		//////////// 本処理/////////////////////////////
		for (Q = 100; Q > 0; Q -= 10) {
			printf("\r now Q is %d          \n", Q);

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			idct(dcoe, dcoe2, 8); // 普通の再構成

			for (j = 0; j < 256; j++)
				for (i = 0; i < 256; i++)
					ddct[i][j] = 0;

			for (j = 0; j < 256; j++)
				for (i = 0; i < 256; i++) {
					a = origin[i][j];
					b = dcoe2[i][j];
					ddct[i][j] = (a - b) / 2 + 127;
				}
			//img_out(ddct, block_flag, Q);

			//差分をICA基底に
			pcaStr = new_pca(origin);
			ICA(ddct, pcaStr, y, w, avg, 100, 0.002);

			// ICA_BASIS 出力よう
			wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底
			fprintf(fp2, "P5\n64 64\n255\n");
			fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA基底出力, 64*64 0~255
			sprintf(output, "OUTPUT\\ICA_BASIS_%d.bmp", Q); //ICA基底bmpで出力
			for (i = 0; i < 64; i++)
				for (j = 0; j < 64; j++)
					temp_basis[i * 64 + j] = nica_basis[i][j];
			img_write_gray(temp_basis, output, 64, 64);

			for (i = 0; i < 64; i++)
				for (j = 0; j < 64; j++)
					diff_basis[(Q / 10) - 1][i][j] = nica_basis[i][j];

			//差分基底がorigin_basisと類似しているか？
			//結果→似てない（DCT成分が抜けているから似ない．より局所的な特徴が基底に現れる）
			m = 0;
			for (a = 0; a < 64; a += 8) {
				for (b = 0; b < 64; b += 8) {
					n = 0;
					for (c = 0; c < 64; c += 8)
						for (d = 0; d < 64; d += 8) {
							sum = 0;
							for (i = 0; i < 8; i++)
								for (j = 0; j < 8; j++)
									sum += pow((double)origin_basis[a + i][b + j] - (double)nica_basis[c][d], 2);
							basis_mse[m][n] = sum / 64;//origin基底と差分基底の類似度（すべての基底で照合）
							if (sum / 64 < 100)
								printf("\n [%d:%d]", m, n);
							n++;
						}
					m++;
				}
			}

		}

		//差分基底同士は類似しているか？
		//結果→似てない（レートごとにｄｃｔでは足りない部分の大きさが変わってくるから似ない）
		for (c = 0; c < 10; c++) {
			m = 0;
			for (a = 0; a < 64; a += 8) {
				for (b = 0; b < 64; b += 8) {
					for (d = 0; d < 10; d++) {
						sum = 0;
						for (i = 0; i < 8; i++)
							for (j = 0; j < 8; j++)
								sum += pow((double)diff_basis[c][a + i][b + j] - (double)diff_basis[d][a + i][b + j], 2);
						diff_basis_mse[c][d][m] = sum / 64;//レートごとの差分基底の類似度
					}
					m++;
				}
			}
		}
	}
	///////////////////////////原画像とdct再構成との差分をica基底にする fin ////////////////////////////////////////////

	// ///////////////////////// ica //////////////////////////////////
	// ICA基底・係数
	// origin = 画素値(256*256),  y = ica係数(ブロックで64個で1024ブロック分),  w = ica基底(64個の計算法の中にそれぞれ64個の計算法がある)
	// ICAに"origin"を入れることで"y"(計算後の値)と"w"(計算の仕方)の結果が出力される
	// 基底は計算方法。係数は 8*8の画素ブロックを構成するのに 64個の基底がそれぞれ どれくらい使われているのか（含まれているか）の値。
	// ブロックとは 256*256画素のうち縦8横8のブロック。一画像につき(256/8) 32*32 = 1024ブロック
	pcaStr = new_pca(origin);
	ICA(origin, pcaStr, y, w, avg, 100, 0.002);

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


	/////////////////////////////////////////////////////////////////////////////////////////// test
	b_entropy_ica(y, b_ica_ent);
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_ica_ent[i];
	sum /= 1024;
	printf("\n%lf", sum);

	Q = 80;

	dct(origin, dcoe, 8); // 係数を出力
	quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
	idct(dcoe, dcoe2, 8); // 普通の再構成

	seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
	xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①

	b_entropy_dct(dcoe, b_dct_ent);
	sum = 0;
	for (i = 0; i < 1024; i++)
		sum += b_dct_ent[i];
	sum /= 256*256;
	//printf("\n%lf", sum);

	ent_count(dcoe, avg);

	///////////////////////////// icaのtest /////////////////////
	double sum1 = 0;
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
	printf("\n%lf\n", sum1/64);
	


	///////////////////////////////////////////////
	
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
							ny[(int)full_mse[0][a+1][j]][j] = 0;

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

						if (threshold > sum / 64.0) {
							threshold = sum / 64.0;
							QQ = n;
						}

					}
				}
				full_mse[1][c+1][j] = threshold;
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
		printf("Do you want to run the MSE or MP ? [ y/n ] : ");
		scanf("%s", &yn);
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

					if (bunrui[0][j] > bunrui[2][j] && bunrui[1][j] > bunrui[3][j]) {
						no_op[j] = 1;
						QQ++;

						if (bunrui[2][j] == 0)
							ica_basis[64][j] = 1; // 基底0
						else {
							for (b = 63; b > 63 - bunrui[2][j]; b--) {
								ica_basis[(int)full_mse[0][b][j]][j] = 1;
								ny[(int)full_mse[0][b][j]][j] = y[(int)full_mse[0][b][j]][j];
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

				ent_out(origin, y, avg, w, ny, no_op, Q);

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
			else if(yn == 'd')
			{
				ent_out(origin, y, avg, w, ny, no_op, Q);
			}
				//img_out(origin, no_op, (a + 1) * 10);
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

				//segmentation_RD_single(origin, y, avg, w, mpans, block_flag, Q);
				//for (i = 0; i < 1024; i++)
				//	block_flag[i] = 0;
			//b_entropy_ica(ny);
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
