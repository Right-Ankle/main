/* normal.c */

/************************************/
/*    �O�����E�V���~�b�g�̒������@ 	*/
/************************************/

#include<stdio.h>
#include<math.h>

// return w -> ��������
void normal(double w[][64]){
	// �x�N�g�����m�̓��ς�0,�傫����1
	// �@�x�N�g�������g�̑傫���Ŋ��� -> �傫�����P�ɂ���
	// �Aa2'*e1 = a2*e1 - (a2*e1)|e1|^2 -> �x�N�g��a2�Ɛ��K�����x�N�g��e1�̓��ς��O�ɂ���
	// �Be2 = a2/|a2| -> �傫�����P�ɂ���
	int i, j, k;
	double f[64] = {0}, temp[64] = {0}, a[64][64] = {0}, sum = 0, sum2 = 0;


	for(i = 0; i < 64; i++)
		for(j = 0; j < 64; j++)
			a[i][j] = w[i][j]; // w -> a �R�s�[

	for(i = 0; i < 64; i++)
		sum += pow(a[i][0], 2); // 1�s��2�悵�đ��a

	for(i = 0; i < 64; i++)
		w[i][0] = a[i][0] / sqrt(sum); // 1�s�ڂ�������


	for(i = 1; i < 64; i++){

		sum = 0;

		for(j = 0; j < 64; j++)
			f[j] = a[j][i]; // �c2�s�ڂ���c�ɃR�s�[ (a2e1?)

		for(j = 0; j < i; j++){

			sum2 = 0;

			for(k = 0; k < 64; k++)
				sum2 += w[k][j] * a[k][i]; // w��j�s�ځ�a��i�s�ڂ̑��a

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