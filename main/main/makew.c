/* makew.c */

/*********************************************/
/*  初期行列をランダム値（0～1）で生成します。	 */
/*********************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES//M_PIを使うための定義
#include<math.h>
#include<time.h>
#include"MT.h"//一様分布を発生させるメルセンヌツイスタを使うためのヘッダファイル

// w -> 初期行列をランダム値（0～1）で生成
void makew(double w[][64]){
	int i,j;

	init_genrand(82405);

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			w[i][j] = genrand_real3();
		}
	}
}