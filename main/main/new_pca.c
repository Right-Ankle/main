/* new_pca.c */

/****************************************************************/
/*   主成分分析です。入力画像を引数に渡し，結果を構造体で返します。	*/
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
	static double x2[64][1024] = { 0 }, sum = 0; // x2[64][1024] -> 原画像を64(ブロック内番号)と1024(ブロック番号)に再編成
	int i, j, k, l;
	int am = 0;
	int an = 0;
	static struct pca pcaTemp = { 0 };

	am = an = 0; // 必要？
    // 原画像 -> 64(ブロック内番号)と1024(ブロック番号)に再編成
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
    // メモリ領域を確保
	r = (double *)malloc(sizeof(double)* p);
	x = (double **)malloc(sizeof(double *)* p);
	a = (double **)malloc(sizeof(double *)* p);
	for (i1 = 0; i1 < p; i1++) {
		x[i1] = (double *)malloc(sizeof(double)* n); // xは64, x[1024]
		a[i1] = (double *)malloc(sizeof(double)* p); // aは64, a[64]
	}

	for (i1 = 0; i1 < n; i1++) {   // データ
		for (i2 = 0; i2 < p; i2++){
			x[i2][i1] = x2[i2][i1]; // 再構成したx2[64][1024]を動的メモリに格納
		}
	}

	sw = principal(p, n, x, r, a, 1.0e-10, 10000);  // 固有値と固有ベクトルの計算（ヤコビ法）

	pcaTemp.p = p;

	if (sw == 0) {
		for (i1 = 0; i1 < p; i1++) {
			pcaTemp.r[i1] = r[i1];  //pcaTemp.r -> 分散
			for (i2 = 0; i2 < p; i2++)
				pcaTemp.a[i1][i2] = a[i1][i2];  //pcaTemp.r -> 相関行列
		}
	}
	else
		printf("***error  解を求めることができませんでした\n");

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
/*     主成分分析                      */
/*          p : 変数の数     （64）      */
/*          n : データの数   (1024)      */
/*          x : データ  (origin 64*1024)               */
/*          r : 分散（主成分）         */
/*          a : 係数  (固有ベクトル)                 */
/*          eps : 収束性を判定する規準   （0e-10）*/
/*          ct : 最大繰り返し回数  （10000） */
/*          return : =0 : 正常         */
/*                   =1 : エラー       */
/***************************************/
int principal(int p, int n, double **x, double *r, double **a, double eps, int ct)
{
	// c[][] -> 相関行列,
	static double **A1, **A2, **C, mean, **X1, **X2, s2;
	int i1, i2, i3, sw = 0;
	// 領域の確保
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
	// データの基準化(標準化)
	// 正規化 -> データの尺度を統一化する　基準化 -> 正規化の中でも平均を0,分散を1にすること
	for (i1 = 0; i1 < p; i1++) {
		mean = 0.0;
		s2 = 0.0;
		for (i2 = 0; i2 < n; i2++) {
			mean += x[i1][i2];// ブロック内の和
			s2 += x[i1][i2] * x[i1][i2];  // ブロック内の２乗の和
		}
		mean /= n;  // ブロック内の平均
		// 分散の処理？
		s2 += x[i1][i2] * x[i1][i2];
		s2 /= n;
		s2 = n * (s2 - mean * mean) / (n - 1);
		s2 = sqrt(s2);
		for (i2 = 0; i2 < n; i2++){
		    // 相関行列
			x[i1][i2] = (x[i1][i2] - mean)/*/ s2*/;
		}
	}
	// 分散強分散行列の計算
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
	// 固有値と固有ベクトルの計算（ヤコビ法）
	// 正常なら  sw = 0
	sw = Jacobi(p, ct, eps, C, A1, A2, X1, X2);

	if (sw == 0) {
		for (i1 = 0; i1 < p; i1++) {
			r[i1] = A1[i1][i1]; // 固有値（対角線上の値）
			for (i2 = 0; i2 < p; i2++)
				a[i1][i2] = X1[i2][i1];  // 固有ベクトル
		}
	}
	// 領域の解放
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
/*     実対称行列の固有値・固有ベクトル（ヤコビ法）              */
/*          n : 次数                                             */
/*          ct : 最大繰り返し回数  （10000回）                         */
/*          eps : 収束判定条件   （1/10^10）                            */
/*          A : 対象とする行列                                   */
/*          A1, A2 : 作業域（nxnの行列），A1の対角要素が固有値   */
/*          X1, X2 : 作業域（nxnの行列），X1の各列が固有ベクトル */
/*          return : =0 : 正常                                   */
/*                   =1 : 収束せず                               */
/*          coded by Y.Suganuma                                  */

/*     これの固有値と固有ベクトルを求めることを「固有値問題を解く」といいます。
　　　　なぜ固有値問題を解くと主成分が求まるのか、以下のような流れがあるようです。
１：分散が最も大きくなるように線（主成分軸）を引く方法を数式であらわしてみた。
２：その式は、分散共分散行列に対して固有値問題を解いた時の固有ベクトルに等しい

********************* ヤコビ法 *************************
A(信号) = D(対角成分) + L + U(非対角成分)としたとき,
Ax = (D + L + U)x = b　これを変形すると,
Dx(k+1) = b - (L+U)x(k) となる。
x(k)が収束したとき, x(k+1)も収束するため, (k) = x(k+1)と表せる.
収束したものを α と置くと, Aα = (D + L + U)α = b となり,
α は連立方程式の解となる。
/*****************************************************************/
#include <math.h>

int Jacobi(int n, int ct, double eps, double **A, double **A1, double **A2,
	double **X1, double **X2)
{
	double max, s, t, v, sn, cs;
	int i1, i2, k = 0, ind = 1, p = 0, q = 0; // （対角行列以外での最大値）max[i1][i2] -> i1 = p, i2 = q;
	// 初期設定
	for (i1 = 0; i1 < n; i1++) {
		for (i2 = 0; i2 < n; i2++) {
			A1[i1][i2] = A[i1][i2]; //対象行列（n*n）を作業領域へコピー
			X1[i1][i2] = 0.0;  // 初期化
		}
		X1[i1][i1] = 1.0;  // 対角線の値を1,それ以外は0
	}
	// 計算
	while (ind > 0 && k < ct) {
		// 最大要素の探索
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
		// 収束判定
		// 収束した
		if (max < eps) // 最大値が（1/10^10）未満まで継続
			ind = 0;
		// 収束しない
		else {
			// 準備
			s = -A1[p][q];
			t = 0.5 * (A1[p][p] - A1[q][q]);
			v = fabs(t) / sqrt(s * s + t * t);
			sn = sqrt(0.5 * (1.0 - v));
			if (s*t < 0.0)
				sn = -sn;
			cs = sqrt(1.0 - sn * sn);
			// Akの計算
			// わかんない
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
			// Xkの計算
			// わかんない
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
			// 次のステップへ
			k++; //繰り返し変数
			for (i1 = 0; i1 < n; i1++) {
				for (i2 = 0; i2 < n; i2++) {
					// 計算後の配列を元の配列に格納　次の段階へ
					// 10000回繰り返す
					A1[i1][i2] = A2[i1][i2];  //　固有値
					X1[i1][i2] = X2[i1][i2];  //　固有ベクトル
				}
			}
		}
	}

	return ind;
}