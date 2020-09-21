/* make_cost.c */

/********************************************/
/*  RDからブロックごとのコストを求めます． 	*/
/********************************************/


#include <stdio.h>
#include "ica.h"
// Rate,Dstansからブロックごとのコストを求める
// RD最適化 -> ラグランジュ（J = Distans + λ * Rate）により解法
void make_cost(double rate[1024], double dis[1024], double cost[1024]){
	int i;

	for(i = 0; i < 1024; i++){
		// λは事前に指定（値が変わる可能性あり）
		cost[i] = dis[i] + lambda * rate[i];
	}

}

