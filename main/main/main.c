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
	char name[20];
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
	static unsigned char origin_change[256][256] = { 0 }; //基底作成用画像（各レートの選出前のICA領域）
	static unsigned char nica_basis[64][64]; //ica基底出力用
	static unsigned char dcoe2[256][256] = { 0 }; //dct再構成用
	static unsigned char  ica_sai[256][256] = { 0 }; //ica再構成用
	static unsigned char block_ica[64] = { 0 }; //ica ブロックでの再構成用
	static unsigned char dct_ica_sai[256][256]; //DCT&ICA併用後の再構成

	////// int //////
	static int a, b, c, d, i, j, k, l, m, n, o, mk, ml; //計算用変数
	static int ori_temp[256 * 256] = { 0 }; // 原画像(256*256の一次元)
	static int ica_temp[256 * 256] = { 0 };
	static int no_op[1024] = { 0 }; // ブロックflag（基本的に0=DCT_Block, 1=ICA_Block）
	static int no_op_0[1024] = { 0 }; // フラグを立てたブロックのみ出力するためのやつ（使い捨て）
	static int no_op_1[1024] = { 0 };
	static int no_op_2[1024] = { 0 };
	static int no_op_3[1024] = { 0 };
	static int no_op_4[1024] = { 0 };
	static int no_op_5[1024] = { 0 };
	static int Block_flag[1024] = { 0 };
	static int Block_flag2[1024] = { 0 };
	static int Q;//圧縮レート
	static int QQ;//適当な変数として使用中？
	int hist[500000]; //entropy算出時のヒストグラム用
	double hist2[500000][2]; //entropy算出時のヒストグラム用 [][0]=係数値, [][1]=確率値（ラプラス）
	double hist3[500000][2]; //entropy算出時のヒストグラム用 [][0]=係数値, [][1]=確率値（ラプラス）
	int test_basis[64]; //どの基底が使用されているのか確認用？
	double test_area[1024]; // ブロック当たりの使用基底数の確認用？
	int ica_fre[64][1024]; //ICA基底頻度
	int ica_fre_temp[64]; //ICA基底毎の使用数
	int excel_temp = 4; // excel出力時に使用
	int step = 100; // ICA係数のentropy算出時に使用
	int dice_temp[2][10]; //基底順位をDICE係数で求める

	////// double //////
	static double sum = 0, min = 0, max = 0, temp = 0;//計算用
	double avg[1024];//ICAの直流成分
	double y[64][1024];//ICAの係数値（基本的にいじらない）
	double y2[64][1024];//ICAの係数値（基本的にいじらない）
	double w[64][64];//ICAの基底
	double ny[64][1024];//ICA_Blockの係数値、計算用
	double ny2[64][1024];//ICA_Blockの係数値、計算用
	double nny[64][1024];//ICA_Blockの係数値、計算用2
	double nw[64][64];//ICA基底、計算用
	double x[64][1024];//ICAの再構成時に使用、直流成分なし
	double xx[64];//ICAの再構成時に使用、直流なし（ブロックごと専用）
	static double mse_dct[2][10][1024]; //DCTのMSEとEntropy格納用、”dct_mse”と変わらん
		//ICAのMSEと基底番号格納用、[0=基底番号、1=MSE][0=0個、1=一番いらない基底、63=1番重要な基底、64=64個][ブロック番号]
	static double full_mse[2][65][1024];//↑
	static double full_mse_temp[2][65][1024];//↑
	double dct_ent[64][1024]; // DCTにおける各ブロック各基底のentropy
	double ica_ent[64][1024]; // ICAにおける各ブロック各基底のentropy
	double ica_ent2[64][1024]; // entropy操作後
	double dcoe[256][256] = { 0 };//DCTの係数
	double ica_basis2[65][1024];//領域分割後のICA_Blockにおける、使用基底と最適基底数　[0~63=フラグ、64=最適基底数][ブロック番号]
	double dcoe_temp[64][1024] = { 0 };//DCTの係数 64*1024版
	double bunrui[4][1024];//領域分割時に使用、[0=DCTの基底数、1=DCTのMSE、2=ICAの基底数、3=ICAのMSE][ブロック番号]
	double bunrui_temp[10][1024][2];
	double ica_dc[1024];  // ICAの直流成分のentropy
	double ica_w[64]; //ICA基底のentropy
	double ica_basis_ent[3]; //ICA基底のentropy
	double basis_temp[64]; //entropy操作時に使用（）
	double excel_basis[7];//0->ica基底，1->DCT単独，2->DCT領域，3->ICA領域，4->ICA領域数，5->ICAのDC "制限基底数=(1-2-5)/((3/4)+0)"（Excel用）
	double dct_mse[1024]; //DCTの各ブロックのMSE
	double dct_dct[1024];
	double coe_temp[64][1024] = { 0 };//
	double psnr_temp2; //PSNR出力用
	static double basis0_ent = 0;//0領域の改善可能なentropy用
		// 重要基底選出用 （無印＝MSEと情報量格納、Results＝各基底の評価、Sort＝画質順に並び替え）
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
	double dct_all_mse = 0;//dct全体のエントロピー
	double basis_ent[4];

	//stract関数用
	static struct pca pcaStr = { 0 };

	// 宣言終了.//////////////////////////////////////////////////////////////////////////////////


	// 出力用フォルダの初期化/////////////////////////////////////////////////////////////////////////
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
	_mkdir("OUTPUT\\Result"); //保存用のファイル格納用フォルダ
	_mkdir("OUTPUT\\Result\\test"); //MSE比較後、基底ごとの領域で分割した画像を出力
	_mkdir("OUTPUT\\Result\\test2"); //MSE比較後、基底ごとの領域で分割した画像を出力

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
	// フォルダ初期化 処理終了//////////////////////////////////////////////////////////////////////////

	//出力ファイル　宣言 ///////////////////////////////////////////////////////////////////////////
	if ((fp = fopen("OUTPUT\\Result\\detail.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp2 = fopen("OUTPUT\\fp2.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp3 = fopen("OUTPUT\\Result\\test2\\Higherbasis[4].csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp4 = fopen("OUTPUT\\Result\\test2\\Correlation.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp5 = fopen("OUTPUT\\Result\\test2\\DCT_hist.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp6 = fopen("OUTPUT\\Result\\DCT_MSE.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp7 = fopen("OUTPUT\\Result\\ica_basis_map.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp8 = fopen("OUTPUT\\Result\\test2\\ica_basis_result.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp9 = fopen("OUTPUT\\Result\\true_profit.txt", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	if ((fp10 = fopen("OUTPUT\\Result\\ICA_limits_result.csv", "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	/////////////////宣言処理 終了//////////////////////////////////////////////////////////////////////////////////////////////////////

	// 画像入力 処理 /////////////////////////////////////////////////////////////////
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
	static char filename16[20] = { '8', '9', '.', 'b', 'm', 'p' };

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

	fprintf(fp, "\n\n filename is [%s]\n\n", filename);

	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	/* 一次元配列から二次元配列に変換 */
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin[i][j] = ori_temp[i * 256 + j];

	//img_out3(origin);
	printf("a");
	/// 基底変更用//////////////////////////
	yn = 'n';
	if (yn == 'n') {
		strcpy(filename, filename16);
		if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
			return -1;

		for (i = 0; i < 256; i++)
			for (j = 0; j < 256; j++)
				origin_change[i][j] = ori_temp[i * 256 + j];
		fprintf(fp, "\n\n Change filename --> [%s]\n\n", filename);
	}
	///基底変更完了////////////////////////
	printf("a");

	///画像入力 処理終了//////////////////////////////////////////////////////////////////////////////////////////////

	//実験中///////////////////////////////////////////////////////////////
	// 使いにくさや、メイン手法との連携部分の改善がまだ
	//
	//入力画像のブロック番号をCSV出力
	yn = 'y';
	if (yn == 'n') {
		while (k == 0) {
			//ファイル名の入力
			printf("\n\n filename please: ");
			scanf("%s", &name);

			//入力画像のブロック番号をCSV出力
			//Block_count(name);

			//入力CSVファイルのブロック番号のみを画像出力
			mk_input_image(origin, name);

			//継続判定
			printf("\n\n continue ? (y:0/n:1) ");
			scanf("%d", &k);
		}
	}
	yn = 'n';
	printf("a");

	for (i = 0; i < 256; i += 8) {
		for (j = 0; j < 256; j += 8) {
			m = 0;
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {
					coe_temp[m][n] = 0;
					coe_temp[m][n] = origin_change[i + k][j + l]; //256*256 -> 64*1024
					m++;
				}
			}
			n++;
		}
	}
	a = 0;
	for (i = 0; i < 1024; i++)
		Block_flag2[i] = 9999;

	for (i = 0; i < 1024; i++) {
		sum = 0;
		for (j = 0; j < 64; j++) {
			sum += coe_temp[j][i];
		}
		if (sum != 0) {
			Block_flag[i] = 1;
			Block_flag2[a] = i;
			a++;
		}
		else
			Block_flag[i] = 0;
	}

	//相関係数出力
	fprintf(fp4, "\n\n\n,");
	for (b = 0; b < a; b++)
		fprintf(fp4, ",[%d]", Block_flag2[b]);
	fprintf(fp4, ",,0~0.2,0.2~0.4,0.4~0.7,0.7~1,,0~0.1");
	for (b = 0; b < a; b++) {
		fprintf(fp4, "\n,[%d]", Block_flag2[b]);
		k = l = m = n = d = 0;
		for (c = 0; c < a; c++) {
			sum = cor_coe2(origin, Block_flag2[b], Block_flag2[c]);
			if (fabs(sum) < 0.2)
				k++;
			else if (fabs(sum) >= 0.2 && fabs(sum) < 0.4)
				l++;
			else if (fabs(sum) >= 0.4 && fabs(sum) < 0.7)
				m++;
			else if (fabs(sum) > 0.7)
				n++;
			if (fabs(sum) < 0.1)
				d++;
			fprintf(fp4, ",%1.3lf", sum);
		}
		fprintf(fp4, ",[%d]", Block_flag2[b]);
		fprintf(fp4, ",%d,%d,%d,%d,,%d", k, l, m, n,d);
	}





	/// 基底作成による尖度を比較中////////////////////////////////////////////////////////////////////
	//printf("a");
	//pcaStr = new_pca(origin);
	//ICA(origin, pcaStr, y, w, avg, 100, 0.002);
	//if (yn == 'n') {
	//	pcaStr = new_pca(origin_change);
	//	ICA(origin_change, pcaStr, y2, w, avg, 100, 0.002);
	//}
	//printf("a");

	//for (j = 0; j < 1024; j++)
	//	for (n = 0; n < 64; n++) {
	//		ny[n][j] = 0;
	//		ny2[n][j] = 0;
	//	}

	//////////////// 係数の確率分布（ラプラス）を尖度で比較 ////////////////
	//// 画像全体でのラプラス分布
	//sum = temp = 0;
	//min = max = 0;
	//a = b = 0;
	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++) {
	//		if (y[j][i] != 0) {
	//			sum += y[j][i];
	//			a++;
	//		}
	//		if (y2[j][i] != 0) {
	//			temp += y2[j][i];
	//			b++;
	//		}
	//	}
	//sum = sum / (double)a;
	//temp = temp / (double)b;
	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++) {
	//		if (y[j][i] != 0)
	//			min += pow(y[j][i] - sum, 2);//分散

	//		if (y2[j][i] != 0) {
	//			max += pow(y2[j][i] - temp, 2);//分散
	//		}
	//	}

	//sum = sqrt(0.5 * (min / (double)a));// ラプラス分布のｂ（0.5*分散をルートしたやつ）
	//temp = sqrt(0.5 * (max / (double)b));
	//printf("%lf", sum);

	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++) {
	//		if (y[j][i] != 0)
	//			ny[j][i] = (1.0 / (2.0 * sum)) * exp(-1.0 * fabs(y[j][i]) / sum);
	//		if (y2[j][i] != 0)
	//			ny2[j][i] = (1.0 / (2.0 * temp)) * exp(-1.0 * fabs(y2[j][i]) / temp);
	//	}

	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++) {
	//		fprintf(fp9, "\n");
	//		if (y[j][i] != 0)
	//			fprintf(fp9, ",%lf,%lf", y[j][i], ny[j][i]);
	//		if (y2[j][i] != 0)
	//			fprintf(fp9, ",,%lf,%lf", y2[j][i], ny2[j][i]);
	//	}

	//fclose(fp9);
	//printf("a");
	//gnuplot5_3(y, ny, y2, ny2);
	//gnuplot5_4(y, y2);

	printf("a");
	////////////////////////////////実験 終了////////////////////////////////////////////

		// ///////////////////////// ica基底と係数を求める //////////////////////////////////
	// ICA基底・係数
	// origin = 画素値(256*256),  y = ica係数(ブロックで64個で1024ブロック分),  w = ica基底(64個の計算法の中にそれぞれ64個の計算法がある)
	// ICAに"origin"を入れることで"y"(計算後の値)と"w"(計算の仕方)の結果が出力される
	// 基底は計算方法。係数は 8*8の画素ブロックを構成するのに 64個の基底がそれぞれ どれくらい使われているのか（含まれているか）の値。
	// ブロックとは 256*256画素のうち縦8横8のブロック。一画像につき(256/8) 32*32 = 1024ブロック
	printf("a");
	if (yn == 'n') {
		pcaStr = new_pca(origin_change);
		ICA(origin_change, pcaStr, y, w, avg, 100, 0.002);
	}
	else {
		pcaStr = new_pca(origin);
		ICA(origin, pcaStr, y, w, avg, 100, 0.002);
	}

	printf("a");
	// ICA_BASIS 出力よう
	wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底

	// 計算用にコピー
	sum = 0;
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++) {
			ny[j][i] = y[j][i]; // ny -> yy(ica係数コピー)
			sum += y[j][i];
		}
	printf("\n%lf", sum);
	printf("a");

	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			nw[j][i] = w[j][i]; // nw-> w(ica基底コピー)

	//再構成確認
	seki5(nw, ny, x); // x -> nw * ny
	xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
	avg_inter(ica_sai, avg); // ica_sai -> 再構成済②
	sprintf(output, "OUTPUT\\Result\\ICA_SAI_main.bmp");
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			ica_temp[i * 256 + j] = ica_sai[i][j];
	img_write_gray(ica_temp, output, 256, 256);

	//for (j = 0; j < 1024; j++)
	//	for (n = 0; n < 64; n++) {
	//		ny[n][j] = 0;
	//		ny2[n][j] = 0;
	//	}


	sum = 0;

	/* ブロックごとの係数のエントロピーの計算 */

		/* histの初期化 */
		for (i = 0; i < 50000; i++) {
			hist[i] = 0;
		}

		/* histの作成 */
		min = 0;

		/* hist2の作成 */
		min = nw[0][0];
		for (j = 0; j < 64; j++)
			for (i = 0; i < 64; i++)
				if (nw[j][i] < min)
					min = nw[j][i]; // histの左端

		for (i = 0; i < 64; i++)
			for (j = 0; j < 64; j++)
				hist[(int)(nw[j][i] - min) + 1]++;//ステップ幅1


		sum = 0;
		/* エントロピーの計算 */
		for (i = 0; i < 50000; i++)
			if (hist[i] > 0) {
				sum += -((hist[i] / (double)(64 * 64)) * (log((hist[i] / (double)(64 * 64))) / log(2)));
			}
		printf("\n basis ent : %lf", sum);

		printf("a");

	// ICAの直流成分をQ100で代用　//////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (j = 0; j < 1024; j++) {
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
		ica_dc[j] = sum / 64.0;
		dct_mse[j] = avg[j];
	}

	Q = 90;
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
					if (m == 0) {
						avg[n] = dcoe[i + k][j + l] / 8;
						//printf("\n\n [%d] : %lf (%lf) diff = %lf ", n, dcoe[i + k][j + l], dcoe[i + k][j + l] / 8, (dcoe[i + k][j + l] / 8) - avg[n]);
					}
					m++;
				}
			}
			n++;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	min = 0;
	for (j = 0; j < 1024; j++) {
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
		//printf("\n\n [%d] : before %lf (%lf) , after %lf (%lf) diff = %lf ", j, ica_dc[j], dct_mse[j], sum / 64.0, avg[j], ica_dc[j] - (sum / 64.0));
		min += (ica_dc[j] - (sum / 64.0)) * (ica_dc[j] - (sum / 64.0));
	}
	//printf("\n\n diff = %lf", min / 1024);

	/////////////////  Step1  のメイン処理　//////////////////////////
	// 1 -> 64 までのMSE調査

	for (i = 0; i < 65; i++)
		for (j = 0; j < 1024; j++) {
			full_mse_temp[0][i][j] = 99;
			full_mse_temp[1][i][j] = 10000;
			full_mse[0][i][j] = 99;
			full_mse[1][i][j] = 10000;
		}

	for (j = 0; j < 1024; j++) {
		for (c = 0; c < 64; c++) { //MSE優先度の格納カウント

			min = 10000; // 最小MSE 保存用
			QQ = 99; //最小MSEの基底番号 保存用

			for (n = 0; n < 64; n++) { //調査対象基底

				for (a = 0; a < 64; a++)
					ny[a][j] = 0; //係数の初期化

				if (c != 0) {
					for (a = 0; a < c; a++) {
						if ((int)full_mse[0][a][j] != 99)
							ny[(int)full_mse[0][a][j]][j] = y[(int)full_mse[0][a][j]][j]; //選出済みの基底の係数を0
					}
				}

				if (ny[n][j] == 0) {
					ny[n][j] = y[n][j]; // 調査対象の基底の係数値を0

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

					if (min > sum / 64.0) {//MSEの減少が一番小さい基底を抜く探索
						min = sum / 64.0;
						QQ = n;
					}
				}
			}
			full_mse[1][c + 1][j] = min; //格納基底のMSE
			full_mse[0][c][j] = (double)QQ; //0~63 必要順で格納
		}
		printf("\r Now Running  :  [%3.3lf]", ((double)j / 1024.0) * 100);
	}
	printf("\r [ Execution finished ]          ");
	printf("\n\n");





	/// 基底を全て使ったときのMSE
	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = y[n][j];

	for (j = 0; j < 1024; j++) {
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
		full_mse[1][64][j] = sum / 64.0;//基底すべて用いた場合のMSE
	}
	/// 基底を全て使った時終了///////////


	/// 基底を0個使った時のMSE /////////
	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++)
			ny[n][j] = 0; // iつ目の基底選択

	for (j = 0; j < 1024; j++) {
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
		full_mse[1][0][j] = sum / 64.0;//基底すべて用いた場合のMSE
	}

	for (i = 0; i < 65; i++)
		for (j = 0; j < 1024; j++) {
			full_mse_temp[0][64 - i][j] = full_mse[0][i][j];
			full_mse_temp[1][64 - i][j] = full_mse[1][i][j];
		}

	for (i = 0; i < 65; i++)
		for (j = 0; j < 1024; j++) {
			full_mse[0][i][j] = full_mse_temp[0][i][j];
			full_mse[1][i][j] = full_mse_temp[1][i][j];
		}

	for (i = 0; i < 65; i++)
		printf(" \n%d : %d , %lf", i, (int)full_mse[0][i][0], full_mse[1][i][0]);
	printf(" \n");
	//for (j = 0; j < 1024; j++) {
	//	for (i = 0; i < 64; i++)
	//		basis_temp[i] = full_mse[1][i][j];
	//	gnuplot5(basis_temp, j);
	//}
	///基底を使わなかったとき　終了///////////////////
	/////////////////  Step1  のメイン処理　終了/////////////////////////////////////////////////////////////

	/*
	min = 10000; // 最小MSE 保存用
	for (i = 0; i < 64; i++)
		ny[i][QQ] = 0; //係数の初期化
	for (i = 0; i < 4; i++)
		excel_basis[i]=99;
	for(j=0;j<1024;j++){
		fprintf(fp8, "\n\n\n\n,[%d]", j);
		min = 10000; // 最小MSE 保存用
		for (k = 0; k < 3; k++) {
			min = 10000; // 最小MSE 保存用
			for (a = 0; a < 64 - k; a++) {

				if (k == 0) {
					for (i = 0; i < 64; i++)
						ny[i][j] = 0; //係数の初期化
					ny[a][j] = y[a][j];//選出済みの基底の係数を0
					for (i = 0; i < 64; i++)
						xx[i] = 0.0;
					seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
					xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
					avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
					sum = 0.0;
					mk = j % 32;
					ml = j / 32;
					for (l = 0; l < 8; l++)
						for (m = 0; m < 8; m++)
							sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE
					if (min > sum / 64.0) { //MSEの減少が一番小さい基底を抜く
						min = sum / 64.0;
						excel_basis[0] = a;
					}
				}

				if (k > 0)
					for (b = a + 1; b < 64; b++) {

						if (k == 1) {
							for (i = 0; i < 64; i++)
								ny[i][j] = 0; //係数の初期化
							ny[a][j] = y[a][j];//選出済みの基底の係数を0
							ny[b][j] = y[b][j];//選出済みの基底の係数を0
							for (i = 0; i < 64; i++)
								xx[i] = 0.0;
							seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
							xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
							avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
							sum = 0.0;
							mk = j % 32;
							ml = j / 32;
							for (l = 0; l < 8; l++)
								for (m = 0; m < 8; m++)
									sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE
							if (min > sum / 64.0) { //MSEの減少が一番小さい基底を抜く
								min = sum / 64.0;
								excel_basis[0] = a;
								excel_basis[1] = b;
							}
						}

						if (k > 1)
							for (c = b + 1; c < 64; c++) {

								if (k == 2) {
									for (i = 0; i < 64; i++)
										ny[i][j] = 0; //係数の初期化
									ny[a][j] = y[a][j];//選出済みの基底の係数を0
									ny[b][j] = y[b][j];//選出済みの基底の係数を0
									ny[c][j] = y[c][j];//選出済みの基底の係数を0
									for (i = 0; i < 64; i++)
										xx[i] = 0.0;
									seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
									xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
									avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
									sum = 0.0;
									mk = j % 32;
									ml = j / 32;
									for (l = 0; l < 8; l++)
										for (m = 0; m < 8; m++)
											sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE
									if (min > sum / 64.0) { //MSEの減少が一番小さい基底を抜く
										min = sum / 64.0;
										excel_basis[0] = a;
										excel_basis[1] = b;
										excel_basis[2] = c;
									}
								}

								//if (k > 2)
								//	for (d = c + 1; d < 64; d++) {
								//		if (k == 3) {
								//			for (i = 0; i < 64; i++)
								//				ny[i][j] = 0; //係数の初期化
								//			ny[a][j] = y[a][j];//選出済みの基底の係数を0
								//			ny[b][j] = y[b][j];//選出済みの基底の係数を0
								//			ny[c][j] = y[c][j];//選出済みの基底の係数を0
								//			ny[d][j] = y[d][j];//選出済みの基底の係数を0
								//			for (i = 0; i < 64; i++)
								//				xx[i] = 0.0;
								//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
								//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
								//			avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
								//			sum = 0.0;
								//			mk = j % 32;
								//			ml = j / 32;
								//			for (l = 0; l < 8; l++)
								//				for (m = 0; m < 8; m++)
								//					sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE
								//			if (min > sum / 64.0) //MSEの減少が一番小さい基底を抜く
								//				min = sum / 64.0;
								//		}

										//if (k > 3)
										//	for (o = d + 1; o < 64 - k + 4; o++) {
										//		if (k == 4) {
										//			for (i = 0; i < 64; i++)
										//				ny[i][j] = 0; //係数の初期化
										//			ny[a][j] = y[a][j];//選出済みの基底の係数を0
										//			ny[b][j] = y[b][j];//選出済みの基底の係数を0
										//			ny[c][j] = y[c][j];//選出済みの基底の係数を0
										//			ny[d][j] = y[d][j];//選出済みの基底の係数を0
										//			ny[o][j] = y[o][j];//選出済みの基底の係数を0
										//			for (i = 0; i < 64; i++)
										//				xx[i] = 0.0;
										//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
										//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
										//			avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
										//			sum = 0.0;
										//			mk = j % 32;
										//			ml = j / 32;
										//			for (l = 0; l < 8; l++)
										//				for (m = 0; m < 8; m++)
										//					sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE
										//			if (min > sum / 64.0) //MSEの減少が一番小さい基底を抜く
										//				min = sum / 64.0;
										//		}

										//	}
									//}
							}
					}
			}
			printf("\n[%d] : %lf (%d)", j, min, k);
			fprintf(fp8, "\n,,%lf", min);
			fprintf(fp8, ",,,");
			for (i = 0; i < k + 1; i++)
				fprintf(fp8, ",%d", (int)excel_basis[i]);
		}
	}*/


	//for (j = 0; j < 1024; j++) {
	//	fprintf(fp8, "\n\n\n\n,[%d]\n,,%lf,,,,%d\n,,%lf,,,,%d,%d\n,,%lf,,,,%d,%d,%d", j, full_mse[1][63][j], (int)full_mse[0][63][j],full_mse[1][62][j], (int)full_mse[0][63][j], (int)full_mse[0][62][j], full_mse[1][61][j], (int)full_mse[0][63][j], (int)full_mse[0][62][j], (int)full_mse[0][61][j]);
	//}

	//fclose(fp8);
	printf("a");
	// //////////////////////////// dct ////////////////////////////////////////
	// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

	yn = 'n';
	if (yn == 'n') {

		yn = 'y';
		ent_count_basis(w, ica_basis_ent);
		fprintf(fp10, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp10, "\nQ,DCT_ent,PSNR,SSIM,MS-SSIM,Area,,ICA_ent,PSNR,SSIM,MS-SSIM,Area,,Basis");

		//fprintf(fp, "\n\n\n- - - - - - - - - - - - - - - - ( Reference ) For DCT - - - - - - - - - - - - - - - \n\n\n");
		// 10段階品質があるから10段階分やる
		printf("\nQ? : ");
		scanf("%d", &QQ);
		for (Q = QQ; Q > 20; Q -= 10) {
			printf("\r now Q is %d          \n", Q);
			fprintf(fp, "\n************************************************************************\n\n");
			fprintf(fp, "\n Q is %d\n\n", Q);

			// dct処理
			dct(origin, dcoe, 8); // 係数を出力
			quantization(dcoe, Q); // 係数の品質を10段階で落とす処理（量子化）落とせば落とすほどデータは軽くなるが、品質が落ちる
			idct(dcoe, dcoe2, 8); // 普通の再構成

			// DCT画像確認用
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

			/////////////////// DCTの各ブロックの基底数と画質とentropy　 /////////////////////////////////////
			block_mse(origin, dcoe2, dct_mse);

			for (j = 0; j < 1024; j++) {
				sum = 0.0;
				mk = j % 32;
				ml = j / 32;
				for (a = 0; a < 8; a++) {
					for (b = 0; b < 8; b++) {
						sum += pow(origin[ml * 8 + b][mk * 8 + a] - dcoe2[ml * 8 + b][mk * 8 + a], 2);
					}
				}
				mse_dct[0][(Q / 10) - 1][j] = sum / 64;//MSEを格納



				i = 0;
				for (b = 0; b < 64; b++)
					if (dcoe_temp[b][j] != 0)
						i++;

				mse_dct[1][(Q / 10) - 1][j] = i;//基底個数を格納
			}


			// 係数のエントロピーを算出///////////////////////////////
			for (i = 0; i < 500000; i++) {
				hist[i] = 0;
			}

			/* hist2の作成 */
			min = dcoe_temp[0][0];
			for (j = 0; j < 64; j++)
				for (i = 0; i < 1024; i++)
					if (dcoe_temp[j][i] < min)
						min = dcoe_temp[j][i]; // histの左端

			for (j = 0; j < 64; j++)
				for (i = 0; i < 1024; i++)
					hist[(int)((dcoe_temp[j][i] - min)) + 1]++;

			sum = 0;
			dct_all_mse = 0;
			for (i = 0; i < 500000; i++)
				if (hist[i] > 0) {
					sum += (-((hist[i] / (double)(64 * 1024)) * (log((hist[i] / (double)(64 * 1024))) / log(2))));
				}
			dct_all_mse = sum;
			printf("\nDCT ent = %lf", sum);// ACとDC関係なくエントロピー算出

			//入力画像の個数変更実験(画像出力)
			//for (i = 0; i < 1024; i++) {
			//	Block_flag[i] = mse_dct[0][(Q / 10) - 1][i];
			//	no_op_0[i] = 0;//100
			//	no_op_1[i] = 0;//200
			//	no_op_2[i] = 0;//500
			//	no_op_3[i] = 0;//700
			//}
			//for(j=0;j<700;j++){
			//	max = 0;
			//	a = 0;
			//	for (i = 0; i < 1024; i++)
			//		if (max < Block_flag[i]) {
			//			max = Block_flag[i];
			//			a = i;
			//		}
			//	if (j < 100)
			//		no_op_0[a] = 1;
			//	if (j < 200)
			//		no_op_1[a] = 1;
			//	if (j < 500)
			//		no_op_2[a] = 1;
			//	if (j < 700)
			//		no_op_3[a] = 1;
			//	Block_flag[a] = 0;
			//}
			//img_out(origin, no_op_0, 100);
			//img_out(origin, no_op_1, 200);
			//img_out(origin, no_op_2, 500);
			//img_out(origin, no_op_3, 700);
			//printf("a");
			/////////////////// DCTの各ブロックの基底数と画質とentropy　終了/////////////////////////////////////


			/// 領域分割のメイン処理 ///////////////////////////

			// Step1 初期化用
			for (j = 0; j < 1024; j++) {
				no_op[j] = 0;
				for (i = 0; i < 4; i++)
					if (i == 2 || i == 0)
						bunrui[i][j] = 99;
					else
						bunrui[i][j] = 10000;
				for (i = 0; i < 64; i++)
					ny[i][j] = 0;
				for (i = 0; i < 64; i++)
					nny[i][j] = 0;
			}

			if (yn == 'y') {
				for (j = 0; j < 1024; j++) {
					a = Q / 10 - 1; //Qレート

					for (b = 0; b < 65; b++) {
						ica_basis2[b][j] = 99;
					}

					// 最小基底数 (いらないやつから探索)
					for (b = 0; b < 65; b++)//ICAの基底数　0は一番いらないやつ
						if (mse_dct[0][a][j] > full_mse[1][b][j] && mse_dct[1][a][j] > (double)(64 - b)) {//
							bunrui[3][j] = full_mse[1][b][j];
							bunrui[2][j] = 64 - b;
						}

					//最大基底数 (いるやつから探索)　使用できる基底数範囲の両端を探索可能
					for (b = 64; b > 0; b--)//ICAの基底数　0は一番いらないやつ
						if (mse_dct[0][a][j] > full_mse[1][b][j] && mse_dct[1][a][j] > (double)(64 - b)) {//
							bunrui[1][j] = full_mse[1][b][j];
							bunrui[0][j] = 64 - b;
						}

					//if (mse_dct[0][a][j] < full_mse[1][0][j]) {
					//	bunrui[3][j] = full_mse[1][0][j];
					//	bunrui[2][j] = 64.0;
					//}

					//bunrui[0][j] = mse_dct[1][a][j];
					//bunrui[1][j] = mse_dct[0][a][j];

					if (mse_dct[1][a][j] > bunrui[2][j] && mse_dct[0][a][j] > bunrui[3][j]) {//
						no_op[j] = 1; // no_op 1 ならica

						ica_basis2[64][j] = bunrui[2][j];

						for (b = 63; b > 63 - bunrui[2][j]; b--) {
							ica_basis2[(int)full_mse[0][b][j]][j] = 1;
							ny[(int)full_mse[0][b][j]][j] = y[(int)full_mse[0][b][j]][j];//重要な順で格納
							nny[(int)full_mse[0][b][j]][j] = y[(int)full_mse[0][b][j]][j];
						}
					}
				}
			}
			else if (yn == 'n')
			{
				a = 1;
			}
			else if (yn == 'd')
			{
				ent_out(origin, y, avg, w, ny, no_op, Q);
			}

			// 基底順位上位10個を出力
			if (Q == 30) {
				a = 0;
				fprintf(fp3, "\n\n\n\n,");
				for (i = 1; i < 5; i++) {
					fprintf(fp3, ",[%d]", i);
					if (i == 5)
						fprintf(fp3, ",");
				}

				for (i = 0; i < 1024; i++)
					if (Block_flag[i] == 1)
						a++;
				for (b = 0; b < a; b++) {
					fprintf(fp3, "\n,[%d]", Block_flag2[b]);
					for (i = 0; i < 4; i++) {
						fprintf(fp3, ",%d", (int)full_mse[0][63 - i][Block_flag2[b]]);
						if (i == 4)
							fprintf(fp3, ",");
					}
				}

				//Dice係数
				fprintf(fp3, "\n\n\n,");
				for (b = 0; b < a; b++)
					fprintf(fp3, ",[%d]", Block_flag2[b]);
				for (b = 0; b < a; b++) {
					fprintf(fp3, "\n,[%d]", Block_flag2[b]);
					sum = 0;
					k = 0;
					for (c = 0; c < a; c++) {
						for (i = 0; i < 4; i++) {
							dice_temp[0][i] = (int)full_mse[0][63 - i][Block_flag2[b]];
							dice_temp[1][i] = (int)full_mse[0][63 - i][Block_flag2[c]];
						}
						temp = cor_coe3(dice_temp);
						fprintf(fp3, ",%1.3lf", temp);
						sum += temp;
						if (temp == 0)
							k++;
					}
					fprintf(fp3, ",[%d]", Block_flag2[b]);
					fprintf(fp3, ",[%1.3lf],%d/%d", sum / a, k, a);
				}

			}

			//for (j = 0; j < 1024; j++)
			//	if (bunrui[0][j] != 99 && bunrui[2][j] != 99)
			//		printf("\n\n [%d] min : (%d) %lf ~max : (%d) %lf\n DCT : (%d) %lf", j, (int)bunrui[2][j], bunrui[3][j], (int)bunrui[0][j], bunrui[1][j], (int)mse_dct[1][(int)(Q / 10 - 1)][j], mse_dct[0][(int)(Q / 10 - 1)][j]);

			fprintf(fp6, "\n\n,[%d]\n", Q);

			for (j = 0; j < 1024; j++) {
				no_op_0[j] = 0;
				no_op_1[j] = 0;
				no_op_2[j] = 0;
				no_op_3[j] = 0;
				no_op_4[j] = 0;
				no_op_5[j] = 0;
				if (bunrui[2][j] <= 0 && 0 <= bunrui[0][j])
					no_op_0[j] = 1;
				if (bunrui[2][j] <= 1 && 1 <= bunrui[0][j]) {
					no_op_1[j] = 1;
					no_op_4[j] = 1;
				}
				if (bunrui[2][j] <= 2 && 2 <= bunrui[0][j]) {
					no_op_2[j] = 1;
					no_op_4[j] = 1;
				}
				if (bunrui[2][j] <= 3 && 3 <= bunrui[0][j]) {
					no_op_3[j] = 1;
					no_op_4[j] = 1;
				}
				if (bunrui[2][j] != 99 && bunrui[0][j] != 99 && bunrui[0][j] != 0) {// 0ブロック以外のICA領域（最小基底数0はしょうがない，最大基底数0は許さない）
					no_op_5[j] = 1;
				}
				//if (j != 0)
				//	fprintf(fp8, "\n\n\n\n\n\n\n");
				bunrui_temp[Q / 10 - 1][j][0] = bunrui[2][j];
				bunrui_temp[Q / 10 - 1][j][1] = bunrui[0][j];

			}

			if (Q == 30) {
				fprintf(fp8, "\n\n\n\n");
				for (a = 7; a > 1; a--)
					fprintf(fp8, ",,,[%d]", (a+1) * 10);
				for (j = 0; j < 1024; j++) {
					fprintf(fp8, "\n\n,[%d]", j);
					for (a = 7; a > 1; a--) {
						fprintf(fp8, ",,%d,%d", (int)bunrui_temp[a][j][0], (int)bunrui_temp[a][j][1]);
					}
				}
			}

			img_out(origin, no_op_1, Q + 1);//基底1ブロック
			img_out(origin, no_op_2, Q + 2);//基底2ブロック
			img_out(origin, no_op_3, Q + 3);//基底3ブロック
			img_out(origin, no_op_4, Q + 4);//0無しICAブロック
			img_out(origin, no_op_0, Q + 5);//0のみICAブロック
			img_out(origin, no_op_5, Q + 6);//0のみICAブロック
			img_out(origin, no_op, Q);//ICA領域

			//DCTで画質が悪く、ICAで少数基底の領域を抽出
			k = 0;
			a = 0;
			b = 0;
			for (j = 0; j < 1024; j++) {
				no_op_0[j] = 0;
				if (no_op_4[j] == 1)
					a++;
				if (no_op_5[j] == 1)
					b++;

				//if (bunrui[2][j] <= 4 && 4 <= bunrui[0][j]) {
				//	no_op_4[j] = 1;
				//}
			//	if (mse_dct[0][a][j] <= 500 && no_op_4[j] == 1) {//基底4以下かつMSEが150以上のブロック
			//		no_op_0[j] = 1;
			//		k++;
			//	}
			//	if (mse_dct[0][a][j] > 600 && no_op_4[j] == 1) {
			//		no_op_0[j] = 1;
			//		k++;
			//	}

			//DCTの画質が極端に悪いブロックのみフラグ, DCTのMSEをExcelに出力
				if (j % 32 == 0)
					fprintf(fp6, "\n");
				//if (no_op_5[j] == 1)
				fprintf(fp6, ",%lf", mse_dct[0][Q / 10 - 1][j]);
				//else
				//	fprintf(fp6, ",");

				no_op_1[j] = 0;
				if (Q == 80)
					sum = 150.4;
				if (Q == 50)
					sum = 362.0;
				if (Q == 30)
					sum = 495.5;

				if (mse_dct[0][Q / 10 - 1][j] >= sum) //MSEで閾値して画像出力
					no_op_1[j] = 1;
			}

			//if (k == 0)
			//	k = 99;
			//img_out(origin, no_op_0, k*1000 + 111);//
			//if (Q == 30 || Q == 50 || Q == 80)
			//	img_out(origin, no_op_1, Q + 9);//MSEで閾値した領域を出力
			printf("a");
			fprintf(fp, "\n\n [ 1~3 ] : %d  [ all ] : %d  (%lf)", a, b, ((double)a / (double)b) * 100);

			//DCTMSEのヒストグラム算出実験
			for (i = 0; i < 500000; i++) {
				hist[i] = 0;
			}

			/* hist2の作成 */
			min = 0;
			//for (i = 0; i < 1024; i++)
			//	if (mse_dct[0][Q / 10 - 1][j] < min)
			//		min = mse_dct[0][Q / 10 - 1][j]; // histの左端

			fprintf(fp5, "\n\n\n,[%d]", Q);
			////Step幅0.1
			//fprintf(fp5, "\n,[Step 0.1],");
			//for (i = 0; i < 1024; i++)
			//	hist[(int)((mse_dct[0][Q / 10 - 1][j] - min)*10) + 1]++;
			//for (i = 0; i < 10000; i++)
			//	fprintf(fp5, ",%d", hist[i]);

			////Step幅1
			//for (i = 0; i < 500000; i++)
			//	hist[i] = 0;
			//fprintf(fp5, "\n,[Step 1],");
			//for (i = 0; i < 1024; i++)
			//	hist[(int)((mse_dct[0][Q / 10 - 1][i] - min)) + 1]++;
			//for (i = 0; i < 1000; i++)
			//	fprintf(fp5, ",%d", hist[i]);

			//Step幅5
			for (i = 0; i < 500000; i++)
				hist[i] = 0;
			fprintf(fp5, "\n[Step 5]\n");
			for (i = 0; i < 1024; i++)
				hist[(int)((mse_dct[0][Q / 10 - 1][i] - min) / 5) + 1]++;
			for (i = 0; i < 200; i++)
				fprintf(fp5, ",%d", hist[i]);

			//Step幅10
			for (i = 0; i < 500000; i++)
				hist[i] = 0;
			fprintf(fp5, "\n[Step 10]\n");
			for (i = 0; i < 1024; i++)
				hist[(int)((mse_dct[0][Q / 10 - 1][i] - min) / 10) + 1]++;
			for (i = 0; i < 100; i++)
				fprintf(fp5, ",%d", hist[i]);

			//Step幅20
			for (i = 0; i < 500000; i++)
				hist[i] = 0;
			fprintf(fp5, "\n[Step 20]\n");
			for (i = 0; i < 1024; i++)
				hist[(int)((mse_dct[0][Q / 10 - 1][i] - min) / 20) + 1]++;
			for (i = 0; i < 50; i++)
				fprintf(fp5, ",%d", hist[i]);

			//Step幅50
			for (i = 0; i < 500000; i++)
				hist[i] = 0;
			fprintf(fp5, "\n[Step 50]\n");
			for (i = 0; i < 1024; i++)
				hist[(int)((mse_dct[0][Q / 10 - 1][i] - min) / 50) + 1]++;
			for (i = 0; i < 20; i++)
				fprintf(fp5, ",%d", hist[i]);


			// 基底3個の画質最大を求める実験


			//for (j = 0; j < 1024; j++) {
			//	// 全てのブロックに基底3個格納
			//	for (i = 0; i < 64; i++)
			//		nny[i][j] = 0;
			//	for (i = 0; i < 3; i++) {
			//		nny[(int)full_mse[0][63 - i][j]][j] = y[(int)full_mse[0][63 - i][j]][j];
			//	}

			//	// 再構成処理
			//	for (i = 0; i < 64; i++)
			//		xx[i] = 0.0;
			//	seki5_Block(nw, nny, xx, j); // xx64 -> nw * ny
			//	xtogen_Block(xx, block_ica, avg, j); // ica_sai -> 再構成済①
			//	avg_inter_Block(block_ica, avg, j); // ica_sai -> 再構成済②
			//	sum = 0.0;
			//	mk = j % 32;
			//	ml = j / 32;
			//	for (l = 0; l < 8; l++)
			//		for (m = 0; m < 8; m++)
			//			sum += pow(origin[ml * 8 + m][mk * 8 + l] - block_ica[m * 8 + l], 2); //MSE

			//	no_op_4[j] = 0;
			//	if (dct_mse[j] > sum / 64.0) {
			//		no_op_4[j] = 1;
			//		printf(" ica : %.2lf", dct_mse[j] - sum / 64.0);
			//	}
			//	else
			//		for (i = 0; i < 64; i++)
			//			nny[i][j] = 0;

			//	no_op[j] = no_op_4[j];
			//	for (i = 0; i < 64; i++)
			//		ny[i][j] = nny[i][j];
			//}
			//basis_ent[3] = 0;
			//seki5(nw, ny, x); // x -> nw * ny
			//xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
			//avg_inter(ica_sai, avg); // ica_sai -> 再構成済②
			//img_out(origin, no_op, Q*10+4);//ICA領域
			//// 実験終了

			//////領域分割メイン処理　終了/////////////////////////////


			/// Step2 メイン処理　/////////////////////////////////////////////////////////////////////////////////////
			for (j = 0; j < 1024; j++) {
				no_op_1[j] = 0;
				no_op_2[j] = 0;
				no_op_3[j] = 0;
			}

			if (yn == 'y') {
				//0の情報量ok

				// 動的配列の宣言
				comb3_0 = (double****)malloc(sizeof(double***) * 1024);
				comb3_1 = (double****)malloc(sizeof(double***) * 1024);
				for (a = 0; a < 1024; a++) {
					comb3_0[a] = (double***)malloc(sizeof(double**) * 64);
					comb3_1[a] = (double***)malloc(sizeof(double**) * 64);
					for (b = 0; b < 64; b++) {
						comb3_0[a][b] = (double**)malloc(sizeof(double*) * 64);
						comb3_1[a][b] = (double**)malloc(sizeof(double*) * 64);
						for (c = 0; c < 64; c++) {
							comb3_0[a][b][c] = (double*)malloc(sizeof(double) * 64);
							comb3_1[a][b][c] = (double*)malloc(sizeof(double) * 64);
						}
					}
				}

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

				/////全探索で画質と情報量を算出　1.2.3個全てのパターン//////////////////////////////////////////////////////
				//基底1個で画質＋の領域の画質・情報改善を全探索で算出
				for (a = 0; a < 64; a++) {

					for (j = 0; j < 1024; j++) {
						no_op_1[j] = 0;
						for (i = 0; i < 64; i++)
							ny[i][j] = 0; //係数初期化
					}

					for (j = 0; j < 1024; j++) {

						for (i = 0; i < 64; i++)
							ny[i][j] = 0; //係数初期化

						//temp = 0;
						//if (full_mse[0][64][j] == a)//正順での基底選択に準拠している。それ以外の基底は使用しない
						//	temp = 1;

						//if (temp == 1)
						if (y[a][j] != 0)
							if (bunrui[2][j] <= 1 && 1 <= bunrui[0][j]) {//基底1個領域を対象

								ny[a][j] = y[a][j];

								for (m = 0; m < 64; m++) {
									xx[m] = 0.0;
								}

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

								comb[j][a][0] = sum / 64; //MSEを全探索
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

							//temp = 0;//正順での基底選択に準拠している。それ以外の基底は使用しない
							//if (full_mse[0][64][j] == a || full_mse[0][64][j] == b)
							//	temp = 1;
							//if (full_mse[0][63][j] == a || full_mse[0][63][j] == b)
							//	temp = 1;

							//if (temp == 1)
							if (y[a][j] != 0 || y[b][j] != 0)
								if (bunrui[2][j] <= 2 && 2 <= bunrui[0][j]) {//基底2個領域を対象

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

								//temp = 0;//正順での基底選択に準拠している。それ以外の基底は使用しない
								//if (full_mse[0][64][j] == a || full_mse[0][64][j] == b || full_mse[0][64][j] == c)
								//	temp = 1;
								//if (full_mse[0][63][j] == a || full_mse[0][63][j] == b || full_mse[0][63][j] == c)
								//	temp = 1;
								//if (full_mse[0][62][j] == a || full_mse[0][62][j] == b || full_mse[0][62][j] == c)
								//	temp = 1;

								//if (temp == 1)
								if (y[a][j] != 0 || y[b][j] != 0 || y[c][j] != 0)
									if (bunrui[2][j] <= 3 && 3 <= bunrui[0][j]) {

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

									}
							}
						}
					}
				}
				////////画質・情報量算出 終了//////////////////////////////////////////////////


				////評価のために累積　処理///////////////////////////////////////
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



				for (j = 0; j < 1024; j++) {
					for (a = 0; a < 64 - 2; a++) {
						for (b = a + 1; b < 64 - 1; b++) {
							for (c = b + 1; c < 64; c++) {

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
								if (comb3_0[j][a][b][c] < comb2[j][k][l][0] && comb3_0[j][a][b][c] < comb[j][m][0] && dct_mse[j] > comb3_0[j][a][b][c]) {
									sum = dct_mse[j] - comb3_0[j][a][b][c];
									comb_result3[a][b][c][0] += sum;
									comb_result3[a][b][c][2]++;
								}
								else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0]) {
									sum = dct_mse[j] - comb2[j][k][l][0];
									comb_result3[a][b][c][0] += sum;//dctからの画質改善量を累積
									comb_result3[a][b][c][2]++;
								}
								else if (dct_mse[j] > comb[j][m][0]) {
									sum = dct_mse[j] - comb[j][m][0];
									comb_result3[a][b][c][0] += sum;//dctからの画質改善量を累積
									comb_result3[a][b][c][2]++;
								}
							}
						}
					}
				}



				//累積（基底2つ）
				k = 0;
				for (a = 0; a < 64; a++) {
					for (b = 0; b < 64; b++) {
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
							sum = 0;
							//1個
							k = a;
							if (comb[j][a][0] > comb[j][b][0])
								k = b;

							//1vs2個の画質比較
							if (dct_mse[j] > comb2[j][a][b][0] && comb[j][k][0] > comb2[j][a][b][0]) {
								//画質・情報量
								sum = dct_mse[j] - comb2[j][a][b][0];
								comb_result2[a][b][0] += sum;//dctからの画質改善量を累積
								comb_result2[a][b][2]++;
							}
							else if (dct_mse[j] > comb[j][k][0]) {
								sum = dct_mse[j] - comb[j][k][0];
								comb_result2[a][b][0] += sum;
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
						if (dct_mse[j] > comb[j][a][0]) {
							sum = 0;
							sum = dct_mse[j] - comb[j][a][0];
							comb_result[a][0] += sum;
							comb_result[a][2]++;
						}

				///////累積処理　終了////////////////////////////////////////////////////


				//////ソートして評価　//////////////////////////////////////////////////////////////////
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
						//comb_after_sort[i][1] = comb_result3[k][l][m][1];
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
						//comb_after_sort[i][1] = comb_result2[n][o][1];
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
						//comb_after_sort[i][1] = comb_result[n][1];
						comb_after_sort[i][2] = (double)n;
						comb_after_sort[i][3] = 99;
						comb_after_sort[i][4] = 99;
						comb_after_sort[i][5] = comb_result[n][2];
						comb_sort[n] = 0;//99番基底が入らないか？
					}
				}
				/////////ソートでの評価　終了////////////////////////////////////////////////////////////////////////
				////////Step2 前半終了///////////////////////////////////////////////////////////////////////////////////////////////////////

				//評価上位の基底群は実際に係数を入れて画像全体のエントロピーを算出，DCTのエントロピーと比較する
				fprintf(fp7, "\n\n,,[%d]\n,,,", Q);
				for (j = 0; j < 1024; j++)
					fprintf(fp7, ",[%d]", j);

				for (d = 0; d < 30; d++) {

					a = (int)comb_after_sort[d][2];
					b = (int)comb_after_sort[d][3];
					c = (int)comb_after_sort[d][4];
					fprintf(fp7, "\n,[%d-%d-%d],,[%d]", a, b, c, d);

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
						for (i = 0; i < 64; i++)
							ny[i][j] = 0;
						no_op_1[j] = 0;
						no_op_2[j] = 0;
						no_op_3[j] = 0;
						no_op_4[j] = 0;
						if (a != 99 && b != 99 && c != 99) {//選出基底3津の時
							//基底3で画質最大
							//2個
							min = comb2[j][a][b][0];//MSEだから小さい順
							k = a;
							l = b;
							if (min > comb2[j][b][c][0]) {
								min = comb2[j][b][c][0];
								k = b;
								l = c;
							}
							if (min > comb2[j][a][c][0]) {
								k = a;
								l = c;
							}
							//1個
							min = comb[j][a][0];//MSEだから小さい順
							m = a;
							if (min > comb[j][b][0]) {
								min = comb[j][b][0];
								m = b;
							}
							if (min > comb[j][c][0])
								m = c;

							//1vs2vs3個で画質比較
							if (comb3_0[j][a][b][c] < comb2[j][k][l][0] && comb3_0[j][a][b][c] < comb[j][m][0] && dct_mse[j] > comb3_0[j][a][b][c]) {
								ny[a][j] = y[a][j];
								ny[b][j] = y[b][j];
								ny[c][j] = y[c][j];
								no_op[j] = 1;
								no_op_3[j] = 1;
								if (y[a][j] != 0 && y[b][j] != 0 && y[c][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 3 Error\n (a, b, c) = (%lf, %lf, %lf)", j, y[a][j], y[b][j], y[c][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb3_0[j][a][b][c]);
							}
							else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0]) {
								ny[k][j] = y[k][j];
								ny[l][j] = y[l][j];
								no_op[j] = 1;
								no_op_2[j] = 1;
								if (y[k][j] != 0 && y[l][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 2 Error\n (k, l) = (%lf, %lf)", j, y[k][j], y[l][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb2[j][k][l][0]);
							}
							else if (dct_mse[j] > comb[j][m][0]) {
								ny[m][j] = y[m][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								if (y[m][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 1 Error\n (m) = (%lf)", j, y[m][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb[j][m][0]);
							}
							else {
								fprintf(fp7, ",");
							}
						}
						else if (a != 99 && b != 99 && c == 99) {//選出基底2津の時
							//1個
							k = a;
							if (comb[j][a][0] > comb[j][b][0])
								k = b;

							//1vs2個の画質比較
							if (dct_mse[j] > comb2[j][a][b][0] && comb[j][k][0] > comb2[j][a][b][0]) {
								//画質・情報量
								ny[a][j] = y[a][j];
								ny[b][j] = y[b][j];
								no_op[j] = 1;
								no_op_2[j] = 1;
								if (y[a][j] != 0 && y[b][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 2 Error\n (a, b) = (%lf, %lf)", j, y[a][j], y[b][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb2[j][a][b][0]);
							}
							else if (dct_mse[j] > comb[j][k][0]) {
								ny[k][j] = y[k][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								if (y[k][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 1 Error\n (k) = (%lf)", j, y[k][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb[j][k][0]);
							}
							else {
								fprintf(fp7, ",");
							}
						}
						else if (a != 99 && b == 99 && c == 99) {
							if (comb[j][a][0] < dct_mse[j]) {
								ny[a][j] = y[a][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								if (y[a][j] != 0)
									no_op_4[j] = 1;
								else
									printf("\n[%d] 1 Error\n (a) = (%lf)", j, y[a][j]);
								fprintf(fp7, ",%lf", dct_mse[j] - comb[j][a][0]);
							}
							else {
								fprintf(fp7, ",");
							}
						}

					}// 基底格納終了

					//img_out(origin, no_op_4, Q * 1000 + d*10+1);//評価上位基底の画像出力　発表資料用
					//for (i = 0; i < 1024; i++)
					//	if (no_op_4[i] == 1)
					//		no_op_4[i] = 0;
					//	else
					//		no_op_4[i] = 1;
					//img_out(origin, no_op_4, Q * 1000 + d*10+2);//DCT_Blockの出力

					//画像全体のエントロピー算出///////////////////////////////////////////////

					for (i = 0; i < 500000; i++) {
						hist[i] = 0;
					}

					/* hist2の作成 *///DCも一緒にエントロピー算出
					//DCは差分のエントロピーを算出
					for (i = 0; i < 1024; i++) {
						if (no_op[i] == 1)
							ica_dc[i] = avg[i] * 8;
						else
							ica_dc[i] = dcoe_temp[0][i];
					}

					test_area[0] = ica_dc[0];
					for (i = 1; i < 1024; i++)
						test_area[i] = (double)(ica_dc[i] - ica_dc[i - 1]);
					/////////////////////////////////////////////////////////

					/* hist2の作成 */
					min = 10000;
					a = 0;//DCT領域数カウント用
					for (j = 0; j < 64; j++)
						for (i = 0; i < 1024; i++)
							if (no_op[i] == 1) {
								if (ny[j][i] < min) {
									min = ny[j][i]; // histの左端
								}
							}
							else if (no_op[i] == 0)
								if (dcoe_temp[j][i] < min && j != 0) {
									min = dcoe_temp[j][i]; // histの左端
								}

					/////////  DCの最小も確認
					//min = test_area[0];
					for (i = 0; i < 1024; i++)
						if (test_area[i] < min)
							min = test_area[i]; // histの左端

					// ACのヒストグラム
					for (j = 0; j < 64; j++)
						for (i = 0; i < 1024; i++) {
							if (no_op[i] == 1) {
								hist[(int)((ny[j][i] - min) * step) + 1]++;
							}
							else
								if (j != 0)//DCを除いているため
									hist[(int)((dcoe_temp[j][i] - min) * step) + 1]++;
						}

					// DCのヒストグラム
					for (i = 0; i < 1024; i++) {
						hist[(int)((test_area[i] - min) * step) + 1]++;
						if (no_op[i] == 1)
							a++;
					}



					sum = 0;
					k = 0;
					//a = 1024 - a; //DCTのDC分引く（DCTは63個だから）
					for (i = 0; i < 500000; i++)
						if (hist[i] > 0) {
							sum += (-((hist[i] / (double)(64 * 1024 + a)) * (log((hist[i] / (double)(64 * 1024 + a))) / log(2))));
							//printf("\n%d : %d (%lf)", i, hist[i], -((hist[i] / (double)(64 * 1024 + a)) * (log((hist[i] / (double)(64 * 1024 + a))) / log(2))));
							k += hist[i];
						}
					//printf("\n\n%d : %d", 64 * 1024 + a, k);
					//printf("\n\n ICA ent = %lf", sum);
					comb_after_sort[d][1] = sum;
				}


				/////// Step2 後半　基底選出処理////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//基底選出
				k = 0;
				c = 0;
				d = 0;
				l = 0;
				sum = 0;
				for (i = 0; i < 1024; i++)
					if (no_op_0[i] == 1)
						l++;
				for (a = 0; a < 30; a++) {

					if (a >= 20)
						c = 1;
					else if (a >= 10 && a < 20)
						c = 2;
					else
						c = 3;
					//printf("\n area sum = %d",(int)(1024 * 64 + comb_after_sort[a][5] + l));
					///printf("\ndct_all_mse = %lf\ncomb_after_sort[a][1] =%lf\n", dct_all_mse, comb_after_sort[a][1]);
					sum = dct_all_mse * 1024 * 64;
					printf("\n DCT sum = %lf", sum);
					fprintf(fp, "\n DCT sum = %lf", sum);

					basis_ent[3] = ent_count_basis2(w, (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4]);
					sum = (comb_after_sort[a][1] * (1024 * 64 + comb_after_sort[a][5] + l) + basis_ent[3] * 64 * 64);
					printf("\n ICA sum = %lf (Basis ent : %lf)", sum, basis_ent[3] * 64 * 64);
					fprintf(fp, "\n ICA sum = %lf (Basis ent : %lf)", sum, basis_ent[3] * 64 * 64);

					if (k == 0) {
						if (dct_all_mse * 1024 * 64 > sum) { // 基底0の改善情報量 + 基底１（対象基底）の改善情報量 + これまでの情報量 > 基底の情報量 * いくつ使っているか
							printf("\n [%d , %d, %d]  %lf vs %lf >>  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							fprintf(fp, "\n [%d , %d, %d]  %lf vs %lf >>  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							for (b = 0; b < 6; b++)
								comb_after_sort[0][b] = comb_after_sort[a][b];//選出基底以外全て初期化
							k++;
							fprintf(fp9, "\n\n [%d] ******************************************************\n", Q);
							fprintf(fp9, "\n [%d , %d, %d]  %lf vs %lf >>  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);

						}
						else {
							printf("\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							fprintf(fp, "\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);

							comb_after_sort[a][0] = 0;//選出基底以外全て初期化
							comb_after_sort[a][1] = 0;//選出基底以外全て初期化
							comb_after_sort[a][2] = 99;//選出基底以外全て初期化
							comb_after_sort[a][3] = 99;//選出基底以外全て初期化
							comb_after_sort[a][4] = 99;//選出基底以外全て初期化
						}
					}
					else {
						printf("\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
						fprintf(fp, "\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);

					}

					if (a == 9) {
						printf("\n");
						fprintf(fp, "\n");
					}
					if (a == 19) {
						printf("\n");
						fprintf(fp, "\n");
					}
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

				basis_ent[0] = (double)a;
				basis_ent[1] = (double)b;
				basis_ent[2] = (double)c;
				basis_ent[3] = ent_count_basis2(w, a, b, c);
				printf("\n\n%d, %d, %d  (%lf)", a, b, c, basis_ent[3]);
				fprintf(fp, "\n\n%d, %d, %d  (%lf)\n\n\n", a, b, c, basis_ent[3]);
				//////基底選出　終了///////////////////////////////////////////////////////////////

				/// 選出基底の係数を格納 ///////////////////////////////////////////////////////
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
					no_op_4[j] = 0;

					if (a != 99 && b != 99 && c != 99) {//選出基底3津の時
						//基底3で画質最大
						//2個
						min = comb2[j][a][b][0];//MSEだから小さい順
						k = a;
						l = b;
						if (min > comb2[j][b][c][0]) {
							min = comb2[j][b][c][0];
							k = b;
							l = c;
						}
						if (min > comb2[j][a][c][0]) {
							k = a;
							l = c;
						}
						//1個
						min = comb[j][a][0];//MSEだから小さい順
						m = a;
						if (min > comb[j][b][0]) {
							min = comb[j][b][0];
							m = b;
						}
						if (min > comb[j][c][0])
							m = c;

						//1vs2vs3個で画質比較
						if (comb3_0[j][a][b][c] < comb2[j][k][l][0] && comb3_0[j][a][b][c] < comb[j][m][0] && dct_mse[j] > comb3_0[j][a][b][c]) {
							ny[a][j] = y[a][j];
							ny[b][j] = y[b][j];
							ny[c][j] = y[c][j];
							no_op[j] = 1;
							no_op_3[j] = 1;
							if (y[a][j] != 0 && y[b][j] != 0 && y[c][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 3 Error\n (a, b, c) = (%lf, %lf, %lf)", j, y[a][j], y[b][j], y[c][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d, %d, %d]", j, dct_mse[j] - comb3_0[j][a][b][c], a, b, c);
						}
						else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0]) {
							ny[k][j] = y[k][j];
							ny[l][j] = y[l][j];
							no_op[j] = 1;
							no_op_2[j] = 1;
							if (y[k][j] != 0 && y[l][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 2 Error\n (k, l) = (%lf, %lf)", j, y[k][j], y[l][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d, %d]", j, dct_mse[j] - comb2[j][k][l][0], k, l);
						}
						else if (dct_mse[j] > comb[j][m][0]) {
							ny[m][j] = y[m][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							if (y[m][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 1 Error\n (m) = (%lf)", j, y[m][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d]", j, dct_mse[j] - comb[j][m][0], m);
						}
					}
					else if (a != 99 && b != 99 && c == 99) {//選出基底2津の時
						//1個
						k = a;
						if (comb[j][a][0] > comb[j][b][0])
							k = b;

						//1vs2個の画質比較
						if (dct_mse[j] > comb2[j][a][b][0] && comb[j][k][0] > comb2[j][a][b][0]) {
							//画質・情報量
							ny[a][j] = y[a][j];
							ny[b][j] = y[b][j];
							no_op[j] = 1;
							no_op_2[j] = 1;
							if (y[a][j] != 0 && y[b][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 2 Error\n (a, b) = (%lf, %lf)", j, y[a][j], y[b][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d, %d]", j, dct_mse[j] - comb2[j][a][b][0], a, b);
						}
						else if (dct_mse[j] > comb[j][k][0]) {
							ny[k][j] = y[k][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							if (y[k][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 1 Error\n (k) = (%lf)", j, y[k][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d]", j, dct_mse[j] - comb[j][k][0], k);
						}
					}
					else if (a != 99 && b == 99 && c == 99) {
						if (comb[j][a][0] < dct_mse[j]) {
							ny[a][j] = y[a][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							if (y[a][j] != 0)
								no_op_4[j] = 1;
							else
								printf("\n[%d] 1 Error\n (a) = (%lf)", j, y[a][j]);
							fprintf(fp9, "\n[%d]    %.0lf  [%d]", j, dct_mse[j] - comb[j][a][0], a);
						}
					}
				}// 基底格納終了



				img_out(origin, no_op_0, Q * 10 + 5);//基底0ブロック
				img_out(origin, no_op_1, Q * 10 + 1);//基底1ブロック
				img_out(origin, no_op_2, Q * 10 + 2);//基底2ブロック
				img_out(origin, no_op_3, Q * 10 + 3);//基底3ブロック
				img_out(origin, no_op_4, Q * 10 + 4);//0抜きICAブロック
				img_out(origin, no_op, Q * 10 + 6);//ICAブロック

				seki5(nw, ny, x); // x -> nw * ny
				xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
				avg_inter(ica_sai, avg); // ica_sai -> 再構成済②

				img_out2(dcoe2, ica_sai, no_op, Q + 4);//再構成画像
				////// 係数格納　終了	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			/// 以下情報量等の算出　処理 ////////////////////////////////////////////////////////
			printf("a");
			// 多分 使用する基底（付加情報）のエントロピーを算出する処理//////////////////////////
			if (yn == 's') {
				for (i = 0; i < 64; i++)
					for (j = 0; j < 64; j++)
						nw[i][j] = w[i][j];
				seki5(nw, ny, x); // x -> nw * ny
				xtogen(x, ica_sai, avg); // ica_sai -> 再構成済①
				avg_inter(ica_sai, avg); // ica_sai -> 再構成済②
				for (j = 0; j < 1024; j++)
					no_op_0[j] = 0;
				for (j = 0; j < 1024; j++)
					for (i = 0; i < 64; i++)
						if (ny[i][j] != 0)
							no_op_0[i]++;
				for (j = 0; j < 64; j++)
					if (no_op_0[j] == 0) {
						for (i = 0; i < 64; i++)
							nw[i][j] = 0;
					}
				//基底のEntropy
				for (i = 0; i < 500000; i++) {
					hist[i] = 0;
				}
				min = 100000;
				for (i = 0; i < 64; i++)
					for (j = 0; j < 64; j++)
						if (min > nw[i][j])
							min = nw[i][j]++;
				for (i = 0; i < 64; i++)
					for (j = 0; j < 64; j++)
						hist[(int)(nw[i][j] - min) + 1]++;
				for (i = 0; i < 500000; i++)
					if (hist[i] > 0) {
						sum += -((hist[i] / (double)(64 * 64)) * (log((hist[i] / (double)(64 * 64))) / log(2)));
					}
				printf("\n Before = %lf", basis_ent[3]);
				basis_ent[3] = sum;
				printf("\n After = %lf", basis_ent[3]);
			}
			////////////////// 付加情報のエントロピー算出終了　間違っているため今は使っていない　///////////////////////////////

			//Qレート
			fprintf(fp10, "\n");
			fprintf(fp10, "%d,", Q);

			// DCTエントロピー
			sum = 0;
			sum = dct_all_mse * 1024 * 64; //DCT自己情報の総和
			sum = sum / (1024 * 64);
			fprintf(fp10, "%lf,", sum);

			// DCT PSNR
			sum = 0;
			sum = psnr(origin, dcoe2);
			fprintf(fp10, "%lf,", sum);

			// DCT SSIM
			sum = 0;
			sum = SSIM(origin, dcoe2, 256, 256);
			fprintf(fp10, "%lf,,", sum);

			// Number of DCT Block
			a = 0;
			for (i = 0; i < 1024; i++)
				if (no_op[i] == 0)
					a++;
			fprintf(fp10, "%d,,", a);

			// ICA エントロピー///////////////////////////////////////////////
			for (i = 0; i < 500000; i++) {
				hist[i] = 0;
			}

			for (i = 0; i < 1024; i++) { // それぞれのDCを格納
				if (no_op[i] == 1)
					ica_dc[i] = avg[i] * 8;
				else
					ica_dc[i] = dcoe_temp[0][i];
			}

			test_area[0] = ica_dc[0];
			for (i = 1; i < 1024; i++)
				test_area[i] = (double)(ica_dc[i] - ica_dc[i - 1]);//DCは差分でエントロピー

			min = 10000;
			for (j = 0; j < 64; j++)
				for (i = 0; i < 1024; i++)
					if (no_op[i] == 1) {
						if (ny[j][i] < min) {
							min = ny[j][i]; // 係数の最小を探索
						}
					}
					else if (no_op[i] == 0)
						if (dcoe_temp[j][i] < min && j != 0) {
							min = dcoe_temp[j][i]; // 係数の最小を探索
						}

			for (i = 0; i < 1024; i++)
				if (test_area[i] < min)
					min = test_area[i]; // DCの最小も確認


			for (j = 0; j < 64; j++)
				for (i = 0; i < 1024; i++) {
					if (no_op[i] == 1)
						hist[(int)((ny[j][i] - min) * step) + 1]++;// ICA係数ヒストグラム
					else
						if (j != 0)//DCを除いているため
							hist[(int)((dcoe_temp[j][i] - min) * step) + 1]++;//DCT係数ヒストグラム
				}

			for (i = 0; i < 1024; i++)
				hist[(int)((test_area[i] - min) * step) + 1]++;// DCヒストグラム

			a = 0;//DCT領域数カウント用
			for (i = 0; i < 1024; i++)
				if (no_op[i] == 1)
					a++;

			sum = 0;
			for (i = 0; i < 500000; i++)
				if (hist[i] > 0) {
					sum += (-((hist[i] / (double)(64 * 1024 + a)) * (log((hist[i] / (double)(64 * 1024 + a))) / log(2))));
				}

			sum = sum * (64 * 1024 + a) + basis_ent[3] * 64 * 64;
			sum = sum / (1024 * 64);

			fprintf(fp10, "%lf,", sum);///////////////////////////////////////////////////

			// ICA PSNR
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
			sum = psnr(origin, dct_ica_sai);
			fprintf(fp10, "%lf,", sum);

			// ICA SSIM
			sum = SSIM(origin, dct_ica_sai, 256, 256);
			fprintf(fp10, "%lf,,", sum);

			// number of ICA Blocks
			fprintf(fp10, "%d,", a);

			//basis of ICA
			fprintf(fp10, ",%d,%d,%d", (int)basis_ent[0], (int)basis_ent[1], (int)basis_ent[2]);



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