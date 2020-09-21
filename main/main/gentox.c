/* gentox.c */

/********************************************/
/*   256*256のデータを64*1024の形にします。	*/
/********************************************/

#include<stdio.h>
// 256*256 -> 64*1024 縦64(ブロック内要素)横1024（各ブロック要素）
// (double)genならgtogen2を使用, // (char)genならgtogenを使用
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
// (double)genならgtogen2を使用
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

