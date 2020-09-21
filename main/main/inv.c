/* inv.c */

/****************************************************************/
/*   w の逆行列を inv_w に格納します。（w の中身も書き換わります）	*/
/****************************************************************/

#include<stdio.h>
// w -> ?, inv_w -> wの逆行列
void inv(double w[][64], double inv_w[][64]){

	double temp;
	int i, j, k;
    // 初期化：対角線は1, それ以外は0
	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			if(i == j)
				inv_w[i][j] = 1.0;
			else
				inv_w[i][j] = 0.0;
		}
	}

	for(i = 0; i < 64; i++){
		// w -> 対角線^-1
		temp = 1.0 / w[i][i];
		for(j = 0; j < 64; j++){
			// w -> wの対角成分を1に
			w[i][j] *= temp;
			// inv_w -> 対角成分の保存
			inv_w[i][j] *= temp;
		}
		for(j = 0; j < 64; j++){
			if(i != j){
				temp = w[j][i];
				for(k = 0; k < 64; k++){
				    // w -> w - 対象とi列の積
					w[j][k] -= w[i][k] * temp;
					inv_w[j][k] -= inv_w[i][k] * temp;
				}
			}
		}
	}
}