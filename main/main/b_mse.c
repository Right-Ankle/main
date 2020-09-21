/* b_mse.c */

/************************************/
/* MSE���u���b�N���ƂɌv�Z���܂��B	*/
/************************************/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>
// MSE = ���ϓ��덷  return -> mse1024(�u���b�N���Ƃ�MSE)
void b_mse(unsigned char origin[][256], unsigned char gen[][256], double mse[1024]){

	// MSE�̂悤�ɁA���f���̐��\���ǂꂾ���������������֐����R�X�g�֐��Ƃ����B
	int i, j, k, l, m, n = 0;
	double sum = 0.0;
	static unsigned char ori_temp[64][1024] = {0}, gen_temp[64][1024] = {0};

    //���摜�Ɣ�r�摜��256*256 -> 64*1024
	n = 0;
	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					ori_temp[m][n] = origin[i+k][j+l];
					m++;
				}
			}
			n++;
		}
	}

	n = 0;
	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					gen_temp[m][n] = gen[i+k][j+l];
					m++;
				}
			}
			n++;
		}
	}

	for(i = 0; i < 1024; i++){
		sum = 0.0;
		for(j = 0; j < 64; j++){
			sum += pow(ori_temp[j][i] - gen_temp[j][i], 2);
		}
		mse[i] = sum;
		//mse[i] = sum / 64;  //�Ȃ� /64���Ȃ��H
	}
}
