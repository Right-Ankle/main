#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "ica.h"
#define GNUPLOT_PATH "C:/Program Files/gnuplot/bin/wgnuplot.exe"

void gnuplot5(double date[64], int name) {
	char out[50];
	static char yn;

	//printf("Can you proceed ? [y/n] :");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {

		FILE* gp;
		static double max, min;
		double temp[64];

		//gp = _popen(GNUPLOT_PATH, "w");
		//printf("----plot start----\n");
		for (int j = 0; j < 64; j++) {
			temp[j] = date[j];
			if (j == 0) {
				max = temp[j];
				min = temp[j];
			}
			else {
				if (max < temp[j])
					max = temp[j];
				if (min > temp[j])
					min = temp[j];
			}
		}

		if (max < 0)
			max = max - max / 5;
		else
			max = max + max / 5;

		if (min > 0)
			min = min - min / 5;
		else
			min = min + min / 5;


		gp = _popen("gnuplot -persist", "w"); // �p�C�v���J���Agnuplot�̗����グ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplot�ɃR�}���h�𑗂�

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\MSE\\plot[%d].png", name);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min - 0.5, max + 0.5);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%lf\n", j, temp[j]);    // �f�[�^�̏�������
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // �o�b�t�@�Ɋi�[����Ă���f�[�^��f���o���i�K�{�j
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	}
}

void gnuplot5_2(double date[64], int name) {
	char out[50];
	static char yn;

		FILE* gp;
		static double max, min;
		double temp[64], origin[64];//temp=���K���z�Aorigin=�~��

		int a, b, c, d, e, i, j;
		double k, l, m, n;

		//gp = _popen(GNUPLOT_PATH, "w");

		for (i = 0; i < 64; i++)
			origin[i] = fabsf(date[i]);

		//�W���l�𐳋K���z�ɒ���
		for (i = 0; i < 64; i++) {
			max = 0;
			for (j = i; j < 64; j++)
				if (max < fabsf(origin[j])) {
					max = fabsf(origin[j]);
					a = j;
				}
			origin[a] = origin[i];
			origin[i] = max;
			if (i == 0)
				temp[32] = max;
			else if (i % 2 == 1)
				temp[32 - ((i + 1) / 2)] = max;
			else
				temp[32 + (i / 2)] = max;
		}

		// �ő�ŏ��`�F�b�N
		for (j = 0; j < 64; j++) {
			if (j == 0) {
				max = fabsf(date[j]);
				min = fabsf(date[j]);
			}
			else {
				if (max < fabsf(date[j]))
					max = fabsf(date[j]);
				if (min > fabsf(date[j]))
					min = fabsf(date[j]);
			}
		}

		if (max < 0)
			max = max - max / 5;
		else
			max = max + max / 5;

		if (min > 0)
			min = min - min / 5;
		else
			min = min + min / 5;



		gp = _popen("gnuplot -persist", "w"); // �p�C�v���J���Agnuplot�̗����グ
		//if (gp == NULL) {
		//	fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
		//	exit(EXIT_FAILURE);
		//}

		// gnuplot�ɃR�}���h�𑗂�

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\MSE\\plot[%d].png", name);
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-2:65]\n");
		fprintf(gp, "set yrange[%lf:%lf]\n", min, max);

		fprintf(gp, "plot '-' with boxes lw 1\n\n");
		//fprintf(gp, "plot '-' with lines linetype 1 \n");
		//fprintf(gp, "plot 'ICA.txt' with boxes lw 2\n\n");
		for (int j = 0; j < 64; j++)
			fprintf(gp, "%d\t%lf\n", j, temp[j]);    // �f�[�^�̏�������
		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // �o�b�t�@�Ɋi�[����Ă���f�[�^��f���o���i�K�{�j
		_pclose(gp);
		//exit(EXIT_SUCCESS);

		//��x�̌v�Z
		k = l = m = n = 0;
		for (i = 0; i < 64; i++)
			k += temp[i];
		k = k / 64; //����

		//�W���΍�
		for (i = 0; i < 64; i++)
			l += pow((temp[i] - k),2);
		l = l / 64;
		l = sqrt(l);

		for (i = 0; i < 64; i++) {
			m += (pow((temp[i]-k),4)/pow(l,4)) - ((double)(3*(64-1)*(64-1))/(double)((64-2)*(64-3)));
		}
		k = ((double)(64 * 65) / (double)(63 * 62 * 61));
		n = k * m -3;

		printf("\n [%d] = %lf", name, n);

}

void gnuplot5_4(double y[64][1024],double y2[64][1024]) {
	char out[50];
	static char yn;

	//printf("Can you proceed ? [y/n] :");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {

		FILE* gp;
		int i,j,k, l, o, n;
		static double min1, min2;
		int hist[500000];
		double hist1[500000][2];// [0]=���v���X�i�����j�C[1]=�W���l�i�����j
		double hist2[500000][2];// [0]=���v���X�i�����j�C[1]=�W���l�i�����j
		double temp_y1[64][1024];
		double temp_y2[64][1024];

		//1�R���̊m�����x
		for (i = 0; i < 500000; i++) {
			hist[i] = 0;
			hist1[i][0] = 0;
			hist1[i][1] = 0;
		}

		min1 = 10000;
		for (i = 0; i < 1024; i++)
			for (j = 0; j < 64; j++)
				if (y[j][i] < min1)
					min1 = y[j][i];

		k = 0;
		for (i = 0; i < 1024; i++)
			for (j = 0; j < 64; j++) {
				if (y[j][i] != 0) {
					hist[(int)((y[j][i] - min1) * 100) + 1]++;
					k++;
				}
			}

		for (i = 0; i < 500000; i++)
			if (hist[i] > 0) {
				hist1[i][0] = ((double)i / 100) + min1;
				hist1[i][1] = (double)hist[i] / (double)(k); //���v���X���z�@����0�@�W���΍��H1 0.5 * exp(-1.0 * fabs((double)hist[i] / (double)(a)))
			}

		//2�R���̊m�����x
		for (i = 0; i < 500000; i++) {
			hist[i] = 0;
			hist2[i][0] = 0;
			hist2[i][1] = 0;
		}

		min2 = 10000;
		for (i = 0; i < 1024; i++)
			for (j = 0; j < 64; j++)
				if (y2[j][i] < min2)
					min2 = y2[j][i];

		l = 0;
		for (i = 0; i < 1024; i++)
			for (j = 0; j < 64; j++) {
				if (y2[j][i] != 0) {
					hist[(int)((y2[j][i] - min2) * 100) + 1]++;
					l++;
				}
			}

		for (i = 0; i < 500000; i++)
			if (hist[i] > 0) {
				hist2[i][0] = ((double)i / 100) + min2;
				hist2[i][1] = (double)hist[i] / (double)(l); //���v���X���z�@����0�@�W���΍��H1 0.5 * exp(-1.0 * fabs((double)hist[i] / (double)(a)))
			}

		gp = _popen("gnuplot -persist", "w"); // �p�C�v���J���Agnuplot�̗����グ

		// gnuplot�ɃR�}���h�𑗂�

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\plot.png");
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-10:10]\n");
		fprintf(gp, "set yrange[0:0.02]\n");

		fprintf(gp, "plot '-' title 'Hybrid 1' with lines lc rgb 'dark-violet', '-' title 'Hybrid 2' w l lc rgb 'dark-green'\n\n");
		//1�ڂ̃f�[�^��������
		for (int j = 0; j < 500000; j++) {
			if (hist1[j][1] != 0)
				fprintf(gp, "%f\t%f\n", hist1[j][0], hist1[j][1]);    // �f�[�^�̏�������
		}
		fprintf(gp, "e\n");
		//2�ڂ̃f�[�^��������
		for (int j = 0; j < 500000; j++) {
			if (hist2[j][1] != 0)
				fprintf(gp, "%f\t%f\n", hist2[j][0], hist2[j][1]);    // �f�[�^�̏�������
		}

		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // �o�b�t�@�Ɋi�[����Ă���f�[�^��f���o���i�K�{�j
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	}
}

void gnuplot5_3(double y[64][1024], double ny[64][1024], double y2[64][1024], double ny2[64][1024]) {
	char out[50];
	static char yn;

	//printf("Can you proceed ? [y/n] :");
	//scanf("%s", &yn);
	yn = 'y';
	if (yn == 'y') {

		FILE* gp;
		int k, l, o, n;
		static double min1, min2;
		double temp1[65536][2];// [0]=���v���X�i�����j�C[1]=�W���l�i�����j
		double temp2[65536][2];// [0]=���v���X�i�����j�C[1]=�W���l�i�����j
		double temp_y1[64][1024];
		double temp_y2[64][1024];

		for (k = 0; k < 64; k++)
			for (l = 0; l < 1024; l++) {
				temp_y1[k][l] = y[k][l];
				temp_y2[k][l] = y2[k][l];
			}


		for (int m = 0; m < 65536; m++) {
			//������
			temp1[m][0] = temp1[m][1] = 0;
			temp2[m][0] = temp2[m][1] = 0;
			min1 = 10000;
			min2 = 10000;
			// 1�ڂ̃\�[�g
			for (int j = 0; j < 64; j++) {
				for (int i = 0; i < 1024; i++) {
					if (temp_y1[j][i] != 0 && min1 > temp_y1[j][i]) {
						min1 = y[j][i];
						temp1[m][0] = temp_y1[j][i];//temp�ɏ����Ŋi�[
						temp1[m][1] = ny[j][i];
						k = j;//�O�ŏ������p
						l = i;
					}
				}
			}
			//2�ڂ̃\�[�g
			for (int j = 0; j < 64; j++) {
				for (int i = 0; i < 1024; i++) {
					if (temp_y2[j][i] != 0 && min2 > temp_y2[j][i]) {
						min2 = y2[j][i];
						temp2[m][0] = temp_y2[j][i];//temp�ɏ����Ŋi�[
						temp2[m][1] = ny2[j][i];
						n = j;//�O�ŏ������p
						o = i;
					}
				}
			}
			temp_y1[k][l] = 0;//������
			temp_y2[n][o] = 0;//������
		}

		gp = _popen("gnuplot -persist", "w"); // �p�C�v���J���Agnuplot�̗����グ

		// gnuplot�ɃR�}���h�𑗂�

		fprintf(gp, "set terminal png\n");
		sprintf(out, "OUTPUT\\plot.png");
		fprintf(gp, "set output '%s'\n", out);
		fprintf(gp, "set boxwidth 0.5 relative\n");
		fprintf(gp, "set style fill solid\n");
		fprintf(gp, "set grid\n");
		fprintf(gp, "set xrange[-10:10]\n");
		fprintf(gp, "set yrange[0:0.6]\n");

		fprintf(gp, "plot '-' title 'Hybrid 1' with lines lc rgb 'dark-violet', '-' title 'Hybrid 2' w l lc rgb 'dark-green'\n\n");
		//1�ڂ̃f�[�^��������
		for (int j = 0; j < 65536; j++) {
			if (temp1[j][1] != 0)
				fprintf(gp, "%f\t%f\n", temp1[j][0], temp1[j][1]);    // �f�[�^�̏�������
		}
		fprintf(gp, "e\n");
		//2�ڂ̃f�[�^��������
		for (int j = 0; j < 65536; j++) {
			if (temp2[j][1] != 0)
				fprintf(gp, "%f\t%f\n", temp2[j][0], temp2[j][1]);    // �f�[�^�̏�������
		}

		fprintf(gp, "e\n");
		fprintf(gp, "set terminal windows\n\n");
		fprintf(gp, "set output\n");
		fflush(gp); // �o�b�t�@�Ɋi�[����Ă���f�[�^��f���o���i�K�{�j
		_pclose(gp);
		//exit(EXIT_SUCCESS);
	}
}