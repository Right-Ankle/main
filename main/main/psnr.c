/* psnr.c */

/****************************************************************/
/*  �����Ƃ��ēn���ꂽ2�̉摜����PSNR���v�Z���C���̒l��Ԃ��܂��B	*/
/****************************************************************/

#include<stdio.h>
#include<math.h>

double psnr(unsigned char origin[][256], unsigned char saikousei[][256]){

	int i, j;
	double PSNR, sum = 0.0;

	for(i = 0; i < 256; i++)
		for(j = 0; j < 256; j++)
			sum += pow(origin[i][j] - saikousei[i][j], 2);
      
    PSNR = 10 * log10((double)(255 * 255) / (sum / (double)(256 * 256)));
	
	return PSNR;
}