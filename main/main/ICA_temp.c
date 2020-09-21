/* ICA.c */

/****************************************************************************************************************************************/
/*  引数（256*256の画像データ，PCAの結果，ICA係数用の64*1024の配列，ICA基底用の64*64の配列，ICAの直流成分用の1024の配列，計算回数，学習係数）	*/
/*  ny, nw, navg は引数として渡すと中身が書き換えられます。適切な計算回数と学習係数は画像により異なります。詳細はnew_ica.cを参照してください。	*/
/****************************************************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "ica.h"
// 引数（256*256の画像データ，PCAの結果，ICA係数用の64*1024の配列，ICA基底用の64*64の配列，ICAの直流成分用の1024の配列，計算回数，学習係数）
// ny->ica係数, nw->ica基底, navg->原ブロックの平均, e->反復回数, f->学習係数
void ICA_temp(unsigned char origin[][256], struct pca pcaTemp, double ny[][1024], double nw[][64], double navg[1024], int e, double f, int num1, int num2) {
	//FILE * fp2; // fp1:pca基底, fp2:ica基底, fp3:ica係数, fp4:
	int i, j, k, m, n, l, a, b, t, count = 0, count2 = 0; // 計算用
	static unsigned char gen[256][256] = { 0 }, npca_basis[64][64] = { 0 }, nica_basis[64][64] = { 0 }, ica_sai[256][256] = { 0 };
	static double  x[64][1024] = { 0 }, w[64][64] = { 0 }, v[64][64] = { 0 }, p[64][64] = { 0 }, y[64][1024] = { 0 }, z[64][64] = { 0 }, q[64][64] = { 0 }, dw[64][64] = { 0 }, w2[64][64] = { 0 };
	static double** xx, ** yy, ** sig, * avg, ** yt, * b_kurt;
	static double temp;
	static double sum = 0;

	char output[50];
	static int temp_basis[64 * 64];
	static int temp_basis2[64 * 64];
	static int ica[64];
	static int temp_sai[256 * 256];

	/*　ファイルオープン　*/
	//if ((fp1 = fopen("PCA_BASIS.pgm", "wb")) == NULL) {  //pgm > グレースケール画像を扱うための拡張子
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp2 = fopen("ICA_test.pgm", "wb")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp3 = fopen("y.csv", "w")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp4 = fopen("ICA_SAI.pgm", "wb")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	//if ((fp5 = fopen("ica_ori.txt", "a")) == NULL) {
	//	fprintf(stderr, "Can not open file\n");
	//}

	printf("\nICA --> [%d : %d] --> ", num1, num2);

	/*　領域を確保　*/
	/*  malloc > 引数で指定するバイト数分のメモリを動的に確保　(使用時はstdlibをインポート) */
	/*           メモリ領域へのポインタが返される (確保できない場合はNULL)　　　　　　　　　*/
	/*           静的・動的メモリ確保の場合,主記憶状にプログラムが存在・サブルーチンが実行中 */
	/*           の間は,メモリが確保し続けられている　　　　　　　　　　　　　　　　　　　　 */
	/*           -> 動的にすれば，解放したメモリ分の無駄を省くことが可能                 */

	// sizeof > バイト数を指定
	xx = (double**)malloc(sizeof(double*) * 64);
	yy = (double**)malloc(sizeof(double*) * 64);
	sig = (double**)malloc(sizeof(double*) * 64);
	avg = (double*)malloc(sizeof(double) * 1024);
	// 構造体1つ1つにもメモリを確保
	for (i = 0; i < 64; i++) {
		// 64個に1024バイト
		xx[i] = (double*)malloc(sizeof(double) * 1024);
		yy[i] = (double*)malloc(sizeof(double) * 1024);
		sig[i] = (double*)malloc(sizeof(double) * 1024);
	}

	yt = (double**)malloc(sizeof(double*) * 1024);
	for (i = 0; i < 1024; i++) {
		// 1024個に64バイト
		yt[i] = (double*)malloc(sizeof(double) * 64);
	}

	// コピー
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			gen[i][j] = origin[i][j];

	// 固有値を小さい順にソート -> 固有ベクトルも交換
	for (i = 0; i < 64 - 1; i++) {
		for (j = 64 - 1; j > i; j--) {
			if (pcaTemp.r[j - 1] < pcaTemp.r[j]) {  /* 前の固有値の方が大きかったら */
				temp = pcaTemp.r[j];        /* 交換する */
				pcaTemp.r[j] = pcaTemp.r[j - 1];
				pcaTemp.r[j - 1] = temp;

				// 固有ベクトルの列をすべて交換
				for (k = 0; k < 64; k++) {
					temp = pcaTemp.a[j][k];
					pcaTemp.a[j][k] = pcaTemp.a[j - 1][k];
					pcaTemp.a[j - 1][k] = temp;
				}
			}
		}
	}


	gentox(gen, x); //256*256 -> 64*1024に変換

	// 縦にコピーしてるだけ(ブロックごとに計算するため。コピー時は特に関係ない)
	for (i = 0; i < 1024; i++) {
		for (j = 0; j < 64; j++) {
			xx[j][i] = x[j][i];
		}
	}
	//
	new_pre(xx, avg, 1024);	//中心化 （xx -> 元の画素値 - 平均値, avg = ブロックごとの平均値）

	new_whitening(pcaTemp, xx, v, p, 1024);	//白色化 -> xx(データの共分散行列が単位行列となるように変換)
	// 白色化後の行列を確認したい。

	makew(w);	//初期行列生成

	/*　反復開始　e -> 反復回数*/
	for (t = 0; t < e; t++) {

		new_seki(w, xx, yy, 1024);	//行列の積を計算
//
		new_sigmoid(yy, sig, 1024);	//シグモイド関数を適用, sigの範囲は0~1

		new_ytoyt(yy, yt, 1024);	//y の転置行列を計算

		new_seki2(sig, yt, z, 1024);	//行列の積を計算

		identity(z, q);	//単位行列との減算

		seki3(q, w, dw);	//行列の積を計算

		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				w[i][j] += f * dw[i][j];	//学習係数 f により過度な学習を抑える
			}
		}

		normal(w);	//直交化
	}

	seki3(w, v, w2); // w2 -> wとvの積
	inv(w2, w);	//ここまでで w が完成 w -> w2の逆行列, w2 -> 単位行列

	printf("END\n");

	wtosai(p, npca_basis);	//出力用PCA基底の作成　p -> pca基底
	wtosai(w, nica_basis);	//出力用ICA基底の作成　w -> ica基底


	//fprintf(fp1, "P5\n64 64\n255\n");
	//fwrite(npca_basis, sizeof(unsigned char), 64 * 64, fp1);	//PCA基底出力, 64*64 0~255

	//fprintf(fp2, "P5\n64 64\n255\n");
	//fwrite(nica_basis, sizeof(unsigned char), 64 * 64, fp2);	//ICA基底出力, 64*64 0~255



	//sprintf(output, "PCA_BASIS.bmp"); //PCA基底bmpで出力
	//for (i = 0; i < 64; i++)
	//	for (j = 0; j < 64; j++)
	//		temp_basis[i * 64 + j] = npca_basis[i][j];
	//img_write_gray(temp_basis, output, 64, 64);

	//sprintf(output, "ICA_BASIS.bmp"); //ICA基底bmpで出力
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			temp_basis[i * 64 + j] = nica_basis[i][j];
	//img_write_gray(temp_basis, output, 64, 64);

	// ICA基底出力
	for (i = 0; i < 64; i++) {
		m = i % 8;
		n = i / 8;
		for (a = 0; a < 8; a++)
			for (b = 0; b < 8; b++)
				ica[a * 8 + b] = temp_basis[64 * 8 * m + 8 * n + b + 64 * a];
		for (a = 0; a < 8; a++)
			for (b = 0; b < 8; b++)
				for (k = 0; k < 8; k++)
					for (l = 0; l < 8; l++)
						temp_basis2[64 * 8 * a + 8 * b + l + 64 * k] = ica[a * 8 + b];

		if (8 * m + n == num2) {
			sprintf(output, "OUTPUT\\coe/coefficient[%d]/ICA[%d].bmp", num1, 8 * m + n);
			img_write_gray(temp_basis2, output, 64, 64); // outputに出力画像を書き出す
		}
		//sprintf(output, "ICA/ICA'[%d].bmp", 8 * m + n);
		//img_write_gray(temp_basis2, output, 64, 64); // outputに出力画像を書き出す
		//sprintf(output, "test/ICA[%d]/ICA[%d].bmp", 8 * m + n, 8 * m + n);
		//img_write_gray(temp_basis2, output, 64, 64); // outputに出力画像を書き出す

	}



	//for (i = 0; i < 64; i++) {
	//	for (j = 0; j < 1024; j++) {
	//		fprintf(fp3, "%lf,", yy[i][j]);	//ICA係数書き込み
	//	}
	//	fprintf(fp3, "\n");
	//}

	//for (i = 0; i < 1024; i++)
	//	for (j = 0; j < 64; j++)
	//		ny[j][i] = yy[j][i]; // ny -> yy(ica係数コピー)

	//for (i = 0; i < 64; i++)
	//	for (j = 0; j < 64; j++)
	//		nw[j][i] = w[j][i]; // nw-> w(ica基底コピー)

	//for (i = 0; i < 1024; i++)
	//	navg[i] = avg[i]; // navg -> avg(ブロックごとの平均値コピー)

	///*　再構成画像の生成（基本の流れ）　*/
	//seki5(nw, ny, x); // x -> nw * ny
	//xtogen(x, ica_sai, navg); // ica_sai -> 再構成済①
	//avg_inter(ica_sai, navg); // ica_sai -> 再構成済②
	///*　再構成画像生成終了　*/

	//fprintf(fp5, "---------- ICA ORIGIN ----------\n");
	//fprintf(fp5, "[8~15][0~7]\n");
	//for (j = 0; j < 8; j++) {
	//	for (i = 8; i < 16; i++) {
	//		fprintf(fp5, "%3d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");

	//fprintf(fp5, "---------- ICA ORIGIN ----------\n");
	//fprintf(fp5, "[0~7][8~15]\n");
	//for (j = 8; j < 16; j++) {
	//	for (i = 0; i < 8; i++) {
	//		fprintf(fp5, "%3d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");



	///*　再構成画像の生成（基本の流れ）　*/
	//seki5(nw, ny, x); // x -> nw * ny
	//xtogen(x, ica_sai, navg); // ica_sai -> 再構成済①
	//avg_inter(ica_sai, navg); // ica_sai -> 再構成済②
	///*　再構成画像生成終了　*/

	//fprintf(fp5, "---------- ICA Process ----------");
	//for (j = 0; j < 256; j++) {
	//	for (i = 0; i < 256; i++) {
	//		fprintf(fp5, "%4d, ", (int)ica_sai[i][j]);
	//	}
	//	fprintf(fp5, "\n");
	//}
	//fprintf(fp5, "\n\n");

	//fprintf(fp4, "P5\n256 256\n255\n");
	//fwrite(ica_sai, sizeof(unsigned char), 256 * 256, fp4);	//再構成画像出力


	//sprintf(output, "ICA_SAI.bmp"); //再構成画像bmpで出力
	//for (i = 0; i < 256; i++)
	//	for (j = 0; j < 256; j++)
	//		temp_sai[i * 256 + j] = ica_sai[i][j];
	//img_write_gray(temp_sai, output, 256, 256);


	/*　領域解放　*/
	// free関数で解放 > 初期化と同意
	for (i = 0; i < 64; i++) {
		free(xx[i]);
		free(yy[i]);
		free(sig[i]);
	}

	for (i = 0; i < 1024; i++) {
		free(yt[i]);
	}

	free(xx);
	free(yy);
	free(sig);
	free(avg);
	free(yt);

	/*　ファイルクローズ　*/
	//fclose(fp1);
	//fclose(fp2);
	//fclose(fp3);
	//fclose(fp4);
	//fclose(fp5);

}
