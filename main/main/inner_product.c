/* inner_product.c */
#include<stdio.h>
#include<math.h>

//内積を求める->ｂがどれだけ仕事をしたか返す
double inner_product(double a[64], double b[64]){
	int i;
	double ad, bd, sum;

	sum = 0.0;

	for(i = 0; i < 64; i++)
		sum += pow(a[i], 2);
	ad = sqrt(sum);

	sum = 0.0;

	for(i = 0; i < 64; i++)
		sum += pow(b[i], 2);
	bd = sqrt(sum);

	sum = 0.0;

	for(i = 0; i < 64; i++)
		sum += a[i] * b[i];

	return sum / (ad * bd);
}