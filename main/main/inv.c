/* inv.c */

/****************************************************************/
/*   w �̋t�s��� inv_w �Ɋi�[���܂��B�iw �̒��g�����������܂��j	*/
/****************************************************************/

#include<stdio.h>
// w -> ?, inv_w -> w�̋t�s��
void inv(double w[][64], double inv_w[][64]){

	double temp;
	int i, j, k;
    // �������F�Ίp����1, ����ȊO��0
	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			if(i == j)
				inv_w[i][j] = 1.0;
			else
				inv_w[i][j] = 0.0;
		}
	}

	for(i = 0; i < 64; i++){
		// w -> �Ίp��^-1
		temp = 1.0 / w[i][i];
		for(j = 0; j < 64; j++){
			// w -> w�̑Ίp������1��
			w[i][j] *= temp;
			// inv_w -> �Ίp�����̕ۑ�
			inv_w[i][j] *= temp;
		}
		for(j = 0; j < 64; j++){
			if(i != j){
				temp = w[j][i];
				for(k = 0; k < 64; k++){
				    // w -> w - �Ώۂ�i��̐�
					w[j][k] -= w[i][k] * temp;
					inv_w[j][k] -= inv_w[i][k] * temp;
				}
			}
		}
	}
}