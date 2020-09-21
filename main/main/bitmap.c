/* ビットマップ画像の入出力 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 入力画像の読み込み（グレースケール画像）*/
int img_read_gray(int *G, char *input, char *name, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *data;
	unsigned char header[54];
	unsigned char palette[1024];
	long width, height;
	unsigned int bitcount;
	char temp[80];
	int i, j;

	/* ファイル読み込み */
	sprintf(temp, "image/%s", input);
	if ((fp = fopen(temp, "rb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* 拡張子を除いたファイル名を返す */
	for (i = 0; input[i] != '.'; i++)
		name[i] = input[i];
	name[i] = '\0';

	/* メモリ確保 */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* ビットマップ画像の読み込み */
	fread(header, 1, 54, fp);
	memcpy(&width, header + 18, sizeof(width));
	memcpy(&height, header + 22, sizeof(height));
	if (width != h_size || height != v_size) {
		printf("指定した画像サイズと入力画像の画像サイズが異なっています\n");
		return -1;
	}
	memcpy(&bitcount, header + 28, sizeof(bitcount));
	if (bitcount != 8) {
		printf("入力画像はグレースケール画像ではありません\n");
		return -1;
	}
	fread(palette, 1, 1024, fp);
	fread(data, 1, h_size * v_size, fp);
	fclose(fp);

	/* bmp形式では色配列が左下→右上になっているので，左上→右下に補正する */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			G[(((v_size - 1) - i) * h_size) + j] = (int)data[i * h_size + j];

	/* メモリ解放 */
	free(data);

	return 0;
}

/* 入力画像の読み込み（カラー画像）*/
int img_read_color(int *R, int *G, int *B, char *input, char *name, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *data;
	unsigned char header[54];
	long width, height;
	unsigned int bitcount;
	int *R_temp, *G_temp, *B_temp;
	char temp[80];
	int i, j;

	/* ファイル読み込み */
	sprintf(temp, "%s", input);
	if ((fp = fopen(temp, "rb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* 拡張子を除いたファイル名を返す */
	for (i = 0; input[i] != '.'; i++)
		name[i] = input[i];
	name[i] = '\0';

	/* メモリ確保 */
	data = (unsigned char *)malloc(sizeof(unsigned char) * (h_size * v_size * 3));
	R_temp = (int *)malloc(sizeof(int) * h_size * v_size);
	G_temp = (int *)malloc(sizeof(int) * h_size * v_size);
	B_temp = (int *)malloc(sizeof(int) * h_size * v_size);

	/* ビットマップ画像の読み込み */
	fread(header, 1, 54, fp);
	memcpy(&width, header + 18, sizeof(width));
	memcpy(&height, header + 22, sizeof(height));
	if (width != h_size || height != v_size) {
		printf("指定した画像サイズと入力画像の画像サイズが異なっています\n");
		return -1;
	}
	memcpy(&bitcount, header + 28, sizeof(bitcount));
	if (bitcount != 24) {
		printf("入力画像はカラー画像ではありません\n");
		return -1;
	}
	fread(data, 1, h_size * v_size * 3, fp);
	fclose(fp);

	/* RGB成分を別々の配列に格納 */
	for (i = 0; i < h_size * v_size * 3; i += 3) {
		B_temp[i / 3] = (int)data[i];
		G_temp[i / 3] = (int)data[i + 1];
		R_temp[i / 3] = (int)data[i + 2];
	}

	/* bmp形式では色配列が左下→右上になっているので，左上→右下に補正する */
	for (i = 0; i < v_size; i++) {
		for (j = 0; j < h_size; j++) {
			R[(((v_size - 1) - i) * h_size) + j] = R_temp[i * h_size + j];
			G[(((v_size - 1) - i) * h_size) + j] = G_temp[i * h_size + j];
			B[(((v_size - 1) - i) * h_size) + j] = B_temp[i * h_size + j];
		}
	}

	/* メモリ解放 */
	free(data);
	free(R_temp);
	free(G_temp);
	free(B_temp);

	return 0;
}

/* 出力画像の書き出し（グレースケール画像）*/
void img_write_gray(int *G, char *output, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *data;
	int i, j;

	/* ヘッダー情報の引数の定義と設定 */
	unsigned char Header[54];
	unsigned int bfType = 'MB';
	unsigned long bfSize = h_size * v_size + 54 + 1024;
	unsigned int bfReserved1 = 0;
	unsigned int bfReserved2 = 0;
	unsigned long bfOffBits = 54 + 1024;
	unsigned long biSize = 40;
	long biWidth = h_size;
	long biHeight = v_size;
	unsigned int biPlanes = 1;
	unsigned int biBitCount = 8;
	unsigned long biCompression = 0;
	unsigned long biSizeImage = h_size * v_size;
	long biXPixPerMeter = 0;
	long biYPixPerMeter = 0;
	unsigned long biClrUsed = 0;
	unsigned long biClrImportant = 0;
	unsigned char Palette[1024];

	if ((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* ファイルヘッダー */
	memcpy(Header + 0, &bfType, sizeof(bfType));
	memcpy(Header + 2, &bfSize, sizeof(bfSize));
	memcpy(Header + 6, &bfReserved1, sizeof(bfReserved1));
	memcpy(Header + 8, &bfReserved2, sizeof(bfReserved2));
	memcpy(Header + 10, &bfOffBits, sizeof(bfOffBits));

	/* 情報ヘッダー */
	memcpy(Header + 14, &biSize, sizeof(biSize));
	memcpy(Header + 18, &biWidth, sizeof(biWidth));
	memcpy(Header + 22, &biHeight, sizeof(biHeight));
	memcpy(Header + 26, &biPlanes, sizeof(biPlanes));
	memcpy(Header + 28, &biBitCount, sizeof(biBitCount));
	memcpy(Header + 30, &biCompression, sizeof(biCompression));
	memcpy(Header + 34, &biSizeImage, sizeof(biSizeImage));
	memcpy(Header + 38, &biXPixPerMeter, sizeof(biXPixPerMeter));
	memcpy(Header + 42, &biYPixPerMeter, sizeof(biYPixPerMeter));
	memcpy(Header + 46, &biClrUsed, sizeof(biClrUsed));
	memcpy(Header + 50, &biClrImportant, sizeof(biClrImportant));

	/* パレットデータ */
	for (i = 0; i < 256; i++) {
		Palette[i * 4] = i;
		Palette[i * 4 + 1] = i;
		Palette[i * 4 + 2] = i;
		Palette[i * 4 + 3] = 0;
	}

	/* メモリ確保 */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* 色配列を左上→右下から，bmp形式用に左下→右上に直す */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			data[(((v_size - 1) - i) * h_size) + j] = G[i * h_size + j];

	/* ファイル書き出し */
	fwrite(Header, 1, 54, fp);
	fwrite(Palette, 1, 1024, fp);
	for (i = 0; i < h_size * v_size; i++)
		fwrite(&data[i], 1, 1, fp);

	/* メモリ解放 */
	free(data);

	fclose(fp);
}

/* 出力画像の書き出し（カラー画像）*/
void img_write_color(int *R, int *G, int *B, char *output, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *R_temp, *G_temp, *B_temp;
	int i, j;

	/* ヘッダー情報の引数の定義と設定 */
	unsigned char Header[54];
	unsigned int bfType = 'MB';
	unsigned long bfSize = h_size * v_size + 54;
	unsigned int bfReserved1 = 0;
	unsigned int bfReserved2 = 0;
	unsigned long bfOffBits = 54;
	unsigned long biSize = 40;
	long biWidth = h_size;
	long biHeight = v_size;
	unsigned int biPlanes = 1;
	unsigned int biBitCount = 24;
	unsigned long biCompression = 0;
	unsigned long biSizeImage = h_size * v_size;
	long biXPixPerMeter = 0;
	long biYPixPerMeter = 0;
	unsigned long biClrUsed = 0;
	unsigned long biClrImportant = 0;

	if ((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* ファイルヘッダー */
	memcpy(Header + 0, &bfType, sizeof(bfType));
	memcpy(Header + 2, &bfSize, sizeof(bfSize));
	memcpy(Header + 6, &bfReserved1, sizeof(bfReserved1));
	memcpy(Header + 8, &bfReserved2, sizeof(bfReserved2));
	memcpy(Header + 10, &bfOffBits, sizeof(bfOffBits));

	/* 情報ヘッダー */
	memcpy(Header + 14, &biSize, sizeof(biSize));
	memcpy(Header + 18, &biWidth, sizeof(biWidth));
	memcpy(Header + 22, &biHeight, sizeof(biHeight));
	memcpy(Header + 26, &biPlanes, sizeof(biPlanes));
	memcpy(Header + 28, &biBitCount, sizeof(biBitCount));
	memcpy(Header + 30, &biCompression, sizeof(biCompression));
	memcpy(Header + 34, &biSizeImage, sizeof(biSizeImage));
	memcpy(Header + 38, &biXPixPerMeter, sizeof(biXPixPerMeter));
	memcpy(Header + 42, &biYPixPerMeter, sizeof(biYPixPerMeter));
	memcpy(Header + 46, &biClrUsed, sizeof(biClrUsed));
	memcpy(Header + 50, &biClrImportant, sizeof(biClrImportant));

	/* メモリ確保 */
	R_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);
	G_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);
	B_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* 色配列を左上→右下から，bmp形式用に左下→右上に直す */
	for (i = 0; i < v_size; i++) {
		for (j = 0; j < h_size; j++) {
			R_temp[(((v_size - 1) - i) * h_size) + j] = R[i * h_size + j];
			G_temp[(((v_size - 1) - i) * h_size) + j] = G[i * h_size + j];
			B_temp[(((v_size - 1) - i) * h_size) + j] = B[i * h_size + j];
		}
	}

	/* ファイル書き出し */
	fwrite(Header, 1, 54, fp);
	for (i = 0; i < h_size * v_size; i++) {
		fwrite(&B_temp[i], 1, 1, fp);
		fwrite(&G_temp[i], 1, 1, fp);
		fwrite(&R_temp[i], 1, 1, fp);
	}

	/* メモリ解放 */
	free(R_temp);
	free(G_temp);
	free(B_temp);

	fclose(fp);
}