//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "ica.h"
//
//int main()
//{
//	FILE* fp; //�t�@�C���|�C���^�̒�`
//
//	static unsigned char origin[256][256] = { 0 };	//���摜�i256*256�̂ݑΉ��j
//	//static  double ori_temp2[64][1024] = { 0 };
//	static int i, j, n, m, k, l, mk, ml, Q, QQ, b, a, ori_temp[256 * 256];
//	static double sum, min, mse_dct[64][1024], mse_ica[64][1024];
//	sum = 0;
//
//	static char image_name[20] = { 0 };	//�摜�t�@�C����(�g���q�܂܂�)
//	char output[20];
//
//	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
//
//	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
//		return -1;
//
//	/* �ꎟ���z�񂩂�񎟌��z��ɕϊ� */
//	for (i = 0; i < 256; i++)
//		for (j = 0; j < 256; j++)
//			origin[i][j] = ori_temp[i * 256 + j];
//
//	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 };
//	static double avg[1024], y[64][1024], w[64][64], x[64][1024], dcoe_temp2[64][1024], dcoe_temp[64][1024] = { 0 };
//	static unsigned char dammy[256][256] = { 0 };
//	static unsigned char block_dct[64], dcoe2[256][256] = { 0 };
//	static unsigned char  ica_sai[256][256] = { 0 };
//	static struct pca pcaStr = { 0 };
//
//	//�\�[�g
//	struct tt** dy, temp;
//	dy = (struct tt**)malloc(sizeof(struct tt*) * 64);
//	for (i = 0; i < 64; i++) {
//		dy[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
//	}
//
//	if ((fp = fopen("test_dct.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	//gentox(origin, ori_temp2);
//
//   //�蓮�ɂ��Q�̑I��
//   //printf("Choose Q? (y->1 / n->0):");
//   // scanf("%d", &QQ);
//   // if (QQ == 1)
//		//for (QQ = 10; QQ <= 100; QQ += 10)
//
//	printf("Choose Q? (y->1 / n->0):");
//	scanf("%d", &Q);
//	printf("Q use %d\n", Q);
//	//fprintf(fp, "\n\n------------Use Q [%d]------------\n", Q);
//
//	dct(origin, dcoe, 8);
//	quantization(dcoe, Q);
//	idct(dcoe, dcoe2, 8);
//	dctcoe_conv(dcoe, dcoe_temp);//256*256->64*1024
//
//	//pcaStr = new_pca(origin);
//	//ICA(origin, pcaStr, y, w, avg, 100, 0.002);
//
//
//
//	//dct
//	for (Q = 100; Q > 0; Q -= 10) {
//		printf("Now Q  [%d]\n\n", Q);
//		fprintf(fp, "\n\n\n------------Use Q [%d]------------\n", Q);
//		for (j = 0; j < 1024; j++) {
//			//fprintf(fp, "\n\n------------BlockNum[%d]------------\n", j+1);
//			for (i = 0; i < 64; i++) {
//				if (i == 0)
//					//fprintf(fp, "\nInnerNum[%d][%d]\n", i, j);
//					b = i;
//				// �Y���W���ȊO0
//				for (a = 0; a < 64; a++) {
//					if (b == a) {
//						dcoe_temp2[a][j] = dcoe_temp[a][j];
//						//y[a][j] = 0;
//					}
//					else {
//						dcoe_temp2[a][j] = 0;
//					}
//					//fprintf(fp, "num[%d][%d] : %lf,  %lf",a , j, dcoe_temp[a][j], dcoe_temp2[a][j]);
//					//if(b == a)
//					//	fprintf(fp, "<--");
//					//fprintf(fp, "\n");
//				}
//
//				idct_Block(dcoe_temp2, block_dct, 8, j);  //�Ώ�block�����̍č\��
//
//			   // �u���b�N���Ƃ�MSE
//				sum = 0.0;
//				mk = j % 32;
//				ml = j / 32;
//
//				for (a = 0; a < 8; a++) {
//					for (b = 0; b < 8; b++) {
//						sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_dct[a * 8 + b], 2);
//						if (dcoe_temp2[0][j] != 0) {
//							//fprintf(fp, "num[%d][%d] : %d , %d", ml * 8 + b, mk * 8 + a, origin[ml * 8 + b][mk * 8 + a], block_dct[a * 8 + b]);
//							//fprintf(fp, "\n");
//						}
//					}
//				}
//				mse_dct[i][j] = sum / 64;
//			}
//
//			//printf("%d\n", j);
//		}
//
//		// �o��
//
//		//fprintf(fp, "-----DCT MSE-----");
//		fprintf(fp, "\n\n");
//
//
//
//		for (i = 0; i < 64; i++) {
//			for (j = 0; j < 1024; j++) {
//				// .val -> �l���擾�E������ύX���L��
//				// .abs -> ��Βl���L��
//				// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//				dy[i][j].val = mse_dct[i][j];		/* ���X�̌W���l */
//				dy[i][j].abs = fabs(mse_dct[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//				dy[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//			}
//		}
//
//		for (n = 0; n < 1024; n++) {
//			for (i = 0; i < 64 - 1; i++) {
//				min = dy[i][n].abs;
//				k = i;
//				for (j = i + 1; j < 64; j++) {
//					if (mse_dct[j][n] < min) {
//						min = dy[j][n].abs;
//						k = j;
//					}
//				}
//				temp = dy[i][n];
//				dy[i][n] = dy[k][n];
//				dy[k][n] = temp;
//			}
//		}
//		//��ʌ܌�
//		//for (i = 0; i < 1024; i++) {
//		//	fprintf(fp, "�u���b�N%4d : ", i + 1);
//		//	for (j = 0; j < 5; j++) {
//		//		fprintf(fp, "%2d, ", dy[j][i].num);
//		//	}
//		//	fprintf(fp, "\n");
//		//}
//		//��ʂP�i���ԍ��j
//		fprintf(fp, "\n<MSE Num>\n");
//		for (i = 0; i < 1024; i++) {
//			fprintf(fp, "%2d, ", dy[0][i].num);
//			if (i % 32 == 31)
//				fprintf(fp, "\n");
//		}
//
//		//��ʂP���iMSE�l�j
//		//fprintf(fp, "\n<MSE Val>\n");
//		//for (i = 0; i < 1024; i++) {
//		//	fprintf(fp, "%5d, ", (int)dy[0][i].val);
//		//	if (i % 32 == 31)
//		//		fprintf(fp, "\n");
//		//}
//	}
//	fclose(fp);
//	for (i = 0; i < 64; i++) {
//		free(dy[i]);
//	}
//	free(dy);
//	printf("finish");
//
//}

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "ica.h"
//
//int main()
//{
//	FILE* fp, * fp2, * fp3; //�t�@�C���|�C���^�̒�`
//
//	//�錾
//	static unsigned char origin[256][256] = { 0 };	//���摜�i256*256�̂ݑΉ��j
//	//static  double ori_temp2[64][1024] = { 0 };
//	static int i, j, n, m, k, l, mk, ml, Q, QQ, b, a, c, ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai2[64][1024], temp_sai3[256][256];
//	static double sum, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[64][1024], mse_ica[64][1024], mse_ica2[64][1024], result_dct[2][1024], result_ica[2][1024], result[2][1024];
//	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 };
//	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dcoe_temp[64][1024] = { 0 };
//	static unsigned char dammy[256][256] = { 0 };
//	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
//	static unsigned char  ica_sai[256][256] = { 0 };
//	static struct pca pcaStr = { 0 };
//	static char image_name[20] = { 0 };	//�摜�t�@�C����(�g���q�܂܂�)
//	char output[20];
//	struct tt** sort_d, temp;
//
//	sum = 0;
//
//	//�ǂݍ��ރt�@�C����
//	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
//
//	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
//		return -1;
//
//	/* �ꎟ���z�񂩂�񎟌��z��ɕϊ� */
//	for (i = 0; i < 256; i++)
//		for (j = 0; j < 256; j++)
//			origin[i][j] = ori_temp[i * 256 + j];
//
//
//	//�\�[�g�p
//	sort_d = (struct tt**)malloc(sizeof(struct tt*) * 64);
//	for (i = 0; i < 64; i++) {
//		sort_d[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
//	}
//
//	//�o�̓t�@�C���@�錾
//	if ((fp = fopen("test_dct.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp2 = fopen("test_ica.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp3 = fopen("result_ica.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//
//	/////////////////�錾���� �I��///////////////////////
//
//
//	// ///////////////////////// ica //////////////////////////////////
//	// ICA���E�W��
//	// origin = ��f�l(256*256),  y = ica�W��(�u���b�N��64��1024�u���b�N��),  w = ica���(64�̌v�Z�@�̒��ɂ��ꂼ��64�̌v�Z�@������)
//	// ICA��"origin"�����邱�Ƃ�"y"(�v�Z��̒l)��"w"(�v�Z�̎d��)�̌��ʂ��o�͂����
//	// ���͌v�Z���@�B�W���� 8*8�̉�f�u���b�N���\������̂� 64�̊�ꂪ���ꂼ�� �ǂꂭ�炢�g���Ă���̂��i�܂܂�Ă��邩�j�̒l�B
//	// �u���b�N�Ƃ� 256*256��f�̂����c8��8�̃u���b�N�B��摜�ɂ�(256/8) 32*32 = 1024�u���b�N
//	pcaStr = new_pca(origin);
//	ICA(origin, pcaStr, y, w, avg, 100, 0.002);
//
//	// �v�Z�p�ɃR�s�[
//	for (i = 0; i < 1024; i++)
//		for (j = 0; j < 64; j++)
//			ny[j][i] = y[j][i]; // ny -> yy(ica�W���R�s�[)
//
//	for (i = 0; i < 64; i++)
//		for (j = 0; j < 64; j++)
//			nw[j][i] = w[j][i]; // nw-> w(ica���R�s�[)
//
//	// ���ʂ̍č\�������iICA�ŕ�������"y"��"w"����"origin"���č\���j
//	//�i�֌W�Ȃ��j
//	seki5(nw, ny, x); // x -> nw * ny
//	xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//	// 64�̊��̂���1�����g�p���邽�߂̏���
//	// ���͂�����Ȃ�����A�g�p����W����64����1�ɐ�������B
//	// �u���b�N���Ŏg�p�������1�~64���邩��64�p�^�[���~�S1024�u���b�N
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			b = i;
//
//			// �Y���W���ȊO0
//			// i�Ԗڂ̌W���i���j�̂ݎg�p�B����ȊO�̌W����0�B
//			for (a = 0; a < 64; a++) {
//				if (b == a)
//					ny[a][j] = y[a][j];
//				else
//					ny[a][j] = 0;
//
//				// �W���m�F�i���Ȃ��j
//				//fprintf(fp, "num[%d][%d] : %lf,  %lf", a, j, ny[a][j], y[a][j]);
//				//if (b == a)
//				//	fprintf(fp, "<--");
//				//fprintf(fp, "\n");
//			}
//
//			// �������i�K���s���j
//			for (a = 0; a < 64; a++)
//				xx[a] = 0.0;
//
//			// 1�u���b�N�ŏ������s���Ă��邽�߁A���̃u���b�N�ԍ��i���j�̌W����
//			// ���ׂĂ̊���p���邱�Ƃ� ���Ƃ̃u���b�N���č\�����鏈��
//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> �č\���χ@
//			avg_inter_Block(block_ica, avg, j); // ica_sai -> �č\���χA
//
//
//			// �u���b�N���Ƃ�MSE
//			// MSE�́i���̒l - �č\���̒l�j^2�����邱�Ƃ�
//			// �č\�������l�����̒l�Ƃǂꂭ�炢����Ă���̂������邽�߂̎w�W
//			sum = 0.0;
//			sum2 = 0.0;
//			mk = j % 32;
//			ml = j / 32;
//
//			// 64��2��̕��ς��炻�̃u���b�N�����ς��Ăǂꂭ�炢 ����Ă���̂�������
//			// �i���Ȃ݂ɁA1�u���b�N�ɂ�64�p�^�[�����S1024�u���b�N�j
//			for (a = 0; a < 8; a++) {
//				for (b = 0; b < 8; b++) {
//					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
//					sum2 += pow(origin[ml * 8 + b][mk * 8 + a] - ica_sai[ml * 8 + b][mk * 8 + a], 2);
//				}
//
//			}
//			mse_ica[i][j] = sum / 64;//����
//			mse_ica2[i][j] = sum2 / 64;
//		}
//		// ���s�m�F�p
//		printf("%d\n", j);
//	}
//
//	//for (j = 0; j < 1024; j++)
//	//	for (i = 0; i < 64; i++)
//	//		fprintf(fp, "num[%d][%d] : %d , %d\n", i,j, (int)mse_ica[i][j], (int)mse_ica2[i][j]);
//
//	// ///////////// ica MSE �\�[�g/////////////////////////
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = mse_ica[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(mse_ica[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			min = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (mse_ica[j][n] < min) {
//					min = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//	////////////////////////////�\�[�g�I��////////////////////////////////
//
//	//�\�[�g���ʂ��i�[
//	// �e�u���b�N64�p�^�[���̒��ň�Ԍ덷�̂Ȃ�����
//	for (i = 0; i < 1024; i++) {
//		result_ica[0][i] = sort_d[0][i].num;  // ���ԍ�
//		result_ica[1][i] = sort_d[0][i].val;    // MSE�l
//	}
//
//	for (i = 0; i < 1024; i++) {
//		for (a = 0; a < 64; a++) {
//			if (result_ica[0][i] == a)
//				ny[a][i] = y[a][i];
//			else
//				ny[a][i] = 0;
//
//			// �W���m�F�i���Ȃ��j
//			//fprintf(fp, "num[%d][%d] : %lf,  %lf", a, j, ny[a][j], y[a][j]);
//			//if (b == a)
//			//	fprintf(fp, "<--");
//			//fprintf(fp, "\n");
//		}
//	}
//
//	seki5(nw, ny, x); // x -> nw * ny
//	xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = ica_sai[a][b];
//
//	sprintf(output, "DCT/ICA_sai.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//
//
//	// ICA�m�F
//	//fprintf(fp2, "\n<Num ICA>\n");
//	//for (i = 0; i < 1024; i++) {
//	//	fprintf(fp2, "%2d, ", (int)result_ica[1][i]);
//	//	if (i % 32 == 31) {
//	//		fprintf(fp2, "\n");
//	//	}
//	//}
//	//fprintf(fp2, "\n");
//
//	//����m�F
//	printf("<ica fin>\n\n");
//	/////////////////////////////////ica �I��/////////////////////////////////////////
//
//
//	// //////////////////////////// dct ////////////////////////////////////////
//	// ICA �Ƒ�̓����BDCT�̊��͔ėp�I�����猈�܂��Ă���B�W���݂̂𓮂����΂���
//
//	//����m�F
//	printf("<dct start>\n");
//
//	// 10�i�K�i�������邩��10�i�K�����
//	for (Q = 100; Q > 0; Q -= 10) {
//		printf("now Q is %d\n", Q);
//		//Q = 100;
//
//			// dct����
//		dct(origin, dcoe, 8); // �W�����o��
//		quantization(dcoe, Q); // �W���̕i����10�i�K�ŗ��Ƃ������i�ʎq���j���Ƃ��Η��Ƃ��قǃf�[�^�͌y���Ȃ邪�A�i����������
//		idct(dcoe, dcoe2, 8); // ���ʂ̍č\��
//
//
//
//		// 256*256->64*1024�i�W���̏��Ԃ�ύX)
//		//  256*256����0�Ԗڂ̃u���b�N�̒l�� [0~7]*[0~7]�Ɋi�[����Ă���i1�Ԗڂ���[0~7]*[8~15]) ���A
//		//  64*1024�ɂ����[0~64]*[0]�ƌv�Z���₷���E�킩��₷���Ȃ�
//		dctcoe_conv(dcoe, dcoe_temp);
//
//		//for (j = 0; j < 1024; j++) {
//
//			//// �W���������i�K���s���j
//		for (a = 0; a < 256; a++) {
//			for (c = 0; c < 256; c++) {
//				coe_temp[c][a] = dcoe[c][a];
//			}
//		}
//
//		// �Y���W���ȊO0
//		for (b = 0; b < 256; b++) {
//			for (a = 0; a < 256; a++) {
//				if (b % 8 != 0 || a % 8 != 0)
//					coe_temp[b][a] = 0.0;
//
//			}
//		}
//
//		gentox(dcoe_temp2, temp_sai3);
//		idct(coe_temp, dcoe3, 8); // ���ʂ̍č\��
//		//idct_Block(dcoe_temp2, block_dct, 8, j);  //�Ώ�block�����̍č\��
//
//
//			//for (a = 0; a < 64; a++)
//			//	temp_sai2[a][j] = block_dct[a];
//
//
//	   // �u���b�N���Ƃ�MSE
//		sum = 0.0;
//		sum2 = 0.0;
//		mk = j % 32;
//		ml = j / 32;
//
//		//for (a = 0; a < 8; a++) {
//		//	for (b = 0; b < 8; b++) {
//		//		//sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_dct[a * 8 + b], 2);
//		//		//fprintf(fp, "num[%d][%d] : %d\n",ml * 8 + b, mk * 8 + a, block_dct[a * 8 + b]);
//		//		//fprintf(fp2, "num[%d][%d] : %d , %d , %d\n", ml * 8 + b, mk * 8 + a, dcoe2[ml * 8 + b][mk * 8 + a], block_dct[a * 8 + b], origin[ml * 8 + b][mk * 8 + a]);
//		//		//sum2 += pow(origin[ml * 8 + b][mk * 8 + a] - dcoe2[ml * 8 + b][mk * 8 + a], 2);
//		//		//fprintf(fp, "num[0][%4d] : %3d , %3d\n", j, (int)dcoe2[ml * 8 + b][mk * 8 + a], (int)block_dct[a * 8 + b]);
//		//	}
//		//}
//		//mse_dct[0][j] = sum / 64.0;
//		//mse_dct2[0][j] = sum2 / 64;
//
//		// ���s�m�F�p
//		//printf("%d\n", j);
//	//}
//
//
//
//	// /////////�\�[�g//////////////
//	// dct�\�[�g
//	//for (i = 0; i < 64; i++) {
//	//	for (j = 0; j < 1024; j++) {
//	//		// .val -> �l���擾�E������ύX���L��
//	//		// .abs -> ��Βl���L��
//	//		// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//	//		sort_d[i][j].val = mse_dct[i][j];		/* ���X�̌W���l */
//	//		sort_d[i][j].abs = fabs(mse_dct[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//	//		sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//	//	}
//	//}
//
//	//for (n = 0; n < 1024; n++) {
//	//	for (i = 0; i < 64 - 1; i++) {
//	//		min = sort_d[i][n].abs;
//	//		k = i;
//	//		for (j = i + 1; j < 64; j++) {
//	//			if (mse_dct[j][n] < min) {
//	//				min = sort_d[j][n].abs;
//	//				k = j;
//	//			}
//	//		}
//	//		temp = sort_d[i][n];
//	//		sort_d[i][n] = sort_d[k][n];
//	//		sort_d[k][n] = temp;
//	//	}
//	//}
//	/////////////////////�\�[�g�I��////////////////////////
//
//	// �\�[�g���ʂ��i�[
//		for (i = 0; i < 1024; i++) {
//			result_dct[0][i] = 0.0;
//			result_dct[1][i] = mse_dct[0][i];
//		}
//		/////////////////////////dct �I��/////////////////////////////
//
//		fprintf(fp2, "<USE Q = %d>\n", Q);
//		sum = 0.0;
//		sum2 = 0.0;
//		for (i = 0; i < 64; i++)
//			count3[i] = 0;
//
//		for (j = 0; j < 1024; j++) {
//			fprintf(fp2, "num[0][%4d] : %8.2lf , %8.2lf   -->   %8.2lf", j, result_dct[1][j], result_ica[1][j], result_dct[1][j] - result_ica[1][j]);
//			count[j] = 0;
//			count2[j] = 0;
//			if ((result_dct[1][j] - result_ica[1][j]) < 10.0) {
//				count2[j]++;
//				count3[(int)result_ica[0][i]]++;
//				if ((result_dct[1][j] - result_ica[1][j]) < 5.0) {
//					count[j] = 1;
//					sum++;
//				}
//				sum2++;
//				if ((result_dct[1][j] - result_ica[1][j]) < 5.0)
//					fprintf(fp2, "<------------------------------------- [  5 or less]\n");
//				else
//					fprintf(fp2, "<------------------------------------- [10 or less]\n");
//			}
//			else
//				fprintf(fp2, "\n");
//		}
//		fprintf(fp2, "\n\n\n");
//
//		fprintf(fp, "\n\n");
//		for (i = 0; i < 64; i++) {
//			//fprintf(fp, "[%2d] (%3d)   |", i, count3[i]);
//			//for (a = 0; a < count3[i]; a++)
//				//fprintf(fp, " �� ");
//			//fprintf(fp, "\n");
//		}
//		//fprintf(fp, "\n\n");
//
//		if (Q == 100) {
//			for (a = 0; a < 256; a++)
//				for (b = 0; b < 256; b++)
//					temp_sai[a * 256 + b] = dcoe3[a][b];
//
//			sprintf(output, "DCT/DCT_sai3.bmp");
//			img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//			for (a = 0; a < 256; a++)
//				for (b = 0; b < 256; b++)
//					temp_sai[a * 256 + b] = dcoe2[a][b];
//
//			sprintf(output, "DCT/DCT_sai.bmp");
//			img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//		}
//
//		for (k = 0; k < 32; k++) {
//			for (l = 0; l < 32; l++) {
//				sum = 0.0;
//				for (i = 0; i < 8; i++) {
//					for (j = 0; j < 8; j++) {
//						sum += pow(origin[i + (8 * k)][j + (8 * l)] - dcoe3[i + (8 * k)][j + (8 * l)], 2);
//					}
//				}
//				best_dct[k * 32 + l] = sum / 64;//����
//			}
//		}
//
//		for (k = 0; k < 32; k++) {
//			for (l = 0; l < 32; l++) {
//				sum = 0.0;
//				for (i = 0; i < 8; i++) {
//					for (j = 0; j < 8; j++) {
//						sum += pow(origin[i + (8 * k)][j + (8 * l)] - ica_sai[i + (8 * k)][j + (8 * l)], 2);
//					}
//				}
//				best_ica[k * 32 + l] = sum / 64;//����
//			}
//		}
//		for (i = 0; i < 1024; i++) {
//			if (best_ica[i] < best_dct[i])
//				fprintf(fp, " 1,");
//			else
//				fprintf(fp, " 0,");
//			if (i % 32 == 31) {
//				fprintf(fp, "\n");
//			}
//
//		}
//
//		fprintf(fp3, "        < UNDER 5 [Q = %d] -->  [%2.0lf/1024]  >\n\n", Q, sum);
//		fprintf(fp3, "        < UNDER 10 [Q = %d] -->  [%2.0lf/1024]  >\n\n", Q, sum2);
//		fprintf(fp3, "        < UNDER 5 & 10  [Q = %d] -->  [%2.0lf/1024]  >\n\n\n\n", Q, sum + sum2);
//
//		for (i = 0; i < 64; i++)
//			count3[i] = 0;
//
//		for (i = 0; i < 1024; i++) {
//			fprintf(fp3, "%d,", count[i]);
//			count3[(int)result_ica[0][i]]++;
//			if (i % 32 == 31) {
//				fprintf(fp3, "\n");
//			}
//		}
//		fprintf(fp3, "\n\n");
//
//		for (i = 0; i < 1024; i++) {
//			fprintf(fp3, "%d,", count2[i]);
//			if (i % 32 == 31) {
//				fprintf(fp3, "\n");
//			}
//		}
//		fprintf(fp3, "\n\n");
//
//		/*for (i = 0; i < 1024; i++) {
//			fprintf(fp, "%2d,", (int)result_ica[0][i]);
//			if (i % 32 == 31) {
//				fprintf(fp, "\n");
//			}
//		}*/
//		fprintf(fp, "\n\n");
//
//		for (i = 0; i < 64; i++) {
//			fprintf(fp3, "[%2d] (%3d)   |", i, count3[i]);
//			for (a = 0; a < count3[i]; a++)
//				fprintf(fp3, " �� ");
//			fprintf(fp3, "\n");
//		}
//		fprintf(fp3, "\n\n");
//
//		for (i = 0; i < 64 - 1; i++) {
//			max = (double)count3[i];
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if ((double)count3[j] > max) {
//					max = (double)count3[j];
//					k = j;
//				}
//			}
//			QQ = count3[i];
//			count3[i] = count3[k];
//			count3[k] = QQ;
//		}
//
//
//		//fprintf(fp3, "--------------<ICA Top 5>------------------\n");
//		//for (i = 0; i < 64; i++) {
//		//	fprintf(fp3, "[%2d] (%2d)   |", i, count3[i]);
//		//	for (a = 0; a < count3[i]; a++)
//		//		fprintf(fp3, " �� ");
//		//	fprintf(fp3, "\n");
//		//}
//		//fprintf(fp3, "\n\n");
//
//
//
//
//			//for (i = 0; i < 1024; i++) {
//			//	fprintf(fp2, "%lf, %lf\n", result_dct[1][i], result_ica[1][i]);
//			//	if (i % 32 == 31) {
//			//		fprintf(fp, "\n");
//			//	}
//			//}
//
//
//			// mse���ʂ̔�r
//		for (i = 0; i < 1024; i++) {
//			if (mse_dct[0][i] < result_ica[1][i]) { // dct�̂ق������������
//				result[0][i] = 0.0;
//				result[1][i] = result_dct[0][i]; //���ԍ����i�[
//			}
//			else {
//				result[0][i] = 1.0;   // �i���̂Ƃ��낷�ׂĂ������ɂȂ�j
//				result[1][i] = result_ica[0][i];
//			}
//		}
//
//
//		////////////////�o��///////////////////
//		//��ʂP�i���ԍ��j
//		//fprintf(fp, "\nUSE Q [%d]\n", Q);
//		//fprintf(fp, "\n<Num>\n");
//		//fprintf(fp2, "\nUSE Q [%d]\n", Q);
//		//fprintf(fp2, "\n<Num>\n");
//
//		//for (i = 0; i < 1024; i++) {
//		//	//fprintf(fp, "%1d,", (int)result[0][i]);
//		//	fprintf(fp, "%lf,", result_dct[0][i] - result_ica[0][i]);
//		//	if (i % 32 == 31) {
//		//		fprintf(fp, "\n");
//		//	}
//		//}
//		//fprintf(fp, "\n\n");
//
//		//��ʂP���iMSE�l�j
//		//fprintf(fp, "\n<MSE Val>\n");
//		//for (i = 0; i < 1024; i++) {
//		//	fprintf(fp, "%5d, ", (int)sort_d[0][i].val);
//		//	if (i % 32 == 31)
//		//		fprintf(fp, "\n");
//		//}
//
//		//////////////////�o�͏I��///////////////////////
//	} // dct�̍ŏ��ɖ߂�
//
//	fclose(fp);
//	fclose(fp2);
//	fclose(fp3);
//
//	for (i = 0; i < 64; i++) {
//		free(sort_d[i]);
//	}
//	free(sort_d);
//	printf("finish");
//
//}


//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "ica.h"
//
//int main()
//{
//	FILE* fp, * fp2, * fp3; //�t�@�C���|�C���^�̒�`
//
//	//�錾
//	static unsigned char origin[256][256] = { 0 };	//���摜�i256*256�̂ݑΉ��j
//	//static  double ori_temp2[64][1024] = { 0 };
//	static int i, j, n, m, k, l, mk, ml, Q, QQ, b, a, c, ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai2[64][1024], temp_sai3[256][256];
//	static double sum, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[64][1024], mse_ica[64][1024], mse_ica2[64][1024], result_dct[2][1024], result_ica[2][1024], result[2][1024];
//	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 };
//	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dcoe_temp[64][1024] = { 0 };
//	static unsigned char dammy[256][256] = { 0 };
//	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
//	static unsigned char  ica_sai[256][256] = { 0 };
//	static struct pca pcaStr = { 0 };
//	static char image_name[20] = { 0 };	//�摜�t�@�C����(�g���q�܂܂�)
//	char output[20];
//	struct tt** sort_d, temp;
//
//	sum = 0;
//
//	//�ǂݍ��ރt�@�C����
//	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'c', 'a', 'm', 'e', 'r', 'a', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'a', 'x', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'e', 'n', 'n', 'a', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 's', 'a', 'i', 'l', 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'w', 'o', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'r', 'i', 'd', 'g', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'u', 'i', 'l', 'd', 'i', 'n', 'g', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'g', 'i', 'r', 'l', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'i', 'g', 'h', 't', 'h', 'o', 'u', 's', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 't', 'e', 'x', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'e', 'a', 'r', 't', 'h', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'm', 'a', 'n', 'd', 'r', 'i', 'l', 'l', '.', 'b', 'm', 'p' };
//
//
//	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
//		return -1;
//
//	/* �ꎟ���z�񂩂�񎟌��z��ɕϊ� */
//	for (i = 0; i < 256; i++)
//		for (j = 0; j < 256; j++)
//			origin[i][j] = ori_temp[i * 256 + j];
//
//
//	//�\�[�g�p
//	sort_d = (struct tt**)malloc(sizeof(struct tt*) * 64);
//	for (i = 0; i < 64; i++) {
//		sort_d[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
//	}
//
//	//�o�̓t�@�C���@�錾
//	if ((fp = fopen("test_dct.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp2 = fopen("test_ica.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp3 = fopen("result_ica.txt", "a")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//
//	/////////////////�錾���� �I��///////////////////////
//
//
//	// ///////////////////////// ica //////////////////////////////////
//	// ICA���E�W��
//	// origin = ��f�l(256*256),  y = ica�W��(�u���b�N��64��1024�u���b�N��),  w = ica���(64�̌v�Z�@�̒��ɂ��ꂼ��64�̌v�Z�@������)
//	// ICA��"origin"�����邱�Ƃ�"y"(�v�Z��̒l)��"w"(�v�Z�̎d��)�̌��ʂ��o�͂����
//	// ���͌v�Z���@�B�W���� 8*8�̉�f�u���b�N���\������̂� 64�̊�ꂪ���ꂼ�� �ǂꂭ�炢�g���Ă���̂��i�܂܂�Ă��邩�j�̒l�B
//	// �u���b�N�Ƃ� 256*256��f�̂����c8��8�̃u���b�N�B��摜�ɂ�(256/8) 32*32 = 1024�u���b�N
//	pcaStr = new_pca(origin);
//	ICA(origin, pcaStr, y, w, avg, 100, 0.002);
//
//
//
//	// �v�Z�p�ɃR�s�[
//	for (i = 0; i < 1024; i++)
//		for (j = 0; j < 64; j++)
//			ny[j][i] = y[j][i]; // ny -> yy(ica�W���R�s�[)
//
//	for (i = 0; i < 64; i++)
//		for (j = 0; j < 64; j++)
//			nw[j][i] = w[j][i]; // nw-> w(ica���R�s�[)
//
//	// ���ʂ̍č\�������iICA�ŕ�������"y"��"w"����"origin"���č\���j
//	//�i�֌W�Ȃ��j
//	seki5(nw, ny, x); // x -> nw * ny
//	xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//	// 64�̊��̂���1�����g�p���邽�߂̏���
//	// ���͂�����Ȃ�����A�g�p����W����64����1�ɐ�������B
//	// �u���b�N���Ŏg�p�������1�~64���邩��64�p�^�[���~�S1024�u���b�N
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			b = i;
//
//			// �Y���W���ȊO0
//			// i�Ԗڂ̌W���i���j�̂ݎg�p�B����ȊO�̌W����0�B
//			for (a = 0; a < 64; a++) {
//				if (b == a)
//					ny[a][j] = y[a][j];
//				else
//					ny[a][j] = 0;
//			}
//
//			// �������i�K���s���j
//			for (a = 0; a < 64; a++)
//				xx[a] = 0.0;
//
//			// 1�u���b�N�ŏ������s���Ă��邽�߁A���̃u���b�N�ԍ��i���j�̌W����
//			// ���ׂĂ̊���p���邱�Ƃ� ���Ƃ̃u���b�N���č\�����鏈��
//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> �č\���χ@
//			avg_inter_Block(block_ica, avg, j); // ica_sai -> �č\���χA
//
//
//			// �u���b�N���Ƃ�MSE
//			// MSE�́i���̒l - �č\���̒l�j^2�����邱�Ƃ�
//			// �č\�������l�����̒l�Ƃǂꂭ�炢����Ă���̂������邽�߂̎w�W
//			sum = 0.0;
//			sum2 = 0.0;
//			mk = j % 32;
//			ml = j / 32;
//
//			// 64��2��̕��ς��炻�̃u���b�N�����ς��Ăǂꂭ�炢 ����Ă���̂�������
//			// �i���Ȃ݂ɁA1�u���b�N�ɂ�64�p�^�[�����S1024�u���b�N�j
//			for (a = 0; a < 8; a++) {
//				for (b = 0; b < 8; b++) {
//					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
//				}
//			}
//			mse_ica[i][j] = sum / 64;//����
//		}
//		// ���s�m�F�p
//		printf("%d\n", j);
//	}
//
//
//	// ///////////// ica MSE �\�[�g/////////////////////////
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = mse_ica[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(mse_ica[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			min = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (mse_ica[j][n] < min) {
//					min = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//	////////////////////////////�\�[�g�I��////////////////////////////////
//
//	//�\�[�g���ʂ��i�[
//	// �e�u���b�N64�p�^�[���̒��ň�Ԍ덷�̂Ȃ�����
//	for (i = 0; i < 1024; i++) {
//		result_ica[0][i] = sort_d[0][i].num;  // ���ԍ�
//		result_ica[1][i] = sort_d[0][i].val;    // MSE�l
//	}
//	// ica�S����DCT�S�����r��
//	for (i = 0; i < 1024; i++) {
//		for (a = 0; a < 64; a++) {
//			//if (result_ica[0][i] == a)
//			ny[a][i] = y[a][i];
//			//else
//				//ny[a][i] = 0;
//		}
//	}
//
//	//seki5(nw, ny, x); // x -> nw * ny
//	//xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	//avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = ica_sai[a][b];
//
//	sprintf(output, "DCT/ICA_sai.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//
//	for (k = 0; k < 32; k++) {
//		for (l = 0; l < 32; l++) {
//			sum = 0.0;
//			for (i = 0; i < 8; i++) {
//				for (j = 0; j < 8; j++) {
//					sum += pow(origin[i + (8 * k)][j + (8 * l)] - ica_sai[i + (8 * k)][j + (8 * l)], 2);
//				}
//			}
//			best_ica[k * 32 + l] = sum / 64;//����
//		}
//	}
//
//	//����m�F
//	printf("<ica fin>\n\n");
//	/////////////////////////////////ica �I��/////////////////////////////////////////
//
//
//	// //////////////////////////// dct ////////////////////////////////////////
//	// ICA �Ƒ�̓����BDCT�̊��͔ėp�I�����猈�܂��Ă���B�W���݂̂𓮂����΂���
//
//	//����m�F
//	printf("<dct start>\n");
//
//	// 10�i�K�i�������邩��10�i�K�����
//	//for (Q = 100; Q > 0; Q -= 10) {
//	//	printf("now Q is %d\n", Q);
//	Q = 100;
//
//	// dct����
//	dct(origin, dcoe, 8); // �W�����o��
//	quantization(dcoe, Q); // �W���̕i����10�i�K�ŗ��Ƃ������i�ʎq���j���Ƃ��Η��Ƃ��قǃf�[�^�͌y���Ȃ邪�A�i����������
//	idct(dcoe, dcoe2, 8); // ���ʂ̍č\��
//
//
//
//	// 256*256->64*1024�i�W���̏��Ԃ�ύX)
//	//  256*256����0�Ԗڂ̃u���b�N�̒l�� [0~7]*[0~7]�Ɋi�[����Ă���i1�Ԗڂ���[0~7]*[8~15]) ���A
//	//  64*1024�ɂ����[0~64]*[0]�ƌv�Z���₷���E�킩��₷���Ȃ�
//
//	// �W���������i�K���s���j
//	for (a = 0; a < 256; a++) {
//		for (c = 0; c < 256; c++) {
//			coe_temp[c][a] = dcoe[c][a];
//		}
//	}
//
//	// �Y���W���ȊO0
//	for (b = 0; b < 256; b++) {
//		for (a = 0; a < 256; a++) {
//			if (b % 8 != 0 || a % 8 != 0)
//				coe_temp[a][b] = 0.0;
//		}
//	}
//
//	idct(coe_temp, dcoe3, 8); // ���ʂ̍č\��
//
//   // �u���b�N���Ƃ�MSE
//	sum = 0.0;
//	sum2 = 0.0;
//	mk = j % 32;
//	ml = j / 32;
//	/////////////////////////dct �I��/////////////////////////////
//
//	//if (Q == 100) {
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = dcoe2[a][b];
//
//	sprintf(output, "DCT/DCT_sai3.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
////}
//
//	for (k = 0; k < 32; k++) {
//		for (l = 0; l < 32; l++) {
//			sum = 0.0;
//			for (i = 0; i < 8; i++) {
//				for (j = 0; j < 8; j++) {
//					sum += pow(origin[i + (8 * k)][j + (8 * l)] - dcoe2[i + (8 * k)][j + (8 * l)], 2);
//				}
//			}
//			best_dct[k * 32 + l] = sum / 64;//����
//		}
//	}
//
//	for (i = 0; i < 1024; i++)
//		count[i] = 0;
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = dcoe2[a][b];
//
//	// mse���ʂ̔�r
//	for (i = 0; i < 1024; i++) {
//		if (best_dct[i] < best_ica[i]) { // dct�̂ق������������
//			result[0][i] = 0.0;
//			result[1][i] = best_dct[i]; //���ԍ����i�[
//			count[(int)result_dct[0][i]]++;
//			k = i % 32;
//			l = i / 32;
//			for (b = 0; b < 8; b++) {
//				for (a = 0; a < 8; a++) {
//					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
//				}
//			}
//
//		}
//		else {
//			result[0][i] = 1.0;
//			result[1][i] = best_ica[i];
//			count[(int)result_ica[0][i]]++;
//		}
//	}
//	sum = 0;
//	sum2 = 0;
//
//
//
//	sprintf(output, "DCT/DI_best.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//	for (i = 0; i < 1024; i++) {
//		//if (best_dct[i] - best_ica[i] < 1) {
//		//	fprintf(fp, "%d, ", 0);
//		//	sum++;
//		//}
//		//else if (best_dct[i] - best_ica[i] < 5) {
//		//	count[i] = i;
//		//	fprintf(fp, "%d, ", 5);
//		//	sum2++;
//		//}
//		//else
//		fprintf(fp, "%d, ", (int)result[0][i]);
//		if (i % 32 == 31) {
//			fprintf(fp, "\n");
//		}
//	}
//	//fprintf(fp, "%d : %d",(int)sum,(int)sum2);
//	fprintf(fp, "\n\n");
//
//	//if (Q == 10) {
//	for (i = 0; i < 1024; i++) {
//		fprintf(fp2, "%2d, ", (int)result_ica[0][i]);
//		if (i % 32 == 31) {
//			fprintf(fp2, "\n");
//		}
//		//fprintf(fp2, "%4.2lf : %4.2lf  -->  %4.2lf", best_dct[i], best_ica[i], best_dct[i] - best_ica[i]);
//		//fprintf(fp2, "\n");
//	}
//	//}
//
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			ny[i][j] = y[i][j];
//		}
//	}
//
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = ny[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(ny[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			max = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (sort_d[j][n].abs > max) {
//					max = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//
//	for (i = 0; i < 1024; i++)
//		count[i] = 0;
//
//
//	for (i = 0; i < 1024; i++) {
//		for (j = 0; j < 64; j++) {
//			if (result_ica[0][i] == sort_d[j][i].num) {
//				fprintf(fp3, "%2d, ", j);
//				count[j]++;
//			}
//		}
//		if (i % 32 == 31) {
//			fprintf(fp3, "\n");
//		}
//	}
//
//	fprintf(fp3, "\n");
//	for (j = 0; j < 64; j++)
//		fprintf(fp3, " [%4d] (%3d) \n", j, count[j]);
//
//
//	////////////////�o��///////////////////
//
//	//////////////////�o�͏I��///////////////////////
////} // dct�̍ŏ��ɖ߂�
//
//	fclose(fp);
//	fclose(fp2);
//	fclose(fp3);
//
//	for (i = 0; i < 64; i++) {
//		free(sort_d[i]);
//	}
//
//	free(sort_d);
//	printf("finish");
//
//}

// 7/16 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <direct.h>
//#include <Windows.h>
//#include <string.h>
//#include "ica.h"
//
//int main()
//{
//	FILE* fp, * fp2, * fp3, * fp4, * fp5; //�t�@�C���|�C���^�̒�`
//
//	//�錾
//	static unsigned char origin[256][256] = { 0 };	//���摜�i256*256�̂ݑΉ��j
//	//static  double ori_temp2[64][1024] = { 0 };
//	static int i, j, n, m, k, l, mk, ml, Q, QQ, b, a, c, seg[64 * 64], ori_temp[256 * 256], count[1024], count2[1024], count3[64], temp_sai[256 * 256], temp_sai11[256 * 256], temp_sai22[256 * 256], temp_sai2[64][1024], temp_sai3[256][256], temp_sai4[64 * 64], ica[64], temp_temp[64];
//	static double sum, best_ica[1024], best_dct[1024], sum2, min, max, mse_dct[64][1024], mse_dct2[1024], mse_ica[64][1024], cost_ica[1024], cost_dct[1024], mse_ica2[1024], result_dct[2][1024], result_ica[2][1024], result[2][1024], lambda = 1024.0;
//	static double coe[256][256] = { 0 }, dct_coe[64][1024] = { 0 }, coe_temp[256][256] = { 0 }, dcoe[256][256] = { 0 };
//	static double avg[1024], y[64][1024], w[64][64], ny[64][1024], nw[64][64], x[64][1024], xx[64], dcoe_temp2[64][1024], dct_cost[64][1024], mse_cost[64][1024], ica_bent[1024], dct_bent[1024], ica_ent[64][1024], dct_ent[64][1024], dcoe_temp[64][1024] = { 0 };
//	static unsigned char dammy[256][256] = { 0 };
//	static unsigned char block_dct[64], dcoe3[256][256] = { 0 }, dcoe2[256][256] = { 0 }, block_ica[64];
//	static unsigned char  ica_sai[256][256] = { 0 };
//	static struct pca pcaStr = { 0 };
//	static char image_name[20] = { 0 };	//�摜�t�@�C����(�g���q�܂܂�)
//	char g[40];
//	char output[30], in[50], out[50];
//	struct tt** sort_d, temp;
//
//	sum = 0;
//
//	//�t�H���_�쐬�@(�Ȃ���΍쐬)
//	//_mkdir("ORIGIN"); //���摜�̃u���b�N�摜���g�債�ďo��
//	//sprintf(g, "rmdir /s /q ICA");
//	//system(g);
//	//_mkdir("ICA"); //ICA�������ꂼ��g�債�ďo��
//	//_mkdir("ICA/COEFFICIENT"); //ICA�������ꂼ��g�債�ďo��
//	//for (j = 0; j < 1024; j++) {
//	//	sprintf(g, "mkdir ICA\\COEFFICIENT\\coe[%d]", j);
//	//	system(g);
//	//}
//	//_mkdir("DCT"); //��������ICA��DCT�̍č\���摜���o��
//	//_mkdir("MSE"); //MSE��r��A��ꂲ�Ƃ̗̈�ŕ��������摜���o��
//	sprintf(g, "rmdir /s /q test");
//	system(g);
//	_mkdir("test"); //MSE��r��A��ꂲ�Ƃ̗̈�ŕ��������摜���o��
//	//_mkdir("DEFAULT"); //MSE��r��A��ꂲ�Ƃ̗̈�ŕ��������摜���o��
//	for (j = 0; j < 64; j++) {
//		sprintf(g, "mkdir test\\ICA[%d]", j);
//		system(g);
//	}
//	//sprintf(g, "rmdir /s /q coe");
//	//system(g);
//	_mkdir("coe"); //���摜�̃u���b�N�摜���g�債�ďo��
//	for (j = 0; j < 20; j++) {
//		sprintf(g, "mkdir coe\\coefficient[%d]", j);
//		system(g);
//	}
//
//
//
//	//�ǂݍ��ރt�@�C����
//	static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'c', 'a', 'm', 'e', 'r', 'a', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'a', 'x', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'e', 'n', 'n', 'a', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 's', 'a', 'i', 'l', 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'w', 'o', 'm', 'a', 'n', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'r', 'i', 'd', 'g', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'o', 'a', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'b', 'u', 'i', 'l', 'd', 'i', 'n', 'g', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'g', 'i', 'r', 'l', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'l', 'i', 'g', 'h', 't', 'h', 'o', 'u', 's', 'e', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 't', 'e', 'x', 't', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'e', 'a', 'r', 't', 'h', '.', 'b', 'm', 'p' };
//	//static char filename[20] = { 'm', 'a', 'n', 'd', 'r', 'i', 'l', 'l', '.', 'b', 'm', 'p' };
//
//
//	if (img_read_gray(ori_temp, filename, image_name, 256, 256) != 0)
//		return -1;
//
//	for (i = 0; i < 20 - 3; i++) {
//		for (j = 0; j < 4; j++) {
//			if (filename[i + j] == "." && filename[i + j] == "b" && filename[i + j] == "m" && filename[i + j] == "p") {
//				filename[i + j] = "";
//			}
//		}
//	}
//
//
//	/* �ꎟ���z�񂩂�񎟌��z��ɕϊ� */
//	for (i = 0; i < 256; i++)
//		for (j = 0; j < 256; j++)
//			origin[i][j] = ori_temp[i * 256 + j];
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = origin[a][b];
//
//	sprintf(output, "DEFAULT/GEN.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//	//�\�[�g�p
//	sort_d = (struct tt**)malloc(sizeof(struct tt*) * 64);
//	for (i = 0; i < 64; i++) {
//		sort_d[i] = (struct tt*)malloc(sizeof(struct tt) * 1024);
//	}
//
//	//�o�̓t�@�C���@�錾
//	if ((fp = fopen("test_dct.txt", "w")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp2 = fopen("test_ica.txt", "w")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp3 = fopen("result_ical.csv", "w")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp4 = fopen("ICA.csv", "w")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	if ((fp5 = fopen("ica.txt", "w")) == NULL) {
//		fprintf(stderr, "Can not open file\n");
//	}
//
//	/////////////////�錾���� �I��///////////////////////
//
//
//	for (i = 0; i < 32; i++) {
//		for (j = 0; j < 32; j++) {
//			for (b = 0; b < 8; b++)
//				for (a = 0; a < 8; a++)
//					for (n = 0; n < 8; n++)
//						for (m = 0; m < 8; m++)
//							seg[64 * 8 * b + 8 * a + n * 64 + m] = ori_temp[256 * 8 * i + 8 * j + a + 256 * b];
//			sprintf(output, "ORIGIN/ORIGIN[%d].bmp", i * 32 + j);
//			img_write_gray(seg, output, 64, 64); // output�ɏo�͉摜�������o��
//		}
//	}
//
//
//
//	// ///////////////////////// ica //////////////////////////////////
//	// ICA���E�W��
//	// origin = ��f�l(256*256),  y = ica�W��(�u���b�N��64��1024�u���b�N��),  w = ica���(64�̌v�Z�@�̒��ɂ��ꂼ��64�̌v�Z�@������)
//	// ICA��"origin"�����邱�Ƃ�"y"(�v�Z��̒l)��"w"(�v�Z�̎d��)�̌��ʂ��o�͂����
//	// ���͌v�Z���@�B�W���� 8*8�̉�f�u���b�N���\������̂� 64�̊�ꂪ���ꂼ�� �ǂꂭ�炢�g���Ă���̂��i�܂܂�Ă��邩�j�̒l�B
//	// �u���b�N�Ƃ� 256*256��f�̂����c8��8�̃u���b�N�B��摜�ɂ�(256/8) 32*32 = 1024�u���b�N
//	pcaStr = new_pca(origin);
//	ICA(origin, pcaStr, y, w, avg, 100, 0.002);
//
//
//
//	// �v�Z�p�ɃR�s�[
//	for (i = 0; i < 1024; i++)
//		for (j = 0; j < 64; j++)
//			ny[j][i] = y[j][i]; // ny -> yy(ica�W���R�s�[)
//
//	for (i = 0; i < 64; i++)
//		for (j = 0; j < 64; j++)
//			nw[j][i] = w[j][i]; // nw-> w(ica���R�s�[)
//
//
//
//	// ���ʂ̍č\�������iICA�ŕ�������"y"��"w"����"origin"���č\���j
//	//�i�֌W�Ȃ��j
//	seki5(nw, ny, x); // x -> nw * ny
//	xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = ica_sai[a][b];
//
//	sprintf(output, "DEFAULT/ICA.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//	b_entropy_ica(ny, ica_bent);
//	b_mse(origin, ica_sai, mse_ica2);
//	make_cost(ica_bent, mse_ica2, cost_ica);
//
//
//	// 64�̊��̂���1�����g�p���邽�߂̏���
//	// ���͂�����Ȃ�����A�g�p����W����64����1�ɐ�������B
//	// �u���b�N���Ŏg�p�������1�~64���邩��64�p�^�[���~�S1024�u���b�N
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			b = i;
//
//			// �Y���W���ȊO0
//			// i�Ԗڂ̌W���i���j�̂ݎg�p�B����ȊO�̌W����0�B
//			for (a = 0; a < 64; a++) {
//				if (b == a)
//					ny[a][j] = y[a][j];
//				else
//					ny[a][j] = 0;
//			}
//
//			// �������i�K���s���j
//			for (a = 0; a < 64; a++)
//				xx[a] = 0.0;
//
//			// 1�u���b�N�ŏ������s���Ă��邽�߁A���̃u���b�N�ԍ��i���j�̌W����
//			// ���ׂĂ̊���p���邱�Ƃ� ���Ƃ̃u���b�N���č\�����鏈��
//			seki5_Block(nw, ny, xx, j); // xx64 -> nw * ny
//			xtogen_Block(xx, block_ica, avg, j); // ica_sai -> �č\���χ@
//			avg_inter_Block(block_ica, avg, j); // ica_sai -> �č\���χA
//
//
//			// �u���b�N���Ƃ�MSE
//			// MSE�́i���̒l - �č\���̒l�j^2�����邱�Ƃ�
//			// �č\�������l�����̒l�Ƃǂꂭ�炢����Ă���̂������邽�߂̎w�W
//			sum = 0.0;
//			sum2 = 0.0;
//			mk = j % 32;
//			ml = j / 32;
//
//			// 64��2��̕��ς��炻�̃u���b�N�����ς��Ăǂꂭ�炢 ����Ă���̂�������
//			// �i���Ȃ݂ɁA1�u���b�N�ɂ�64�p�^�[�����S1024�u���b�N�j
//			for (a = 0; a < 8; a++) {
//				for (b = 0; b < 8; b++) {
//					sum += pow(origin[ml * 8 + b][mk * 8 + a] - block_ica[b * 8 + a], 2);
//				}
//			}
//			mse_ica[i][j] = sum / 64;//����
//		}
//		// ���s�m�F�p
//		printf("%d\n", j);
//	}
//
//
//	// ///////////// ica MSE �\�[�g/////////////////////////
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = mse_ica[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(mse_ica[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			min = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (mse_ica[j][n] < min) {
//					min = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//	////////////////////////////�\�[�g�I��////////////////////////////////
//
//	//�\�[�g���ʂ��i�[
//	// �e�u���b�N64�p�^�[���̒��ň�Ԍ덷�̂Ȃ�����
//	for (i = 0; i < 1024; i++) {
//		result_ica[0][i] = sort_d[0][i].num;  // ���ԍ�
//		result_ica[1][i] = sort_d[0][i].val;    // MSE�l
//	}
//	// ica�S����DCT�S�����r��
//	for (i = 0; i < 1024; i++) {
//		for (a = 0; a < 64; a++) {
//			if (result_ica[0][i] == a)
//				ny[a][i] = y[a][i];
//			else
//				ny[a][i] = 0;
//		}
//	}
//
//	seki5(nw, ny, x); // x -> nw * ny
//	xtogen(x, ica_sai, avg); // ica_sai -> �č\���χ@
//	avg_inter(ica_sai, avg); // ica_sai -> �č\���χA
//
//
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = ica_sai[a][b];
//
//	sprintf(output, "DCT/ICA_sai.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//
//
//	for (k = 0; k < 32; k++) {
//		for (l = 0; l < 32; l++) {
//			sum = 0.0;
//			for (i = 0; i < 8; i++) {
//				for (j = 0; j < 8; j++) {
//					sum += pow(origin[i + (8 * k)][j + (8 * l)] - ica_sai[i + (8 * k)][j + (8 * l)], 2);
//				}
//			}
//			best_ica[k * 32 + l] = sum / 64;//����
//		}
//	}
//
//	// �W���m�F
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = y[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(y[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			max = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (y[j][n] > max) {
//					max = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			fprintf(fp4, "%lf, ", y[i][j]);
//		}
//		fprintf(fp4, "\n");
//	}
//
//
//	//�W��10�ȏ�g�p�̂�
//
//
//	//����m�F
//	printf("<ica fin>\n\n");
//	/////////////////////////////////ica �I��/////////////////////////////////////////
//
//
//	// //////////////////////////// dct ////////////////////////////////////////
//	// ICA �Ƒ�̓����BDCT�̊��͔ėp�I�����猈�܂��Ă���B�W���݂̂𓮂����΂���
//
//	//����m�F
//	printf("<dct start>\n");
//
//	// 10�i�K�i�������邩��10�i�K�����
//	//for (Q = 100; Q > 0; Q -= 10) {
//	//	printf("now Q is %d\n", Q);
//	Q = 100;
//
//	// dct����
//	dct(origin, dcoe, 8); // �W�����o��
//	quantization(dcoe, Q); // �W���̕i����10�i�K�ŗ��Ƃ������i�ʎq���j���Ƃ��Η��Ƃ��قǃf�[�^�͌y���Ȃ邪�A�i����������
//	idct(dcoe, dcoe2, 8); // ���ʂ̍č\��
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = dcoe2[a][b];
//
//	sprintf(output, "DEFAULT/DCT[%d].bmp", Q);
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//	b_entropy_dct(dcoe, dct_bent);
//	b_mse(origin, dcoe2, mse_dct2);
//	make_cost(dct_bent, mse_dct2, cost_dct);
//
//	// 256*256->64*1024�i�W���̏��Ԃ�ύX)
//	//  256*256����0�Ԗڂ̃u���b�N�̒l�� [0~7]*[0~7]�Ɋi�[����Ă���i1�Ԗڂ���[0~7]*[8~15]) ���A
//	//  64*1024�ɂ����[0~64]*[0]�ƌv�Z���₷���E�킩��₷���Ȃ�
//
//	// �W���������i�K���s���j
//	for (a = 0; a < 256; a++) {
//		for (c = 0; c < 256; c++) {
//			coe_temp[c][a] = dcoe[c][a];
//		}
//	}
//
//	// �Y���W���ȊO0
//	for (b = 0; b < 256; b++) {
//		for (a = 0; a < 256; a++) {
//			if (b % 8 != 0 || a % 8 != 0)
//				coe_temp[a][b] = 0.0;
//		}
//	}
//
//	idct(coe_temp, dcoe3, 8); // ���ʂ̍č\��
//
//   // �u���b�N���Ƃ�MSE
//	sum = 0.0;
//	sum2 = 0.0;
//	mk = j % 32;
//	ml = j / 32;
//	/////////////////////////dct �I��/////////////////////////////
//
//	//if (Q == 100) {
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = dcoe3[a][b];
//
//	sprintf(output, "DCT/DCT_sai3.bmp");
//	img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
////}
//
//	for (k = 0; k < 32; k++) {
//		for (l = 0; l < 32; l++) {
//			sum = 0.0;
//			for (i = 0; i < 8; i++) {
//				for (j = 0; j < 8; j++) {
//					sum += pow(origin[i + (8 * k)][j + (8 * l)] - dcoe3[i + (8 * k)][j + (8 * l)], 2);
//				}
//			}
//			best_dct[k * 32 + l] = sum / 64;//����
//		}
//	}
//
//	for (i = 0; i < 1024; i++)
//		count[i] = 0;
//
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = y[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(y[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			max = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (sort_d[j][n].abs > max) {
//					max = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//	for (j = 0; j < 64; j++) {
//
//		for (a = 0; a < 256; a++)
//			for (b = 0; b < 256; b++)
//				temp_sai[a * 256 + b] = origin[a][b];
//
//		// mse���ʂ̔�r
//		for (i = 0; i < 1024; i++) {
//			if (best_dct[i] <= best_ica[i]) { // dct�̂ق������������
//				result[0][i] = 0.0;
//				result[1][i] = best_dct[i]; //���ԍ����i�[
//				count[(int)result_dct[0][i]]++;
//			}
//			else {
//				result[0][i] = 1.0;
//				result[1][i] = best_ica[i];
//				count[(int)result_ica[0][i]]++;
//				k = i % 32;
//				l = i / 32;
//
//				if (j != result_ica[0][i]) {
//					for (b = 0; b < 8; b++) {
//						for (a = 0; a < 8; a++) {
//							temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
//						}
//					}
//				}
//				else {
//					for (b = 0; b < 8; b++)
//						for (a = 0; a < 8; a++)
//							for (n = 0; n < 8; n++)
//								for (m = 0; m < 8; m++)
//									seg[64 * 8 * b + 8 * a + n * 64 + m] = ori_temp[256 * 8 * l + 8 * k + a + 256 * b];
//
//					QQ = 0;
//
//					for (b = 0; b < 64; b++)
//						if (result_ica[0][i] == sort_d[b][i].num)
//							QQ = b;
//
//					sprintf(output, "test/ICA[%d]/ORIGIN[%d][%d].bmp", j, l * 32 + k, QQ);
//					img_write_gray(seg, output, 64, 64); // output�ɏo�͉摜�������o��
//				}
//			}
//		}
//
//		sprintf(output, "MSE/MSE[%d].bmp", j);
//		img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//	}
//
//	sum = 0;
//	sum2 = 0;
//
//	for (a = 0; a < 256; a++)
//		for (b = 0; b < 256; b++)
//			temp_sai[a * 256 + b] = origin[a][b];
//
//	// bent���ʂ̔�r
//	for (i = 0; i < 1024; i++) {
//		if (cost_dct[i] >= cost_ica[i]) { // dct�̂ق������������
//			result[0][i] = 0.0;
//			result[1][i] = best_dct[i]; //���ԍ����i�[
//			k = i % 32;
//			l = i / 32;
//			for (b = 0; b < 8; b++) {
//				for (a = 0; a < 8; a++) {
//					temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
//				}
//			}
//		}
//		else {
//			result[0][i] = 1.0;
//			result[1][i] = best_ica[i];
//			count[(int)result_ica[0][i]]++;
//		}
//		sprintf(output, "RD.bmp", j);
//		img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//	}
//
//
//
//
//
//
//	for (i = 0; i < 1024; i++) {
//		//if (best_dct[i] - best_ica[i] < 1) {
//		//	fprintf(fp, "%d, ", 0);
//		//	sum++;
//		//}
//		//else if (best_dct[i] - best_ica[i] < 5) {
//		//	count[i] = i;
//		//	fprintf(fp, "%d, ", 5);
//		//	sum2++;
//		//}
//		//else
//		fprintf(fp, "%d, ", (int)result[0][i]);
//		if (i % 32 == 31) {
//			fprintf(fp, "\n");
//		}
//	}
//	//fprintf(fp, "%d : %d",(int)sum,(int)sum2);
//	fprintf(fp, "\n\n");
//
//	//if (Q == 10) {
//	for (i = 0; i < 1024; i++) {
//		fprintf(fp2, "%2d, ", (int)result_ica[0][i]);
//		if (i % 32 == 31) {
//			fprintf(fp2, "\n");
//		}
//		//fprintf(fp2, "%4.2lf : %4.2lf  -->  %4.2lf", best_dct[i], best_ica[i], best_dct[i] - best_ica[i]);
//		//fprintf(fp2, "\n");
//	}
//	//}
//
//	for (j = 0; j < 1024; j++) {
//		for (i = 0; i < 64; i++) {
//			ny[i][j] = y[i][j];
//		}
//	}
//
//	for (i = 0; i < 64; i++) {
//		for (j = 0; j < 1024; j++) {
//			// .val -> �l���擾�E������ύX���L��
//			// .abs -> ��Βl���L��
//			// .num -> ���X�̌W���ɑΉ�����u���b�N���ԍ����L��
//			sort_d[i][j].val = ny[i][j];		/* ���X�̌W���l */
//			sort_d[i][j].abs = fabs(ny[i][j]);	/* �\�[�g�͌W���̐�Βl�ōs��*/
//			sort_d[i][j].num = i;					/* num�Ɍ��X�̌W���ɑΉ�����ԍ����L�� */
//		}
//	}
//
//	for (n = 0; n < 1024; n++) {
//		for (i = 0; i < 64 - 1; i++) {
//			max = sort_d[i][n].abs;
//			k = i;
//			for (j = i + 1; j < 64; j++) {
//				if (sort_d[j][n].abs > max) {
//					max = sort_d[j][n].abs;
//					k = j;
//				}
//			}
//			temp = sort_d[i][n];
//			sort_d[i][n] = sort_d[k][n];
//			sort_d[k][n] = temp;
//		}
//	}
//
//
//	for (i = 0; i < 1024; i++)
//		count[i] = 0;
//
//	for (m = 10; m < 11; m++) {
//		sprintf(g, "mkdir coe\\coefficient[%d]", m);
//		system(g);
//
//		for (a = 0; a < 256; a++)
//			for (b = 0; b < 256; b++) {
//				temp_sai[a * 256 + b] = origin[a][b];
//			}
//
//		for (i = 0; i < 64; i++)
//			temp_temp[i] = 99;
//
//		c = 0; ml = 0;
//
//		for (i = 0; i < 1024; i++) {
//			for (j = 0; j < 64; j++) {
//				if (result_ica[0][i] == sort_d[j][i].num) {
//					max = sort_d[j][i].num;
//					//fprintf(fp3, "%2d, ", j);
//					//count[j]++;
//					k = i % 32;
//					l = i / 32;
//					c = 0;
//					if (j <= m) {
//						for (b = 0; b < 8; b++)
//							for (a = 0; a < 8; a++)
//								temp_sai[256 * 8 * l + 8 * k + a + 256 * b] = 0;
//
//						/*for (mk = 0; mk < ml + 1; mk++) {
//							if (temp_temp[mk] == max)
//								c++;
//						}
//						if (0 == c) {
//							ICA_temp(origin, pcaStr, y, w, avg, 100, 0.002, m, (int)max);
//							temp_temp[ml] = (int)max;
//							ml++;
//						}*/
//
//					}
//				}
//			}
//			//if (i % 32 == 31) {
//			//	fprintf(fp3, "\n");
//			//}
//		}
//
//		for (i = 0; i < 64; i++)
//			count3[i] = 0;
//
//		for (i = 0; i < 1024; i++) {
//			for (j = 0; j < 64; j++) {
//				if (result_ica[0][i] == sort_d[j][i].num) {
//					max = sort_d[j][i].num;
//					fprintf(fp3, "%2d, ", j);
//					count3[j]++;
//				}
//			}
//			if (i % 32 == 31) {
//				fprintf(fp3, "\n");
//			}
//		}
//
//		gnuplot2(count3);
//
//		sprintf(output, "coe/coefficient[%d]/coefficient[%d].bmp", m, m);
//		img_write_gray(temp_sai, output, 256, 256); // output�ɏo�͉摜�������o��
//
//
//		/*sprintf(g, "cp ICA\\ICA[%d] coe\\coefficient[]", m);
//		system(g);*/
//
//
//	}
//
//	//fprintf(fp3, "\n");
//	//for (j = 0; j < 64; j++)
//	//	fprintf(fp3, " [%4d] (%3d) \n", j, count[j]);
//
//
//	////////////////�o��///////////////////
//
//	//////////////////�o�͏I��///////////////////////
////} // dct�̍ŏ��ɖ߂�
//
//	fclose(fp);
//	fclose(fp2);
//	fclose(fp3);
//	fclose(fp4);
//	fclose(fp5);
//	//gnuplot(y);
//	for (i = 0; i < 64; i++) {
//		free(sort_d[i]);
//	}
//
//	free(sort_d);
//	printf("finish");
//
//}
