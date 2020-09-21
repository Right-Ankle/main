/* dctcoe_conv.c */
#include<stdio.h>
// 256*256 -> 64*1024
void dctcoe_conv(double in[256][256], double out[64][1024]){
	int i, j, k, l, m, n;

	i = j = k = l = m = n = 0;

	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			m = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					out[m][n] = in[i + k][j + l];
					m++;
				}
			}
			n++;
		}
	}

}
