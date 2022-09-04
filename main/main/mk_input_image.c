//CSV(�u���b�N�ԍ��̂ݏ�����Ă���)��ǂݍ��݁A�����ꂽ�u���b�N�ԍ��݂̂��F�Â����摜���o��

#include <stdio.h>
#include <math.h>
#include "ica.h"

void mk_input_image(char origin[256][256], char name[20]) {
	FILE *fp;
	static char moji[10];//�擾������������i�[
	static char filename[100];
	static int block_flag[1024];//�o�͂���u���b�N�̔ԍ��Ƀt���O�𗧂Ă�
	static int block[1024]; //�e�u���b�N���o�͂���ۂ̌`��i���̔ԍ��j���i�[�@�i�ʏ�͔z��Ɠ����ԍ������A0��99���i�[����Ă���΁A0�Ԃ�99�Ԃ̌`��ŏo�͂����j
	char block_image[256][256]; //�����܂�������̉摜
	static double x[64][1024] = { 0 }; //���摜�i64*1024�j
	static double origin_temp[64][1024]; //�z��R�s�[�p
	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;

	//	�ǂݍ��݃��[�h�Ńt�@�C�����J��
	sprintf(filename, "image\\���͉摜�ύX�����p\\Cameraman\\Block_num[%s111].csv", name);
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Can not open file\n");
	}

	//������
	for (i = 0; i < 1024; i++)
		block_flag[i] = 9999;

	//	NULL�|�C���^�̏I�[�܂Ńt�@�C�����當����1�s���ǂݍ���
	i = 0;
	while (fgets(moji, 10, fp) != NULL)
	{
		//	�ǂݍ��񂾕�����𐔎��ɕϊ����A�Ώۂ̃u���b�N�ԍ��Ƀt���O�𗧂Ă�
		block_flag[i] = atoi(moji);
		i++;
	}
	fclose(fp);

	//�u���b�N����64�ȉ��������ꍇ�̂����܂�����
	for (i = 0; i < 1024; i++) {
		if (block_flag[i] != 9999)
			k++;
		block[i] = block_flag[i];
	}

	if (k < 64) {
		//�u���b�N����64�ȏ�ɂȂ�܂ł����܂��������s��
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

	//�t���O�̗����Ă���ԍ��̌��摜�̌`����R�s�[

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

	//�Ώۂ̌��摜���R�s�[�܂��͊i�[
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

	//�o��
	sprintf(name, "%s111", name);
	img_out5(block_image, atoi(name));
}