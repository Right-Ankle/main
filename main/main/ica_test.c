
/*
 * C言語のサンプルプログラム - Webkaru
 * - 入力した2つの数値を交換 -
 */
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS





int main(void)
{

  /* 変数の宣言 */
  int a, b, tmp, i, j, p, k, rnd, n,l,s;
  double x[4][16], s2, y[4][4],mean1,mean2, s3, mean, s4,w[4][16];
  p = 64;
  n = 1024;
  s = 256;

/*********************************************************************************************/
  static int ori_temp[256 * 256] = { 0 };
	static unsigned char origin[256][256] = { 0 };	//原画像（256*256のみ対応）
  static char image_name[20] = { 0 };	//画像ファイル名(拡張子含まず)
	FILE *fp;
	unsigned char *data;
	unsigned char header[54];
	unsigned char palette[1024];
	long width, height;
	unsigned int bitcount;
	char temp[80];
  int h_size, v_size;
  static char filename[20] = { 'b', 'a', 'r', 'b', 'a', 'r', 'a', '.', 'b', 'm', 'p' };
  char output[25];


  h_size = v_size = s;  //256合わせ

	/* ファイル読み込み */
	sprintf(temp, "image/%s", filename);
	if ((fp = fopen(temp, "rb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* 拡張子を除いたファイル名を返す */
	for (i = 0; filename[i] != '.'; i++)
		image_name[i] = filename[i];
	image_name[i] = '\0';

	/* メモリ確保 */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* ビットマップ画像の読み込み */
	// fread(header, 1, 54, fp);
	// memcpy(&width, header + 18, sizeof(width));
	// memcpy(&height, header + 22, sizeof(height));
	// if (width != h_size || height != v_size) {
  //   printf("width:%ld, height:%ld, header:%s\n",width,height,header);
	// 	printf("指定した画像サイズと入力画像の画像サイズが異なっています\n");
	// 	return -1;
	// }
	// memcpy(&bitcount, header + 28, sizeof(bitcount));
	// if (bitcount != 8) {
	// 	printf("入力画像はグレースケール画像ではありません\n");
	// 	return -1;
	// }
	fread(palette, 1, 1024, fp);
	fread(data, 1, h_size * v_size, fp);
	fclose(fp);

	/* bmp形式では色配列が左下→右上になっているので，左上→右下に補正する */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			ori_temp[(((v_size - 1) - i) * h_size) + j] = (int)data[i * h_size + j];

	/* メモリ解放 */
	free(data);
/*********************************************************************************************/

for (i = 0; i < s; i++)
	for (j = 0; j < s; j++)
		origin[i][j] = ori_temp[i * 256 + j];

/*********************************************************************************************/
  sprintf(output, "gray/%s_gray.bmp", image_name);

  //FILE *fp;


	/* ヘッダー情報の引数の定義と設定 */
	unsigned char Header[54];
	unsigned int bfType = 'MB';
	unsigned long bfSize = h_size * v_size + 54 + 1024;
	unsigned int bfReserved1 = 0;
	unsigned int bfReserved2 = 0;
	unsigned long bfOffBits = 54 + 1024;
	unsigned long biSize = 40;
	long biWidth = h_size;
	long biHeight = v_size;
	unsigned int biPlanes = 1;
	unsigned int biBitCount = 8;
	unsigned long biCompression = 0;
	unsigned long biSizeImage = h_size * v_size;
	long biXPixPerMeter = 0;
	long biYPixPerMeter = 0;
	unsigned long biClrUsed = 0;
	unsigned long biClrImportant = 0;
  unsigned char jalette[1030];


	if ((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "ファイルを開けませんでした\n");
		exit(1);
	}

	/* ファイルヘッダー */
	memcpy(Header + 0, &bfType, sizeof(bfType));
	memcpy(Header + 2, &bfSize, sizeof(bfSize));
	memcpy(Header + 6, &bfReserved1, sizeof(bfReserved1));
	memcpy(Header + 8, &bfReserved2, sizeof(bfReserved2));
	memcpy(Header + 10, &bfOffBits, sizeof(bfOffBits));

	/* 情報ヘッダー */
	memcpy(Header + 14, &biSize, sizeof(biSize));
	memcpy(Header + 18, &biWidth, sizeof(biWidth));
	memcpy(Header + 22, &biHeight, sizeof(biHeight));
	memcpy(Header + 26, &biPlanes, sizeof(biPlanes));
	memcpy(Header + 28, &biBitCount, sizeof(biBitCount));
	memcpy(Header + 30, &biCompression, sizeof(biCompression));
	memcpy(Header + 34, &biSizeImage, sizeof(biSizeImage));
	memcpy(Header + 38, &biXPixPerMeter, sizeof(biXPixPerMeter));
	memcpy(Header + 42, &biYPixPerMeter, sizeof(biYPixPerMeter));
	memcpy(Header + 46, &biClrUsed, sizeof(biClrUsed));
	memcpy(Header + 50, &biClrImportant, sizeof(biClrImportant));

	/* パレットデータ */

	for (i = 0; i < 256; i++) {

    jalette[i * 4] = i;
		jalette[i * 4 + 1] = i;
		jalette[i * 4 + 2] = i;
		jalette[i * 4 + 3] = 0;
	}

	/* メモリ確保 */
	data = (unsigned char *)malloc(sizeof(unsigned char) * h_size * v_size);

	/* 色配列を左上→右下から，bmp形式用に左下→右上に直す */
	for (i = 0; i < v_size; i++)
		for (j = 0; j < h_size; j++)
			data[(((v_size - 1) - i) * h_size) + j] = ori_temp[i * h_size + j];

	/* ファイル書き出し */
	fwrite(Header, 1, 54, fp);
	fwrite(jalette, 1, 1024, fp);
	for (i = 0; i < h_size * v_size; i++)
		fwrite(&data[i], 1, 1, fp);

	/* メモリ解放 */
	free(data);

	fclose(fp);
  /*********************************************************************************************/

//   p=4;
//   n=16;
//   s3=0.0;
//   s4=0.0;

//   for(i=0;i<p;i++){
//     for(j=0;j<n;j++){
//         rnd=rand();
//         rnd=(double)(rnd%256);
//         x[i][j]=rnd;
//         rnd=0;
//     }
//   }
//   for(i=0;i<p;i++){
//     for(j=0;j<p;j++){
//       y[i][j]=0.0;
//       rnd=rand();
//       rnd=(double)(rnd%256);
//       y[i][j]=rnd;
//       rnd=0;
//       if(i==j)
//       y[i][i]=0;
//     }
//   }

// printf("===== 元の構造体[x] =====\n");
// for (i = 0; i < p; i++) {
//   for (j = 0; j < n; j++) {
//     printf(" ,%d", (int)x[i][j]);
//     if(j==n-1)
//       printf("\n");
//   }
// }

// printf("===== 元の構造体[y] =====\n");
// for (i = 0; i < p; i++) {
//   for (j = 0; j < p; j++) {
//     printf(" ,%d", (int)y[i][j]);
//     if(j==p-1)
//       printf("\n");
//   }
// }

// 	for(i = 0; i < p; i++){
// 		for(j = 0; j < n; j++){
// 			for(k = 0; k < p; k++){
// 				w[i][j] += y[i][k] * x[k][j];
// 			}
// 		}
// 	}

// printf("===== 行列積　計算後 =====\n");
// for (i = 0; i < p; i++) {
//   for (j = 0; j < n; j++) {
//     printf(" ,%d", (int)(w[i][j]));
//     if(j==n-1)
//       printf("\n");
//   }
// }

//   printf("==相関行列==\n");
// 	for (i = 0; i < p; i++) {
// 		for (j = 0; j < p; j++) {
//       s2 = 0.0;
//       s3 = 0.0;
//       s4 = 0.0;
//       mean = 0.0;
//       l=0;
//       mean1=mean2=0.0;
//         for(k = 0; k < n; k++){
//           mean1 += x[i][k];
//           mean2 += x[j][k];
//         }
//         mean1 /= (double)n;
//         mean2 /= (double)n;
//         for(k = 0; k < n; k++){
//         s2 += (x[i][k] - mean1)*(x[j][k] - mean2);
//         //s3 += pow((x[i][k] - mean1),2.0); // 相関行列で使用
//         //s4 += pow((x[j][k] - mean2),2.0);
//         }
//         s2 = s2 / (double)(n-1)/*/ (sqrt(s3) * sqrt(s4))*/;

//         if(i<=j)
//           y[j][i] = s2;
//         l++;
// 		}

// }

// printf("-- 分散共分散行列 --\n");
// for (i = 0; i < p; i++) {
// 		for (j = 0; j < p; j++) {
//           if(j==0&&i==0){
//             printf("_|");
//             for(k=0;k<p;k++)
//                 printf("_____%d____|",k+1);
//             printf("\n");
//         }
//         if(j==0)
//         printf("%d|",i+1);
//         if(i >= j){
//           if(y[i][j]<0){
//             printf("%f ,", y[i][j]);
//           }else{
//             printf(" %f ,", y[i][j]);
//           }
//         }

//         if(j==p-1)
//         printf("\n");

// 		}
// }


//   printf("-----基準化処理-----\n");
// 	for (i = 0; i < p; i++) {
// 		mean = 0.0;
// 		s2 = 0.0;
// 		for (j = i; j < n; j++) {
// 			mean += x[i][j]; // ブロック番号の平均
// 			s2 += x[i][j] * x[i][j];  // ブロック番号の２乗の和
// 		}
// 		mean /= n;  // ブロック番号の平均
// //
//     printf("横の平均[%d]:%f ,", i+1,mean);
// //
// 		// 分散の処理？
// 		 s2 += x[i][j] * x[i][j];
// //
//     printf("横の二乗和[%d][%d]:%f ,", i, j, s2);
// //
// 		s2 /= (double)n; //２乗和平均
// 		s2 = (double)n * (s2 - mean * mean) / ((double)n - 1.0);
// //
//     printf("横の二乗和平均[%d][%d]:%f ,", i, j, s2);
// //
// 		s2 = sqrt(s2);
// //
//     printf("sqrt(s2)[%d][%d]:%f ,", i, j, s2);
//     printf("\n");
// //
// 		for (j = 0; j < n; j++){
// 		    // (ブロック内すべて) 元値 - 平均値 (相関行列)  /s2するとバグる
// 			x[i][j] = (x[i][j] - mean);// /s2
// 	}
// }



//   for (i = 0; i < p; i++) {
// 		for (j = i; j < p; j++) {
// 			s2 = 0.0;
// 			for (k = 0; k < n; k++)
// 				s2 += x[i][k] * x[j][k];
// 			s2 /= (n - 1);
// 			y[i][j] = s2;
// 			if (i != j)
// 				y[j][i] = s2;
// 		}
// 	}


//   /* 2つの数値を入力 */
//   /*
//   printf("a = ");
//   scanf("%d", &a);


//   printf("b = ");
//   scanf("%d", &b);
//   */


//   /* 数値の入れ替え */
//   /*
//   tmp = b;
//   b = a;
//   a = tmp;
//   x=0;
//   for(i=0;i<9;i++){
//   x++;
//   }
//   */
// /*
// printf("===== Result =====\n");
// for (i = 0; i < p; i++) {
//   for (j = 0; j < n; j++) {
//     printf("%f ,", x[i][j]);
//     if(j==n-1)
//       printf("\n");
//   }
// }
// */
// printf("\n↓計算後\n");
// for (i = 0; i < p; i++) {
// 	for (j = 0; j < p; j++) {
//     printf("%f ,", y[i][j]);
//     if(j==p-1)
//       printf("\n");
//   }
// }


  return 0;
}
