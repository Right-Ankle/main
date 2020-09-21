/* wtosai.c */

/****************************************/
/*   ICA基底を画像のデータに変換します。	*/
/****************************************/

#include<stdio.h>
// return sai -> ica基底を画像データに変換
void wtosai(double w[][64], unsigned char sai[][64]){

	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;//ループカウンタ
	double maxw, minw, scale;

	maxw = w[0][0]; // 最大
	minw = w[0][0]; // 最小

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			if(w[i][j] > maxw)
				maxw = w[i][j];
			if(w[i][j] < minw)
				minw = w[i][j];
		}
	}

	scale = 255 / (maxw - minw); // scale変換

    // scale変換
	for(i = 0; i < 64; i += 8){
		for(j = 0; j < 64; j += 8){
			m = 0;
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					sai[i + k][j + l] = (unsigned char)(w[m][n] * scale + scale * -minw);
					m++;
				}
			}
			n++;
		}
	}
}



