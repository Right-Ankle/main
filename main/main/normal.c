/* normal.c */

/************************************/
/*    グラム・シュミットの直交化法 	*/
/************************************/

#include<stdio.h>
#include<math.h>

// return w -> 直交化済
void normal(double w[][64]){
	// ベクトル同士の内積が0,大きさが1
	// ①ベクトルを自身の大きさで割る -> 大きさを１にする
	// ②a2'*e1 = a2*e1 - (a2*e1)|e1|^2 -> ベクトルa2と正規直交ベクトルe1の内積を０にする
	// ③e2 = a2/|a2| -> 大きさを１にする
	int i, j, k;
	double f[64] = {0}, temp[64] = {0}, a[64][64] = {0}, sum = 0, sum2 = 0;


	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			a[i][j] = w[i][j]; // w -> a コピー

	for(i = 0; i < 64; i++)
		sum += pow(a[i][0], 2); // 1行目2乗して総和

	for(i = 0; i < 64; i++)
		w[i][0] = a[i][0] / sqrt(sum); // 1行目だけ割る


	for(i = 1; i < 64; i++){

		sum = 0;

		for(j = 0; j < 64; j++)
			f[j] = a[j][i]; // 縦2行目から縦にコピー (a2e1?)

		for(j = 0; j < i; j++){

			sum2 = 0;

			for(k = 0; k < 64; k++)
				sum2 += w[k][j] * a[k][i]; // wのj行目＊aのi行目の総和

			for(k = 0; k < 64; k++)
				temp[k] = w[k][j] * sum2; //

			for(k = 0; k < 64; k++)
				f[k] -= temp[k];
		}

		for(j = 0; j < 64; j++)
			sum += pow(f[j], 2);

		for(j = 0; j < 64; j++)
			w[j][i] = f[j] / sqrt(sum);
	}

}