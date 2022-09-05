//入力された画像内の色の付いたブロックの番号をCSVファイルで出力

#include <stdio.h>
#include "ica.h"

void Block_count(char name[20]) {
	FILE *fp;
	static char filename[100];
	static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応)
	static double x[64][1024] = { 0 }; //原画像（64*1024）
	double sum;
	static int ori_temp[256 * 256] = { 0 }; // 原画像(256*256の一次元)
	static int count[1024] = { 0 };
	int i, j, k, l, m, n;

	//ファイル名と拡張子の結合
	//strcpy(image_name, name);
	sprintf(filename, "%s.bmp",name);
	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	//出力ファイルのファイル名を作成
	sprintf(filename, "OUTPUT\\Result\\Block_num[%s].csv", image_name);
	if ((fp = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	/* 一次元配列から二次元配列に変換 */
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			origin[i][j] = ori_temp[i * 256 + j];

	//256*256->64*1024
	n = 0;
	for (i = 0; i < 256; i += 8) {
		for (j = 0; j < 256; j += 8) {
			m = 0;
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {
					x[m][n] = origin[i + k][j + l]; //256*256 -> 64*1024
					m++;
				}
			}
			n++;
		}
	}

	//ブロック番号をカウント
	for (i = 0; i < 1024; i++) {
		sum = 0;
		count[i] = 0;
		for (j = 0; j < 64; j++) {
			sum += x[j][i];
		}
		if (sum != 0)
			fprintf(fp, "%d\n", i);//番号をCSVに書き出す
		else
			fprintf(fp, "%d\n", 9999);//対象以外は9999で埋める
	}

	//ファイル出力
	fclose(fp);

}