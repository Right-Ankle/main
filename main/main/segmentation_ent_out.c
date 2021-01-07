/* segmentation_ent_out.c */

#define _CRT_SECURE_NO_WARNINGS	//scanf��fopen�ŃG���[���o��̂ŏ����܂���

#include<stdio.h>
#include<math.h>
#include "ica.h"

void segmentation_ent_out(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int Q){
	FILE *fp;
	static int i, j, k, l, m, n, xx, yy, sw = 0;
	static int mp_key[1024] = { 0 };

	static double minent = 0;
	static int minL0 = 0;

	static unsigned char ica_sai[256][256] = { 0 }, dct_sai[256][256] = { 0 };

	static double ytemp[64][1024] = { 0 }, x[64][1024] = { 0 }, b_psnr[1024] = { 0 };
	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 };

	static double qdct_p_map[1024] = { 0 }, qica_p_map[64][1024] = { 0 };
	static double qdct_ent_map[1024] = { 0 }, qica_ent_map[64][1024] = { 0 };

	/* �o�͗p */
	static char output[20];
	static unsigned char seg[32][32] = { 0 }, seg2[256][256] = { 0 };
	static int temp_basis[32 * 32], temp_basis2[256 * 256];
	static double tmp[32][32] = { 0 }, tmp2[256][256] = { 0 }, tmp3[64][1024] = { 0 }, oritmp[64][1024];
	static int block = 0, sub = 0;


	struct t icaver[1024] = { 0 }, icaske[1024] = { 0 },temp;
	static double b_ver_ica[1024] = { 0 }, b_ske_ica[1024] = { 0 }, max;
	static double yori[64][1024] = { 0 };
	static int out[1024] = { 0 };


	if ((fp = fopen("OUTPUT\\RD.txt", "a")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	///////////////////////////////////////////////////////////////////////////////

	/* qica_p_map��qica_e_map�̍쐬 */
	for (i = 0; i < 64; i++){
		for (j = 0; j < 1024; j++){
			ytemp[i][j] = y[i][j];
			yori[i][j] = y[i][j];
		}
	}

	for (i = 0; i < 64; i++){

		for (j = 0; j < 1024; j++){
			ytemp[mpans[j][63 - i]][j] = 0;
		}

		seki(w, ytemp, x);
		xtogen(x, ica_sai, avg);
		avg_inter(ica_sai, avg);
		block_psnr(origin, ica_sai, b_psnr);

		for (j = 0; j < 1024; j++){
			qica_p_map[i][j] = b_psnr[j];
		}

		b_entropy_ica(ytemp, qica_ent_map[i]);

	}

	/* qdct_p_map�̍쐬 */
	dct(origin, coe, 8);
	quantization(coe, Q);
	idct(coe, dct_sai, 8);

	block_psnr(origin, dct_sai, b_psnr);

	for (i = 0; i < 1024; i++){
		qdct_p_map[i] = b_psnr[i];
	}

	/* qdct_e_map�̍쐬 */
	b_entropy_dct(coe, qdct_ent_map);



	/* mp_key�̍쐬 */
	for (i = 0; i < 1024; i++){
		minent = 1000;
		for (j = 0; j < 64; j++){
			if (qdct_p_map[i] <= qica_p_map[j][i]){
				if (minent > qica_ent_map[j][i]){
					mp_key[i] = j;
					minent = qica_ent_map[j][i];
				}
			}
		}
	}

	/* mp_key�Ɋ�Â�ICA�W���팸 */
	for (i = 0; i < 1024; i++)
		for (j = 0; j < mp_key[i]; j++)
			y[mpans[i][63 - j]][i] = 0;

	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++)
			ytemp[i][j] = y[i][j];

	seki(w, ytemp, x);
	b_entropy_ica(ytemp, qica_ent_map[0]);

	/* �u���b�N���� */
	for (i = 0; i < 1024; i++){
		if (qica_ent_map[0][i] <= qdct_ent_map[i]){
			block_flag[i] = 1;
		}
		else{
			block_flag[i] = 0;
		}
	}


	/* ���މ摜�쐬  ������l */
	for (i = 0; i < 32; i++){
		for (j = 0; j < 32; j++){
			if (block_flag[i * 32 + j] == 1){

				//������l
				tmp[i][j] = 255;
			}
			else{
				tmp[i][j] = 0;
			}

			//32x32 => 256x256
			for (m = 0; m < 8; m++){
				for (n = 0; n < 8; n++){
					tmp2[i * 8 + m][j * 8 + n] = tmp[i][j];
					seg2[i * 8 + m][j * 8 + n] = (unsigned char)(tmp2[i * 8 + m][j * 8 + n]);
				}
			}
		}
	}

	/* ���މ摜�쐬  ICA_Block */
	get_seg_image(origin, block_flag, Q);



	/*�R�X�g�֐������߂�*/////////////////////////////////////////////////
	//�u���b�N����R�i�G���g���s�[�j��D�iMSE�j
	static double dctent[1024] = { 0 }, icaent[1024] = { 0 };
	static double dctmse[1024] = { 0 }, icamse[1024] = { 0 };

	for (i = 0; i < 1024; i++){
		dctent[i] = 0;
		dctmse[i] = 0;
		icaent[i] = 0;
		icamse[i] = 0;
	}

	//R�����߂�
	dct(origin, coe, 8);
	quantization(coe, Q);
	idct(coe, dct_sai, 8);
	b_entropy_dct(coe, dctent);

	//D�����߂�
	b_mse(origin, dct_sai, dctmse);


	//R�����߂�
	for (i = 0; i < 64; i++)
		for (j = 0; j < 1024; j++)
			ytemp[i][j] = y[i][j];

	b_entropy_ica(ytemp, icaent);

	//D�����߂�
	seki(w, ytemp, x);
	xtogen(x, ica_sai, avg);
	avg_inter(ica_sai, avg);
	b_mse(origin, ica_sai, icamse);

	////////////////////////////////////////////////

	/* ���������\�̔�r *////////////////////////////////////
	static double optmse = 0.0;
	static double optrate = 0.0;
	static double segpsnr = 0.0;

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
	for (i = 0; i < 50000; i++){
		hist[0][i] = hist[1][i] = 0;
	}

	gentox2(coe, coef);
	mini[0] = coef[0][0];
	mini[1] = avg[0];

	for (i = 1; i < 1024; i++) {
		if (block_flag[i] == 0){
			if (coef[0][i] < mini[0])
				mini[0] = coef[0][i];
		}
		if (block_flag[i] == 1){
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

	for (i = 0; i < 1024; i++){
		if (block_flag[i] == 1){
			optmse += icamse[i];
			optrate += icaent[i];
		}
		else{
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

	printf("[%d]\t%lf\t%lf\n", Q, optrate, segpsnr); //optrate -> ����

	/////////////////////////////////////////////////////////



	/* ����bmp�ŏo�� */
	sprintf(output, "OUTPUT\\seg.bmp");
	for (i = 0; i < 256; i++){
		for (j = 0; j < 256; j++){
			temp_basis2[i * 256 + j] = seg2[i][j];
		}
	}
	img_write_gray(temp_basis2, output, 256, 256);

	fclose(fp);
}