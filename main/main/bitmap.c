/* �r�b�g�}�b�v�摜�̓��o�� */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ���͉摜�̓ǂݍ��݁i�O���[�X�P�[���摜�j*/
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

	/* �t�@�C���ǂݍ��� */
	sprintf(temp, "image/%s", input);
	if ((fp = fopen(temp, "rb")) == NULL) {
		fprintf(stderr, "�t�@�C�����J���܂���ł���\n");
		exit(1);
	}

	/* �g���q���������t�@�C������Ԃ� */
	for (i = 0; input[i] != '.'; i++)
		name[i] = input[i];
	name[i] = '\0';

	/* �������m�� */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* �r�b�g�}�b�v�摜�̓ǂݍ��� */
	fread(header, 1, 54, fp);
	memcpy(&width, header + 18, sizeof(width));
	memcpy(&height, header + 22, sizeof(height));
	if (width != h_size || height != v_size) {
		printf("�w�肵���摜�T�C�Y�Ɠ��͉摜�̉摜�T�C�Y���قȂ��Ă��܂�\n");
		return -1;
	}
	memcpy(&bitcount, header + 28, sizeof(bitcount));
	if (bitcount != 8) {
		printf("���͉摜�̓O���[�X�P�[���摜�ł͂���܂���\n");
		return -1;
	}
	fread(palette, 1, 1024, fp);
	fread(data, 1, h_size * v_size, fp);
	fclose(fp);

	/* bmp�`���ł͐F�z�񂪍������E��ɂȂ��Ă���̂ŁC���と�E���ɕ␳���� */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			G[(((v_size - 1) - i) * h_size) + j] = (int)data[i * h_size + j];

	/* ��������� */
	free(data);

	return 0;
}

/* ���͉摜�̓ǂݍ��݁i�J���[�摜�j*/
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

	/* �t�@�C���ǂݍ��� */
	sprintf(temp, "%s", input);
	if ((fp = fopen(temp, "rb")) == NULL) {
		fprintf(stderr, "�t�@�C�����J���܂���ł���\n");
		exit(1);
	}

	/* �g���q���������t�@�C������Ԃ� */
	for (i = 0; input[i] != '.'; i++)
		name[i] = input[i];
	name[i] = '\0';

	/* �������m�� */
	data = (unsigned char *)malloc(sizeof(unsigned char) * (h_size * v_size * 3));
	R_temp = (int *)malloc(sizeof(int) * h_size * v_size);
	G_temp = (int *)malloc(sizeof(int) * h_size * v_size);
	B_temp = (int *)malloc(sizeof(int) * h_size * v_size);

	/* �r�b�g�}�b�v�摜�̓ǂݍ��� */
	fread(header, 1, 54, fp);
	memcpy(&width, header + 18, sizeof(width));
	memcpy(&height, header + 22, sizeof(height));
	if (width != h_size || height != v_size) {
		printf("�w�肵���摜�T�C�Y�Ɠ��͉摜�̉摜�T�C�Y���قȂ��Ă��܂�\n");
		return -1;
	}
	memcpy(&bitcount, header + 28, sizeof(bitcount));
	if (bitcount != 24) {
		printf("���͉摜�̓J���[�摜�ł͂���܂���\n");
		return -1;
	}
	fread(data, 1, h_size * v_size * 3, fp);
	fclose(fp);

	/* RGB������ʁX�̔z��Ɋi�[ */
	for (i = 0; i < h_size * v_size * 3; i += 3) {
		B_temp[i / 3] = (int)data[i];
		G_temp[i / 3] = (int)data[i + 1];
		R_temp[i / 3] = (int)data[i + 2];
	}

	/* bmp�`���ł͐F�z�񂪍������E��ɂȂ��Ă���̂ŁC���と�E���ɕ␳���� */
	for (i = 0; i < v_size; i++) {
		for (j = 0; j < h_size; j++) {
			R[(((v_size - 1) - i) * h_size) + j] = R_temp[i * h_size + j];
			G[(((v_size - 1) - i) * h_size) + j] = G_temp[i * h_size + j];
			B[(((v_size - 1) - i) * h_size) + j] = B_temp[i * h_size + j];
		}
	}

	/* ��������� */
	free(data);
	free(R_temp);
	free(G_temp);
	free(B_temp);

	return 0;
}

/* �o�͉摜�̏����o���i�O���[�X�P�[���摜�j*/
void img_write_gray(int *G, char *output, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *data;
	int i, j;

	/* �w�b�_�[���̈����̒�`�Ɛݒ� */
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
		fprintf(stderr, "�t�@�C�����J���܂���ł���\n");
		exit(1);
	}

	/* �t�@�C���w�b�_�[ */
	memcpy(Header + 0, &bfType, sizeof(bfType));
	memcpy(Header + 2, &bfSize, sizeof(bfSize));
	memcpy(Header + 6, &bfReserved1, sizeof(bfReserved1));
	memcpy(Header + 8, &bfReserved2, sizeof(bfReserved2));
	memcpy(Header + 10, &bfOffBits, sizeof(bfOffBits));

	/* ���w�b�_�[ */
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

	/* �p���b�g�f�[�^ */
	for (i = 0; i < 256; i++) {
		Palette[i * 4] = i;
		Palette[i * 4 + 1] = i;
		Palette[i * 4 + 2] = i;
		Palette[i * 4 + 3] = 0;
	}

	/* �������m�� */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* �F�z������と�E������Cbmp�`���p�ɍ������E��ɒ��� */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			data[(((v_size - 1) - i) * h_size) + j] = G[i * h_size + j];

	/* �t�@�C�������o�� */
	fwrite(Header, 1, 54, fp);
	fwrite(Palette, 1, 1024, fp);
	for (i = 0; i < h_size * v_size; i++)
		fwrite(&data[i], 1, 1, fp);

	/* ��������� */
	free(data);

	fclose(fp);
}

/* �o�͉摜�̏����o���i�J���[�摜�j*/
void img_write_color(int *R, int *G, int *B, char *output, int h_size, int v_size)
{
	FILE *fp;
	unsigned char *R_temp, *G_temp, *B_temp;
	int i, j;

	/* �w�b�_�[���̈����̒�`�Ɛݒ� */
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
		fprintf(stderr, "�t�@�C�����J���܂���ł���\n");
		exit(1);
	}

	/* �t�@�C���w�b�_�[ */
	memcpy(Header + 0, &bfType, sizeof(bfType));
	memcpy(Header + 2, &bfSize, sizeof(bfSize));
	memcpy(Header + 6, &bfReserved1, sizeof(bfReserved1));
	memcpy(Header + 8, &bfReserved2, sizeof(bfReserved2));
	memcpy(Header + 10, &bfOffBits, sizeof(bfOffBits));

	/* ���w�b�_�[ */
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

	/* �������m�� */
	R_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);
	G_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);
	B_temp = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* �F�z������と�E������Cbmp�`���p�ɍ������E��ɒ��� */
	for (i = 0; i < v_size; i++) {
		for (j = 0; j < h_size; j++) {
			R_temp[(((v_size - 1) - i) * h_size) + j] = R[i * h_size + j];
			G_temp[(((v_size - 1) - i) * h_size) + j] = G[i * h_size + j];
			B_temp[(((v_size - 1) - i) * h_size) + j] = B[i * h_size + j];
		}
	}

	/* �t�@�C�������o�� */
	fwrite(Header, 1, 54, fp);
	for (i = 0; i < h_size * v_size; i++) {
		fwrite(&B_temp[i], 1, 1, fp);
		fwrite(&G_temp[i], 1, 1, fp);
		fwrite(&R_temp[i], 1, 1, fp);
	}

	/* ��������� */
	free(R_temp);
	free(G_temp);
	free(B_temp);

	fclose(fp);
}