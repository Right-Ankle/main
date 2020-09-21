/* gentox.c */

/********************************************/
/*   256*256�̃f�[�^��64*1024�̌`�ɂ��܂��B	*/
/********************************************/

#include<stdio.h>
// 256*256 -> 64*1024 �c64(�u���b�N���v�f)��1024�i�e�u���b�N�v�f�j
// (double)gen�Ȃ�gtogen2���g�p, // (char)gen�Ȃ�gtogen���g�p
void gentox(unsigned char gen[][256], double x[][1024]){

	int i, j, k, l;
	int m = 0;
	int n = 0;

	for(i = 0; i < 256; i += 8){
		for(j = 0; j < 256; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					x[m][n] = (double)gen[i+k][j+l];
					m++;
				}
			}
			n++;
		}
	}
}
// (double)gen�Ȃ�gtogen2���g�p
void gentox2(double gen[][256], double x[][1024]){

	int i, j, k, l;
	int m = 0;
	int n = 0;

	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			m = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					x[m][n] = gen[i + k][j + l];
					m++;
				}
			}
			n++;
		}
	}
}

