/* new_pca.c */

/****************************************************************/
/*   �听�����͂ł��B���͉摜�������ɓn���C���ʂ��\���̂ŕԂ��܂��B	*/
/****************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ica.h"

int principal(int p, int n, double **x, double *r, double **a, double eps, int ct);
int Jacobi(int n, int ct, double eps, double **A, double **A1, double **A2, double **X1, double **X2);

struct pca new_pca(unsigned char gen[256][256])
{
	static double **x, *r, **a;
	int i1, i2, n, p, sw;
	static double x2[64][1024] = { 0 }, sum = 0; // x2[64][1024] -> ���摜��64(�u���b�N���ԍ�)��1024(�u���b�N�ԍ�)�ɍĕҐ�
	int i, j, k, l;
	int am = 0;
	int an = 0;
	static struct pca pcaTemp = { 0 };

	am = an = 0; // �K�v�H
    // ���摜 -> 64(�u���b�N���ԍ�)��1024(�u���b�N�ԍ�)�ɍĕҐ�
	for (i = 0; i < 256; i += 8){
		for (j = 0; j < 256; j += 8){
			am = 0;
			for (k = 0; k < 8; k++){
				for (l = 0; l < 8; l++){
					x2[am][an] = (double)gen[i + k][j + l];
					am++;
				}
			}
			an++;
		}
	}


	p = 64;
	n = 1024;
    // �������̈���m��
	r = (double *)malloc(sizeof(double)* p);
	x = (double **)malloc(sizeof(double *)* p);
	a = (double **)malloc(sizeof(double *)* p);
	for (i1 = 0; i1 < p; i1++) {
		x[i1] = (double *)malloc(sizeof(double)* n); // x��64, x[1024]
		a[i1] = (double *)malloc(sizeof(double)* p); // a��64, a[64]
	}

	for (i1 = 0; i1 < n; i1++) {   // �f�[�^
		for (i2 = 0; i2 < p; i2++){
			x[i2][i1] = x2[i2][i1]; // �č\������x2[64][1024]�𓮓I�������Ɋi�[
		}
	}

	sw = principal(p, n, x, r, a, 1.0e-10, 10000);  // �ŗL�l�ƌŗL�x�N�g���̌v�Z�i���R�r�@�j

	pcaTemp.p = p;

	if (sw == 0) {
		for (i1 = 0; i1 < p; i1++) {
			pcaTemp.r[i1] = r[i1];  //pcaTemp.r -> ���U
			for (i2 = 0; i2 < p; i2++)
				pcaTemp.a[i1][i2] = a[i1][i2];  //pcaTemp.r -> ���֍s��
		}
	}
	else
		printf("***error  �������߂邱�Ƃ��ł��܂���ł���\n");

	for (i1 = 0; i1 < p; i1++) {
		free(x[i1]);
		free(a[i1]);
	}
	free(x);
	free(a);
	free(r);

	return pcaTemp;
}

/***************************************/
/*     �听������                      */
/*          p : �ϐ��̐�     �i64�j      */
/*          n : �f�[�^�̐�   (1024)      */
/*          x : �f�[�^  (origin 64*1024)               */
/*          r : ���U�i�听���j         */
/*          a : �W��  (�ŗL�x�N�g��)                 */
/*          eps : �������𔻒肷��K��   �i0e-10�j*/
/*          ct : �ő�J��Ԃ���  �i10000�j */
/*          return : =0 : ����         */
/*                   =1 : �G���[       */
/***************************************/
int principal(int p, int n, double **x, double *r, double **a, double eps, int ct)
{
	// c[][] -> ���֍s��,
	static double **A1, **A2, **C, mean, **X1, **X2, s2;
	int i1, i2, i3, sw = 0;
	// �̈�̊m��
	C = (double **)malloc(sizeof(double *)* p);
	A1 = (double **)malloc(sizeof(double *)* p);
	A2 = (double **)malloc(sizeof(double *)* p);
	X1 = (double **)malloc(sizeof(double *)* p);
	X2 = (double **)malloc(sizeof(double *)* p);
	for (i1 = 0; i1 < p; i1++) {
		C[i1] = (double *)malloc(sizeof(double)* p);
		A1[i1] = (double *)malloc(sizeof(double)* p);
		A2[i1] = (double *)malloc(sizeof(double)* p);
		X1[i1] = (double *)malloc(sizeof(double)* p);
		X2[i1] = (double *)malloc(sizeof(double)* p);
	}
	// �f�[�^�̊��(�W����)
	// ���K�� -> �f�[�^�̎ړx�𓝈ꉻ����@��� -> ���K���̒��ł����ς�0,���U��1�ɂ��邱��
	for (i1 = 0; i1 < p; i1++) {
		mean = 0.0;
		s2 = 0.0;
		for (i2 = 0; i2 < n; i2++) {
			mean += x[i1][i2];// �u���b�N���̘a
			s2 += x[i1][i2] * x[i1][i2];  // �u���b�N���̂Q��̘a
		}
		mean /= n;  // �u���b�N���̕���
		// ���U�̏����H
		s2 += x[i1][i2] * x[i1][i2];
		s2 /= n;
		s2 = n * (s2 - mean * mean) / (n - 1);
		s2 = sqrt(s2);
		for (i2 = 0; i2 < n; i2++){
		    // ���֍s��
			x[i1][i2] = (x[i1][i2] - mean)/*/ s2*/;
		}
	}
	// ���U�����U�s��̌v�Z
	for (i1 = 0; i1 < p; i1++) {
		for (i2 = i1; i2 < p; i2++) {
			s2 = 0.0;
			for (i3 = 0; i3 < n; i3++)
				s2 += x[i1][i3] * x[i2][i3];
			s2 /= (n - 1);
			C[i1][i2] = s2;
			if (i1 != i2)
				C[i2][i1] = s2;
		}
	}
	// �ŗL�l�ƌŗL�x�N�g���̌v�Z�i���R�r�@�j
	// ����Ȃ�  sw = 0
	sw = Jacobi(p, ct, eps, C, A1, A2, X1, X2);

	if (sw == 0) {
		for (i1 = 0; i1 < p; i1++) {
			r[i1] = A1[i1][i1]; // �ŗL�l�i�Ίp����̒l�j
			for (i2 = 0; i2 < p; i2++)
				a[i1][i2] = X1[i2][i1];  // �ŗL�x�N�g��
		}
	}
	// �̈�̉��
	for (i1 = 0; i1 < p; i1++) {
		free(C[i1]);
		free(A1[i1]);
		free(A2[i1]);
		free(X1[i1]);
		free(X2[i1]);
	}
	free(C);
	free(A1);
	free(A2);
	free(X1);
	free(X2);

	return sw;
}

/*****************************************************************/
/*     ���Ώ̍s��̌ŗL�l�E�ŗL�x�N�g���i���R�r�@�j              */
/*          n : ����                                             */
/*          ct : �ő�J��Ԃ���  �i10000��j                         */
/*          eps : �����������   �i1/10^10�j                            */
/*          A : �ΏۂƂ���s��                                   */
/*          A1, A2 : ��ƈ�inxn�̍s��j�CA1�̑Ίp�v�f���ŗL�l   */
/*          X1, X2 : ��ƈ�inxn�̍s��j�CX1�̊e�񂪌ŗL�x�N�g�� */
/*          return : =0 : ����                                   */
/*                   =1 : ��������                               */
/*          coded by Y.Suganuma                                  */

/*     ����̌ŗL�l�ƌŗL�x�N�g�������߂邱�Ƃ��u�ŗL�l���������v�Ƃ����܂��B
�@�@�@�@�Ȃ��ŗL�l���������Ǝ听�������܂�̂��A�ȉ��̂悤�ȗ��ꂪ����悤�ł��B
�P�F���U���ł��傫���Ȃ�悤�ɐ��i�听�����j���������@�𐔎��ł���킵�Ă݂��B
�Q�F���̎��́A���U�����U�s��ɑ΂��ČŗL�l�������������̌ŗL�x�N�g���ɓ�����

********************* ���R�r�@ *************************
A(�M��) = D(�Ίp����) + L + U(��Ίp����)�Ƃ����Ƃ�,
Ax = (D + L + U)x = b�@�����ό`�����,
Dx(k+1) = b - (L+U)x(k) �ƂȂ�B
x(k)�����������Ƃ�, x(k+1)���������邽��, (k) = x(k+1)�ƕ\����.
�����������̂� �� �ƒu����, A�� = (D + L + U)�� = b �ƂȂ�,
�� �͘A���������̉��ƂȂ�B
/*****************************************************************/
#include <math.h>

int Jacobi(int n, int ct, double eps, double **A, double **A1, double **A2,
	double **X1, double **X2)
{
	double max, s, t, v, sn, cs;
	int i1, i2, k = 0, ind = 1, p = 0, q = 0; // �i�Ίp�s��ȊO�ł̍ő�l�jmax[i1][i2] -> i1 = p, i2 = q;
	// �����ݒ�
	for (i1 = 0; i1 < n; i1++) {
		for (i2 = 0; i2 < n; i2++) {
			A1[i1][i2] = A[i1][i2]; //�Ώۍs��in*n�j����Ɨ̈�փR�s�[
			X1[i1][i2] = 0.0;  // ������
		}
		X1[i1][i1] = 1.0;  // �Ίp���̒l��1,����ȊO��0
	}
	// �v�Z
	while (ind > 0 && k < ct) {
		// �ő�v�f�̒T��
		max = 0.0;
		for (i1 = 0; i1 < n; i1++) {
			for (i2 = 0; i2 < n; i2++) {
				if (i2 != i1) {
					if (fabs(A1[i1][i2]) > max) {
						max = fabs(A1[i1][i2]);
						p = i1;
						q = i2;
					}
				}
			}
		}
		// ��������
		// ��������
		if (max < eps) // �ő�l���i1/10^10�j�����܂Ōp��
			ind = 0;
		// �������Ȃ�
		else {
			// ����
			s = -A1[p][q];
			t = 0.5 * (A1[p][p] - A1[q][q]);
			v = fabs(t) / sqrt(s * s + t * t);
			sn = sqrt(0.5 * (1.0 - v));
			if (s*t < 0.0)
				sn = -sn;
			cs = sqrt(1.0 - sn * sn);
			// Ak�̌v�Z
			// �킩��Ȃ�
			for (i1 = 0; i1 < n; i1++) {
				if (i1 == p) {
					for (i2 = 0; i2 < n; i2++) {
						if (i2 == p)
							A2[p][p] = A1[p][p] * cs * cs + A1[q][q] * sn * sn -
							2.0 * A1[p][q] * sn * cs;
						else if (i2 == q)
							A2[p][q] = 0.0;
						else
							A2[p][i2] = A1[p][i2] * cs - A1[q][i2] * sn;
					}
				}
				else if (i1 == q) {
					for (i2 = 0; i2 < n; i2++) {
						if (i2 == q)
							A2[q][q] = A1[p][p] * sn * sn + A1[q][q] * cs * cs +
							2.0 * A1[p][q] * sn * cs;
						else if (i2 == p)
							A2[q][p] = 0.0;
						else
							A2[q][i2] = A1[q][i2] * cs + A1[p][i2] * sn;
					}
				}
				else {
					for (i2 = 0; i2 < n; i2++) {
						if (i2 == p)
							A2[i1][p] = A1[i1][p] * cs - A1[i1][q] * sn;
						else if (i2 == q)
							A2[i1][q] = A1[i1][q] * cs + A1[i1][p] * sn;
						else
							A2[i1][i2] = A1[i1][i2];
					}
				}
			}
			// Xk�̌v�Z
			// �킩��Ȃ�
			for (i1 = 0; i1 < n; i1++) {
				for (i2 = 0; i2 < n; i2++) {
					if (i2 == p)
						X2[i1][p] = X1[i1][p] * cs - X1[i1][q] * sn;
					else if (i2 == q)
						X2[i1][q] = X1[i1][q] * cs + X1[i1][p] * sn;
					else
						X2[i1][i2] = X1[i1][i2];
				}
			}
			// ���̃X�e�b�v��
			k++; //�J��Ԃ��ϐ�
			for (i1 = 0; i1 < n; i1++) {
				for (i2 = 0; i2 < n; i2++) {
					// �v�Z��̔z������̔z��Ɋi�[�@���̒i�K��
					// 10000��J��Ԃ�
					A1[i1][i2] = A2[i1][i2];  //�@�ŗL�l
					X1[i1][i2] = X2[i1][i2];  //�@�ŗL�x�N�g��
				}
			}
		}
	}

	return ind;
}