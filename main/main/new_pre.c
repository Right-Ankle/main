/* new_pre.c */

/****************************/
/*   中心化処理を行います。	*/
/****************************/

#include<stdio.h>
#include<math.h>
// xx -> (元の画素値 - 平均値), avg -> ブロックごとの平均値,
void new_pre(double **xx, double *avg, int data){
	double sum;
	int i, j;
    // ica.c -> date = 1024
	for(i = 0; i < data; i++){

		sum = 0;

		for(j = 0; j < 64; j++)
			sum += xx[j][i];

		avg[i] = sum / 64.0; //ブロックごとの平均値

		for(j = 0; j < 64; j++)
			xx[j][i] = (xx[j][i] - avg[i]); // xx <- (元の画素値 - 平均値)
	}
}

