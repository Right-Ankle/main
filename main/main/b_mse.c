/* b_mse.c */

/************************************/
/* MSEをブロックごとに計算します。	*/
/************************************/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>
// MSE = 平均二乗誤差  return -> mse1024(ブロックごとのMSE)
void b_mse(unsigned char origin[][256], unsigned char gen[][256], double mse[1024]){

	// MSEのように、モデルの性能がどれだけ悪いかを示す関数をコスト関数という。
	int i, j, k, l, m, n = 0;
	double sum = 0.0;
	static unsigned char ori_temp[64][1024] = {0}, gen_temp[64][1024] = {0};

    //原画像と比較画像を256*256 -> 64*1024
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
		//mse[i] = sum / 64;  //なぜ /64しない？
	}
}
