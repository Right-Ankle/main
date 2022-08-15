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
	static unsigned char origin_change[256][256] = { 0 }; //基底作成用画像（各レートの選出前のICA領域）
	static unsigned char nica_basis[64][64]; //ica基底出力用
	static unsigned char dcoe2[256][256] = { 0 }; //dct再構成用
	static unsigned char  ica_sai[256][256] = { 0 }; //ica再構成用
	static unsigned char block_ica[64] = { 0 }; //ica ブロックでの再構成用
	static unsigned char dct_ica_sai[256][256]; //DCT&ICA併用後の再構成

	////// int //////
	static int a, b, c, d, i, j, k, l, m, n, o, mk, ml; //計算用変数
	static int ori_temp[256 * 256] = { 0 }; // 原画像(256*256の一次元)
	static int no_op[1024] = { 0 }; // ブロックflag（基本的に0=DCT_Block, 1=ICA_Block）
	static int no_op_0[1024] = { 0 }; // フラグを立てたブロックのみ出力するためのやつ（使い捨て）
	static int no_op_1[1024] = { 0 };
	static int no_op_2[1024] = { 0 };
	static int no_op_3[1024] = { 0 };
	static int no_op_4[1024] = { 0 };
	static int no_op_5[1024] = { 0 };
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
	double ica_dc[1024];  // ICAの直流成分のentropy
	double ica_w[64]; //ICA基底のentropy
	double ica_basis_ent[3]; //ICA基底のentropy
	double basis_temp[64]; //entropy操作時に使用（）
	double excel_basis[7];//0->ica基底，1->DCT単独，2->DCT領域，3->ICA領域，4->ICA領域数，5->ICAのDC "制限基底数=(1-2-5)/((3/4)+0)"（Excel用）
	double dct_mse[1024]; //DCTの各ブロックのMSE
	double dct_dct[1024];
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
	// フォルダ初期化 処理終了//////////////////////////////////////////////////////////////////////////


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
	static char filename16[20] = { '1', '6', '.', 'b', 'm', 'p' };

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
	}
	///基底変更完了////////////////////////
	printf("a");
	///画像入力 処理終了//////////////////////////////////////////////////////////////////////////////////////////////


	//出力ファイル　宣言 ///////////////////////////////////////////////////////////////////////////
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
	/////////////////宣言処理 終了//////////////////////////////////////////////////////////////////////////////////////////////////////

		// ///////////////////////// ica基底と係数を求める //////////////////////////////////
	// ICA基底・係数
	// origin = 画素値(256*256),  y = ica係数(ブロックで64個で1024ブロック分),  w = ica基底(64個の計算法の中にそれぞれ64個の計算法がある)
	// ICAに"origin"を入れることで"y"(計算後の値)と"w"(計算の仕方)の結果が出力される
	// 基底は計算方法。係数は 8*8の画素ブロックを構成するのに 64個の基底がそれぞれ どれくらい使われているのか（含まれているか）の値。
	// ブロックとは 256*256画素のうち縦8横8のブロック。一画像につき(256/8) 32*32 = 1024ブロック
	pcaStr = new_pca(origin);
	//originは原画像の輝度の値、yはICA係数の値、ｗはICA基底の輝度、avgは各ブロックの平均値が入っている．
	//ICA()の処理は”今は”追う必要ない
	ICA(origin, pcaStr, y, w, avg, 100, 0.002);

	printf("a");

	// 計算用nyにコピー
	sum = 0;
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 64; j++) {
			ny[j][i] = y[j][i]; // ny -> y(ica係数コピー)
			sum += y[j][i];
		}
	printf("\n%lf", sum); //各ブロックの係数値の平均
	printf("a");

	//計算用nwにコピー
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			nw[j][i] = w[j][i]; // nw-> w(ica基底コピー)

	//初期化
	for (j = 0; j < 1024; j++)
		for (n = 0; n < 64; n++) {
			ny[n][j] = 0;
			ny2[n][j] = 0;
		}

	// ICAの直流成分をQ90で代用　//////////////////////////////////////////////////////////////////////////////////////////////////////////

	Q = 90; //DCTをQ90で量子化
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
						//各ブロックのDCT直流成分を1/8すると画素の平均値になる
						//ICAの直流成分は画素値と同じであるため，そのまま代用可能
						avg[n] = dcoe[i + k][j + l] / 8;
					}
					m++;
				}
			}
			n++;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////

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

	/// 基底を全て使ったときのMSE////////////////////////////////////////////////////////////////////
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
	/// 基底を全て使った時終了///////////////////////////////////////////////////////////////////////////


	/// 基底を0個使った時のMSE ///////////////////////////////////////////////////////////////////
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

	
	///基底を使わなかったとき　終了///////////////////
	/////////////////  Step1  のメイン処理　終了/////////////////////////////////////////////////////////////

	printf("a");
	// //////////////////////////// dct ////////////////////////////////////////
	// ICA と大体同じ。DCTの基底は汎用的だから決まっている。係数のみを動かせばいい

	yn = 'n';
	if (yn == 'n') {

		yn = 'y';
		//出力用やつ　無視して大丈夫
		ent_count_basis(w, ica_basis_ent);
		fprintf(fp7, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp7, "\nQ,DCT_only,DCT_area,ICA_area,ICA_Num,ICA_DC,Basis_Type,Basis_Num,,Result");
		fprintf(fp10, "\nICA_Basis,%lf", ica_basis_ent[0]);
		fprintf(fp10, "\nQ,DCT_ent,PSNR,SSIM,MS-SSIM,Area,,ICA_ent,PSNR,SSIM,MS-SSIM,Area,,Basis");

		//fprintf(fp, "\n\n\n- - - - - - - - - - - - - - - - ( Reference ) For DCT - - - - - - - - - - - - - - - \n\n\n");
		// 10段階品質があるから10段階分やる
		printf("\nQ? : ");
		scanf("%d", &QQ);
		for (Q = QQ; Q > 0; Q -= 10) {
			printf("\r now Q is %d          \n", Q);

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
			else if (yn == 'n') //従来手法（ＭＰ法）用　　アーカイブに処理がある
			{
				a = 1;
			}

			fprintf(fp8, "\n\n\n\n,,[%d]\n", Q);
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
				if (bunrui[2][j] != 99 && bunrui[0][j] != 99 && bunrui[0][j] != 0) {// 0ブロック以外のICA領域
					no_op_5[j] = 1;
				}
				if (j != 0)
					fprintf(fp8, "\n\n\n\n\n\n\n");
				fprintf(fp8, ",[%d], %d,%d", j, (int)bunrui[2][j], (int)bunrui[0][j]);
			}

			img_out(origin, no_op_1, Q + 1);//基底1ブロック
			img_out(origin, no_op_2, Q + 2);//基底2ブロック
			img_out(origin, no_op_3, Q + 3);//基底3ブロック
			img_out(origin, no_op_4, Q + 4);//0無しICAブロック
			img_out(origin, no_op_0, Q + 5);//0のみICAブロック
			img_out(origin, no_op_5, Q + 6);//0のみICAブロック
			img_out(origin, no_op, Q);//ICA領域

			fclose(fp8);
			printf("a");
			//////領域分割メイン処理　終了/////////////////////////////


			/// Step2 メイン処理　/////////////////////////////////////////////////////////////////////////////////////
			for (j = 0; j < 1024; j++) {
				no_op_1[j] = 0;
				no_op_2[j] = 0;
				no_op_3[j] = 0;
			}

			if (yn == 'y') {

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

				//配列の初期化
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

						//基底選択をstep1の順序で固定する用
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

							//基底選択をstep1の順序で固定する用
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

								//基底選択をstep1の順序で固定する用
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
				for (d = 0; d < 30; d++) {

					a = (int)comb_after_sort[d][2];
					b = (int)comb_after_sort[d][3];
					c = (int)comb_after_sort[d][4];


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
								no_op_4[j] = 1;
							}
							else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0]) {
								ny[k][j] = y[k][j];
								ny[l][j] = y[l][j];
								no_op[j] = 1;
								no_op_2[j] = 1;
								no_op_4[j] = 1;
							}
							else if (dct_mse[j] > comb[j][m][0]) {
								ny[m][j] = y[m][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								no_op_4[j] = 1;
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
								no_op_4[j] = 1;
							}
							else if (dct_mse[j] > comb[j][k][0]) {
								ny[k][j] = y[k][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								no_op_4[j] = 1;
							}
						}
						else if (a != 99 && b == 99 && c == 99) {
							if (comb[j][a][0] < dct_mse[j]) {
								ny[a][j] = y[a][j];
								no_op[j] = 1;
								no_op_1[j] = 1;
								no_op_4[j] = 1;
							}
						}
					}// 基底格納終了

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
						}
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

					sum = dct_all_mse * 1024 * 64; //平均情報量から自己情報量に戻す
					printf("\n DCT sum = %lf", sum);
					sum = (comb_after_sort[a][1] * (1024 * 64 + comb_after_sort[a][5] + l) + ica_basis_ent[c - 1] * 64 * 64);//平均情報量から自己情報量に戻す。この時、ICA基底の情報量も加えている
					printf("\n ICA sum = %lf (%lf)", sum, dct_all_mse * 1024 * 64 - sum);//DCTとの差を算出

					//画質が高い組み合わせから比較
					if (k == 0) {//kはカウンターで選出基底が決まった場合ｋ＝1となる
						if (dct_all_mse * 1024 * 64 > sum) {//DCTのエントロピーと比較
							printf("\n [%d , %d, %d]  %lf vs %lf >>  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							for (b = 0; b < 6; b++)
								comb_after_sort[0][b] = comb_after_sort[a][b];//選出基底以外全て初期化
							k++;
						}
						else {//エントロピーがDCTを超えた場合は初期化
							printf("\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
							comb_after_sort[a][0] = 0;//選出基底以外全て初期化
							comb_after_sort[a][1] = 0;//選出基底以外全て初期化
							comb_after_sort[a][2] = 99;//選出基底以外全て初期化
							comb_after_sort[a][3] = 99;//選出基底以外全て初期化
							comb_after_sort[a][4] = 99;//選出基底以外全て初期化
						}
					}
					else {
						printf("\n [%d , %d, %d]  %lf vs %lf <  %lf  :  %lf  (%d)", (int)comb_after_sort[a][2], (int)comb_after_sort[a][3], (int)comb_after_sort[a][4], dct_all_mse * 1024 * 64, sum, dct_all_mse * 1024 * 64 - sum, comb_after_sort[a][0], (int)comb_after_sort[a][5]);
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

				basis_ent[0] = (double)a;
				basis_ent[1] = (double)b;
				basis_ent[2] = (double)c;
				basis_ent[3] = ent_count_basis2(w, a, b, c);
				printf("\n%d, %d, %d  (%lf)", a, b, c, basis_ent[3]);
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
							ny[a][j] = y[a][j];
							ny[b][j] = y[b][j];
							ny[c][j] = y[c][j];
							no_op[j] = 1;
							no_op_3[j] = 1;
							no_op_4[j] = 1;
						}
						else if (comb2[j][k][l][0] < comb[j][m][0] && dct_mse[j] > comb2[j][k][l][0]) {
							ny[k][j] = y[k][j];
							ny[l][j] = y[l][j];
							no_op[j] = 1;
							no_op_2[j] = 1;
							no_op_4[j] = 1;
						}
						else if (dct_mse[j] > comb[j][m][0]) {
							ny[m][j] = y[m][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							no_op_4[j] = 1;
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
							no_op_4[j] = 1;
						}
						else if (dct_mse[j] > comb[j][k][0]) {
							ny[k][j] = y[k][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							no_op_4[j] = 1;
						}
					}
					else if (a != 99 && b == 99 && c == 99) {
						if (comb[j][a][0] < dct_mse[j]) {
							ny[a][j] = y[a][j];
							no_op[j] = 1;
							no_op_1[j] = 1;
							no_op_4[j] = 1;
						}
					}

				}

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


	fclose(fp10);
	//gnuplot(dcoe_temp);


	printf(" All finish");

	printf("\nPress any button...");
	getchar();
	getchar();
}