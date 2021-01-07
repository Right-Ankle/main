/* segmentation_ent_out.c */

#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"
// 領域分類をもとにエントロピーを出力
void ent_out(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], double y_flag[64][1024], int block_flag[1024], int Q) {
	FILE* fp;
	static int i, j, k, l, m, n, xx, yy, sw = 0;
	static int mp_key[1024] = { 0 };

	static double minent = 0;
	static int minL0 = 0;

	static unsigned char ica_sai[256][256] = { 0 }, dct_sai[256][256] = { 0 };

	static double ytemp[64][1024] = { 0 }, x[64][1024] = { 0 }, b_psnr[1024] = { 0 };
	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 };

	static double qdct_p_map[1024] = { 0 }, qica_p_map[64][1024] = { 0 };
	static double qdct_ent_map[1024] = { 0 }, qica_ent_map[64][1024] = { 0 };

	/* 出力用 */
	static char output[20];
	static unsigned char seg[32][32] = { 0 }, seg2[256][256] = { 0 };
	static int temp_basis[32 * 32], temp_basis2[256 * 256];
	static double tmp[32][32] = { 0 }, tmp2[256][256] = { 0 }, tmp3[64][1024] = { 0 }, oritmp[64][1024];
	static int block = 0, sub = 0;


	struct t icaver[1024] = { 0 }, icaske[1024] = { 0 }, temp;
	static double b_ver_ica[1024] = { 0 }, b_ske_ica[1024] = { 0 }, max;
	static double yori[64][1024] = { 0 };
	static int out[1024] = { 0 };


	if ((fp = fopen("OUTPUT\\RD.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	///////////////////////////////////////////////////////////////////////////////

	/*コスト関数を求める*/////////////////////////////////////////////////
	//ブロック毎のR（エントロピー）とD（MSE）
	static double dctent[1024] = { 0 }, icaent[1024] = { 0 };
	static double dctmse[1024] = { 0 }, icamse[1024] = { 0 };

	for (i = 0; i < 1024; i++) {
		dctent[i] = 0;
		dctmse[i] = 0;
		icaent[i] = 0;
		icamse[i] = 0;
	}

	//Rを求める
	dct(origin, coe, 8);
	quantization(coe, Q);
	idct(coe, dct_sai, 8);
	b_entropy_dct(coe, dctent);

	//Dを求める
	b_mse(origin, dct_sai, dctmse);


	//Rを求める
	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++)
			ytemp[i][j] = y_flag[i][j];

	b_entropy_ica(ytemp, icaent);

	//Dを求める
	seki(w, ytemp, x);
	xtogen(x, ica_sai, avg);
	avg_inter(ica_sai, avg);
	b_mse(origin, ica_sai, icamse);

	////////////////////////////////////////////////

	/* 符号化性能の比較 *////////////////////////////////////
	static double optmse = 0.0;
	static double optrate = 0.0;
	static double segpsnr = 0.0;

	static int hist[2][50000];
	static double DC_rate = 0.0, entro[2] = { 0 };
	static double coef[64][1024] = { 0 }, mini[2];
	static int number[2];

	for (i = 0; i < 2; i++) {
		entro[i] = 0;
		number[i] = 0;
	}
	optmse = 0.0;
	optrate = 0.0;
	segpsnr = 0.0;

	//dc
	for (i = 0; i < 50000; i++) {
		hist[0][i] = hist[1][i] = 0;
	}

	gentox2(coe, coef);
	mini[0] = coef[0][0];
	mini[1] = avg[0];

	for (i = 1; i < 1024; i++) {
		if (block_flag[i] == 0) {
			if (coef[0][i] < mini[0])
				mini[0] = coef[0][i];
		}
		if (block_flag[i] == 1) {
			if (avg[i] < mini[1])
				mini[1] = avg[i];
		}
	}

	for (i = 0; i < 1024; i++) {
		if (block_flag[i] == 0) {
			hist[0][(int)(coef[0][i] - mini[0]) + 1]++;
			number[0]++;
		}
		if (block_flag[i] == 1) {
			hist[1][(int)(avg[i] - mini[1]) + 1]++;
			number[1]++;
		}
	}


	for (i = 0; i < 50000; i++) {
		if (hist[0][i] > 0)
			entro[0] += -((double)hist[0][i] / number[0]) * (log((double)hist[0][i] / number[0]) / log(2));
		if (hist[1][i] > 0)
			entro[1] += -((double)hist[1][i] / number[1]) * (log((double)hist[1][i] / number[1]) / log(2));
	}

	DC_rate = entro[0] * number[0] + entro[1] * number[1];



	//ac

	for (i = 0; i < 1024; i++) {
		if (block_flag[i] == 1) {
			optmse += icamse[i];
			optrate += icaent[i];
		}
		else {
			optmse += dctmse[i];
			optrate += dctent[i];
		}
	}

	optmse = optmse / (256 * 256);
	optrate = (DC_rate + optrate) / (256 * 256);

	segpsnr = 10.0 * log10(pow(255, 2) / optmse);

	fprintf(fp, "%d\t", Q);
	fprintf(fp, "%lf\t", optrate);
	fprintf(fp, "%lf\t", segpsnr);
	fprintf(fp, "\n");

	printf("[%d]\t%lf\t%lf\n", Q, optrate, segpsnr); //optrate -> 情報量

	/////////////////////////////////////////////////////////

	fclose(fp);
}