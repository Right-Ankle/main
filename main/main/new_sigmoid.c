/* new_sigmoid.c */

/************************************/
/*   y にシグモイド関数を適用します。 */
/************************************/

#include<stdio.h>
#define _USE_MATH_DEFINES//M_PIを使うための定義
#include<math.h>
// sig -> yyを利用したシグモイド関数
void new_sigmoid(double **yy, double **sig, int data)
{
	// シグモイド関数 -> 単調増加, f(0,0.5)で点対称
    //                 lim->∞ = 1, lim->-∞ = 0
	//                 入力に対して0~1の範囲で出力
	int i, j;

	for(i = 0; i < 64; i++){
		for(j = 0; j < data; j++){
			sig[i][j] = 1.0 / (1.0 + exp(-1.0 * yy[i][j]));
		}
	}

}