//���͂��ꂽ�摜���̐F�̕t�����u���b�N�̔ԍ���CSV�t�@�C���ŏo��

#include <stdio.h>
#include "ica.h"

void Block_count(char name[20]) {
	FILE *fp;
	static char filename[100];
	static char image_name[20] = { 0 };	//�摜�t�@�C����(�g���q�܂܂�)
	static unsigned char origin[256][256] = { 0 };	//���摜�i256*256�̂ݑΉ�)
	static double x[64][1024] = { 0 }; //���摜�i64*1024�j
	double sum;
	static int ori_temp[256 * 256] = { 0 }; // ���摜(256*256�̈ꎟ��)
	static int count[1024] = { 0 };
	int i, j, k, l, m, n;

	//�t�@�C�����Ɗg���q�̌���
	//strcpy(image_name, name);
	sprintf(filename, "%s.bmp",name);
	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
		return -1;

	//�o�̓t�@�C���̃t�@�C�������쐬
	sprintf(filename, "OUTPUT\\Result\\Block_num[%s].csv", image_name);
	if ((fp = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	/* �ꎟ���z�񂩂�񎟌��z��ɕϊ� */
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

	//�u���b�N�ԍ����J�E���g
	for (i = 0; i < 1024; i++) {
		sum = 0;
		count[i] = 0;
		for (j = 0; j < 64; j++) {
			sum += x[j][i];
		}
		if (sum != 0)
			fprintf(fp, "%d\n", i);//�ԍ���CSV�ɏ����o��
		else
			fprintf(fp, "%d\n", 9999);//�ΏۈȊO��9999�Ŗ��߂�
	}

	//�t�@�C���o��
	fclose(fp);

}