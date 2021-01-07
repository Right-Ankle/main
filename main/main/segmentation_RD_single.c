/* segmentation_RD_single.c */

#define _CRT_SECURE_NO_WARNINGS	//scanfとfopenでエラーが出るので書きました

#include<stdio.h>
#include<math.h>
#include "ica.h"

#define STEP 10

void segmentation_RD_single(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int QQ){
	FILE *fp;
	static int i, j, k, l, m, n, Q = 0;

	static double ytemp[64][1024] = { 0 }, x[64][1024] = { 0 };
	static unsigned char ica_sai[256][256] = { 0 }, dct_sai[256][256] = { 0 };
	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 };

	//ブロック毎のR（エントロピー）とD（MSE）
	static double dctent[1024] = { 0 }, icaent[64][1024] = { 0 };
	static double dctmse[1024] = { 0 }, icamse[64][1024] = { 0 };

	static double dctcosttemp[100 / STEP][1024] = { 0 }, icacosttemp[64][1024] = { 0 };
	static double min;

	static struct c dctcost[100 / STEP][1024] = { 0 }, icacost[64][1024] = { 0 }, temp;
	static struct t costsumtotal[100 / STEP] = { 0 }, temp2;	//コスト関数の総和の退避用配列

	static int block_flag_temp[100 / STEP][1024] = { 0 };

	for (i = 0; i < 1024; i++){
		for (j = 0; j < (100 / STEP); j++){
			dctcost[j][i].num = i;
			dctcost[j][i].r_num = (j + 1) * (100 / STEP);
			dctcost[j][i].rate = 0;
			dctcost[j][i].dis = 0;
			dctcost[j][i].cost = 0;
		}

		for (j = 0; j < 64; j++){
			icacost[j][i].num = i;
			icacost[j][i].r_num = 63 - j + 1;
			icacost[j][i].rate = 0;
			icacost[j][i].dis = 0;
			icacost[j][i].cost = 0;
		}
	}

	for (i = 0; i < (100 / STEP); i++){
		costsumtotal[i].num = i;
		costsumtotal[i].val = 0;
	}


	if ((fp = fopen("RD.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}


	/* DCT単独のコスト関数を求める */
	for (Q = 100; Q > 0; Q -= STEP){
		dct(origin, coe, 8);
		quantization(coe, Q);
		idct(coe, dct_sai, 8);

		//Rを求める
		b_entropy_dct(coe, dctent);

		//Dを求める
		b_mse(origin, dct_sai, dctmse);

		make_cost(dctent, dctmse, dctcosttemp[Q / STEP - 1]);

		for (i = 0; i < 1024; i++){
			dctcost[Q / STEP - 1][i].rate = dctent[i];
			dctcost[Q / STEP - 1][i].dis = dctmse[i];
			dctcost[Q / STEP - 1][i].cost = dctcosttemp[Q / STEP - 1][i];
		}
	}

	/* ICA単独のコスト関数を求める */
	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++)
			ytemp[i][j] = y[i][j];

	//ブロック毎の64通りのコスト関数を作成
	for (i = 0; i < 64; i++){
		seki(w, ytemp, x);
		xtogen(x, ica_sai, avg);
		avg_inter(ica_sai, avg);

		//Rを求める
		b_entropy_ica(ytemp, icaent[i]);

		//Dを求める
		b_mse(origin, ica_sai, icamse[i]);

		make_cost(icaent[i], icamse[i], icacosttemp[i]);

		for (j = 0; j < 1024; j++){
			icacost[i][j].rate = icaent[i][j];
			icacost[i][j].dis = icamse[i][j];
			icacost[i][j].cost = icacosttemp[i][j];
		}

		//基底を削減
		for (j = 0; j < 1024; j++){
			ytemp[mpans[j][63 - i]][j] = 0;
		}
	}


	///*出力確認 ここから*/	///////////////////////////////////

	fprintf(fp, "Q,R,D,コスト,");
	fprintf(fp, "\n");
	for (i = 0; i < 1024; i++){
		//DCT
		fprintf(fp, "DCT,");
		fprintf(fp, "%d,", dctcost[0][i].num);
		fprintf(fp, "\n");
		for (j = 0; j < 10; j++){
			fprintf(fp, "%d,", dctcost[j][i].r_num);
			fprintf(fp, "%lf,", dctcost[j][i].rate);
			fprintf(fp, "%lf,", dctcost[j][i].dis);
			fprintf(fp, "%lf,", dctcost[j][i].cost);
			fprintf(fp, "\n");
		}

		//ICA
		fprintf(fp, "ICA,");
		fprintf(fp, "%d,", icacost[0][i].num);
		fprintf(fp, "\n");
		for (j = 0; j < 64; j++){
			fprintf(fp, "%d,", icacost[j][i].r_num);
			fprintf(fp, "%lf,", icacost[j][i].rate);
			fprintf(fp, "%lf,", icacost[j][i].dis);
			fprintf(fp, "%lf,", icacost[j][i].cost);
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}

	///*出力確認 ここまで*/	///////////////////////////////////


	//ICAをブロック毎にコストの昇順に並び替え
	for (n = 0; n < 1024; n++){
		for (i = 0; i < 64 - 1; i++) {
			min = icacost[i][n].cost;			/* i 番目の要素を暫定的に最小値とし */
			k = i;								/* 添字を保存 */
			for (j = i + 1; j < 64; j++) {
				if (icacost[j][n].cost < min) {	/* より小さい値が現れたら */
					min = icacost[j][n].cost;	/* 最小値の入れ替え */
					k = j;						/* 添字を保存 */
				}
			}									/* このループを抜けるとk に最小値の添字が入っている */
			temp = icacost[i][n];				/* i 番目の要素と最小値の交換 */
			icacost[i][n] = icacost[k][n];
			icacost[k][n] = temp;
		}
	}

	///*出力確認 ここから*/	///////////////////////////////////

	fprintf(fp, "Q,R,D,コスト,");
	fprintf(fp, "\n");
	for (i = 0; i < 1024; i++){
		//DCT
		fprintf(fp, "DCT,");
		fprintf(fp, "%d,", dctcost[0][i].num);
		fprintf(fp, "\n");
		for (j = 0; j < 10; j++){
			fprintf(fp, "%d,", dctcost[j][i].r_num);
			fprintf(fp, "%lf,", dctcost[j][i].rate);
			fprintf(fp, "%lf,", dctcost[j][i].dis);
			fprintf(fp, "%lf,", dctcost[j][i].cost);
			fprintf(fp, "\n");
		}

		//ICA
		fprintf(fp, "ICA,");
		fprintf(fp, "%d,", icacost[0][i].num);
		fprintf(fp, "\n");
		for (j = 0; j < 64; j++){
			fprintf(fp, "%d,", icacost[j][i].r_num);
			fprintf(fp, "%lf,", icacost[j][i].rate);
			fprintf(fp, "%lf,", icacost[j][i].dis);
			fprintf(fp, "%lf,", icacost[j][i].cost);
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}

	///*出力確認 ここまで*/	///////////////////////////////////



	//ブロック毎のコスト関数の比較
	for (n = 0; n < (100 / STEP); n++){
		for (i = 0; i < 1024; i++) {
			if (icacost[0][i].cost <= dctcost[n][i].cost){
				block_flag_temp[n][i] = 1;
			}
			else{
				block_flag_temp[n][i] = 0;
			}
		}
	}


	//DCTレートごとのコスト関数の総和を求める
	for (n = 0; n < (100 / STEP); n++){
		for (i = 0; i < 1024; i++){
			if (block_flag_temp[n][i] == 1){
				costsumtotal[n].val += icacost[0][i].cost;
			}
			else{
				costsumtotal[n].val += dctcost[n][i].cost;
			}
		}
	}

	///*出力確認 ここから*/	///////////////////////////////////

	for (i = 0; i < 10; i++){
		fprintf(fp, "%d,", costsumtotal[i].num);
		fprintf(fp, "%lf,", costsumtotal[i].val);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	///*出力確認 ここまで*/	///////////////////////////////////

	//最小となるコスト関数の総和
	for (i = 0; i < (100 / STEP) - 1; i++) {
		min = costsumtotal[i].val;			/* i 番目の要素を暫定的に最小値とし */
		k = i;							/* 添字を保存 */
		for (j = i + 1; j < (100 / STEP); j++) {
			if (costsumtotal[j].val < min) {	/* より小さい値が現れたら */
				min = costsumtotal[j].val;	/* 最小値の入れ替え */
				k = j;					/* 添字を保存 */
			}
		}								/* このループを抜けるとk に最小値の添字が入っている */
		temp2 = costsumtotal[i];				/* i 番目の要素と最小値の交換 */
		costsumtotal[i] = costsumtotal[k];
		costsumtotal[k] = temp2;
	}


	/* 最終的なブロック分類 */
	for (i = 0; i < 1024; i++){
		if (block_flag_temp[costsumtotal[0].num][i] == 1){
			block_flag[i] = 1;
		}
		else{
			block_flag[i] = 0;
		}
	}

	/* 分類画像作成 */
	get_seg_image(origin, block_flag, Q);



	/* 符号化性能の比較 *////////////////////////////////////
	static double optmse = 0.0;
	static double optrate = 0.0;
	static double segpsnr = 0.0;
	static int QQQ;

	static int hist[2][50000];
	static double DC_rate = 0.0, entro[2] = { 0 };
	static double coef[64][1024] = { 0 }, mini[2];
	static int number[2];

	for (i = 0; i < 2; i++){
		entro[i] = 0;
		number[i] = 0;
	}
	optmse = 0.0;
	optrate = 0.0;
	segpsnr = 0.0;

	//dc
	for (i = 0; i < 50000; i++)
		hist[0][i] = hist[1][i] = 0;

	gentox2(coe, coef);
	mini[0] = coef[0][0];
	mini[1] = avg[0];
	for (i = 1; i < 1024; i++) {
		if (block_flag_temp[costsumtotal[0].num][i] == 0){
			if (coef[0][i] < mini[0])
				mini[0] = coef[0][i];
		}
		if (block_flag_temp[costsumtotal[0].num][i] == 1){
			if (avg[i] < mini[1])
				mini[1] = avg[i];
		}
	}


	for (i = 0; i < 1024; i++) {
		if (block_flag_temp[costsumtotal[0].num][i] == 0) {
			hist[0][(int)(coef[0][i] - mini[0]) + 1]++;
			number[0]++;
		}
		if (block_flag_temp[costsumtotal[0].num][i] == 1) {
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
	for (i = 0; i < 1024; i++){
		if (block_flag_temp[costsumtotal[0].num][i] == 1){
			optmse += icacost[0][i].dis;
			optrate += icacost[0][i].rate;
		}
		else{
			optmse += dctcost[costsumtotal[0].num][i].dis;
			optrate += dctcost[costsumtotal[0].num][i].rate;
		}
	}

	optmse = optmse / (256 * 256);
	optrate = (DC_rate + optrate) / (256 * 256);

	segpsnr = 10.0 * log10(pow(255, 2) / optmse);

	//fprintf(fp, "Proposed Method\n");
	//fprintf(fp, "Conventional Method\n");
	//fprintf(fp, "Q\tMSE\tbitrate\tPSNR\n");

	QQQ = (int)(costsumtotal[0].num + 1) * STEP;
	fprintf(fp, "%lf\t", lambda);
	fprintf(fp, "%d\t", QQQ);
	//fprintf(fp, "%lf\t", optmse);
	fprintf(fp, "%lf\t", optrate);
	fprintf(fp, "%lf\t", segpsnr);
	fprintf(fp, "\n");

	/////////////////////////////////////////////////////////

	printf("[%d]\t%lf\t%lf\n", QQQ, optrate, segpsnr);

	/*出力確認 ここから*/	///////////////////////////////////




	for (n = 0; n < 10; n++){
		fprintf(fp, "Q,R,D,コスト,");
		fprintf(fp, "[%d],", n);
		fprintf(fp, "best-[%d],", costsumtotal[0].num);
		fprintf(fp, "\n");

		for (i = 0; i < 1024; i++){
			//DCT
			fprintf(fp, "DCT,");
			fprintf(fp, "%d,", dctcost[n][i].num);
			fprintf(fp, "\n");
			fprintf(fp, "%d,", dctcost[n][i].r_num);
			fprintf(fp, "%lf,", dctcost[n][i].rate);
			fprintf(fp, "%lf,", dctcost[n][i].dis);
			fprintf(fp, "%lf,", dctcost[n][i].cost);
			fprintf(fp, "\n");
			//ICA
			fprintf(fp, "ICA,");
			fprintf(fp, "%d,", icacost[0][i].num);
			fprintf(fp, "\n");
			fprintf(fp, "%d,", icacost[0][i].r_num);
			fprintf(fp, "%lf,", icacost[0][i].rate);
			fprintf(fp, "%lf,", icacost[0][i].dis);
			fprintf(fp, "%lf,", icacost[0][i].cost);
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}

	/*出力確認 ここまで*/	///////////////////////////////////



	fclose(fp);

}