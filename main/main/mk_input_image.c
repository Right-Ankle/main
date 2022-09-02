//CSV(ブロック番号のみ書かれている)を読み込み、書かれたブロック番号のみが色づいた画像を出力

#include <stdio.h>
#include <math.h>
#include "ica.h"

void mk_input_image(char origin[256][256]) {
	FILE *fp;
	char moji[10];//取得した文字列を格納
	int block_flag[1024];//出力するブロックの番号にフラグを立てる
	int block[1024]; //各ブロックを出力する際の形状（元の番号）を格納　（通常は配列と同じ番号だが、0に99が格納されていれば、0番は99番の形状で出力される）
	char block_image[256][256]; //かさまし処理後の画像
	static double x[64][1024] = { 0 }; //原画像（64*1024）
	static double origin_temp[64][1024]; //配列コピー用
	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;

	//	読み込みモードでファイルを開く
	if ((fp = fopen("image\\Block_num[80].csv", "r")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	//初期化
	for (i = 0; i < 1024; i++)
		block_flag[i] = 9999;

	//	NULLポインタの終端までファイルから文字を1行ずつ読み込む
	while (fgets(moji, 10, fp) != NULL)
	{
		//	読み込んだ文字列を数字に変換し、対象のブロック番号にフラグを立てる
		block_flag[atoi(moji)] = atoi(moji);
	}

	fclose(fp);

	//ブロック数が64個以下だった場合のかさまし処理
	for (i = 0; i < 1024; i++) {
		if (block_flag[i] != 9999)
			k++;
		block[i] = block_flag[i];
	}

	if (k < 64) {
		//ブロック数が64以上になるまでかさまし処理を行う
		for (i = 0; i < ceil(64.0 / (double)k); i++) {
			for (j = 0; j < 1024; j++) {
				if (block[l] == 9999 && block_flag[j] != 9999) {
					block[l] = block_flag[j];
					l++;
				}
				else if (block[l] != 9999)
					l++;
			}
		}
	}

	//フラグの立っている番号の原画像の形状をコピー

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

	//対象の原画像をコピーまたは格納
	for (i = 0; i < 1024; i++) {
		for (j = 0; j < 64; j++) {
			if (block[i] != 9999)
				origin_temp[j][i] = x[j][block[i]];
			else
				origin_temp[j][i] = 0;
		}
	}

	//64*1024->256*256
	xtogen2(origin_temp, block_image);

	//出力
	img_out5(block_image, 49);
}