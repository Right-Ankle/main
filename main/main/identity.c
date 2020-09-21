/* identity.c */

/********************************************/
/*�P�ʍs���z�̌��Z���ʂ�q�Ɋi�[���܂��B*/
/********************************************/

#include<stdio.h>
// q -> ???? - z
void identity(double z[][64], double q[][64]){

	int i, j;

	for(i = 0; i < 64; i++){
		for(j = 0; j < 64; j++){
			if(i == j){
				q[i][j] = 1.0 - z[i][j];
			}
			else{
				q[i][j] = -z[i][j];
			}
		}
	}
}