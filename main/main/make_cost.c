/* make_cost.c */

/********************************************/
/*  RD����u���b�N���Ƃ̃R�X�g�����߂܂��D 	*/
/********************************************/


#include <stdio.h>
#include "ica.h"
// Rate,Dstans����u���b�N���Ƃ̃R�X�g�����߂�
// RD�œK�� -> ���O�����W���iJ = Distans + �� * Rate�j�ɂ���@
void make_cost(double rate[1024], double dis[1024], double cost[1024]){
	int i;

	for(i = 0; i < 1024; i++){
		// �ɂ͎��O�Ɏw��i�l���ς��\������j
		cost[i] = dis[i] + lambda * rate[i];
	}

}

