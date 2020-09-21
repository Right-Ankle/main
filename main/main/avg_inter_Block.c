/* avg_inter_Block.c */

/********************************************/
/*再構成された画像に直流成分を補填します*/
/********************************************/


#include <stdio.h>
#include "ica.h"

// 256*256 return gen
void avg_inter_Block(unsigned char gen[64], double avg[1024], int num) {
	int i, j;
	double temp, sum = 0, x[64][1024] = { 0 };


	// sum -> ブロック内の平均
	for (i = 0; i < 64; i++)
		sum += (double)gen[i];

	sum /= 64.0;

	sum = avg[num] - sum;



	for (i = 0; i < 64; i++) {
		temp = (double)gen[i] + sum;
		if (temp > 255)
			temp = 255;
		if (temp < 0)
			temp = 0;
		gen[i] = (unsigned char)temp;
	}

	sum = 0;
}

