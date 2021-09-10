
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
#include <string.h>
#include "ica.h"

int mirror(int x, int min, int max) {
    while (x < min || x >= max) {
        if (x < min) x = min + (min - x - 1);
        if (x >= max) x = max + (max - x - 1);
    }

    return x;
}

double SQR(double x) {
    return x * x;
}

double SSIM(unsigned char data1[][256], unsigned char data2[][256], int width, int height) {

    double c1 = pow(0.01 * 255, 2);
    double c2 = pow(0.03 * 255, 2);
    double data1_temp[256 * 256];
    double data2_temp[256 * 256];

    int m = 2;

    double Sum = 0.0;

    int mx, my, a, b;

    for (a = 0; a < 256; a++)
        for (b = 0; b < 256; b++) {
            data1_temp[a * 256 + b] = (double)data1[a][b];
            data2_temp[a * 256 + b] = (double)data2[a][b];
        }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            double ave1 = 0.0, ave2 = 0.0; // •½‹Ï
            double var1 = 0.0, var2 = 0.0; // •ªŽU
            double cov = 0.0; // ‹¤•ªŽU

            for (int dy = -m; dy <= m; dy++) {
                for (int dx = -m; dx <= m; dx++) {
                    mx = mirror(x + dx, 0, width);
                    my = mirror(y + dy, 0, height);

                    ave1 += data1_temp[mx + my * width];
                    ave2 += data2_temp[mx + my * width];
                }
            }
            ave1 /= SQR(m * 2.0 + 1.0);
            ave2 /= SQR(m * 2.0 + 1.0);

            for (int dy = -m; dy <= m; dy++) {
                for (int dx = -m; dx <= m; dx++) {
                    mx = mirror(x + dx, 0, width);
                    my = mirror(y + dy, 0, height);

                    var1 += SQR(data1_temp[mx + my * width] - ave1);
                    var2 += SQR(data2_temp[mx + my * width] - ave2);
                    cov += (data1_temp[mx + my * width] - ave1) * (data2_temp[mx + my * width] - ave2);
                }
            }
            var1 /= SQR(m * 2.0 + 1.0);
            var2 /= SQR(m * 2.0 + 1.0);
            cov /= SQR(m * 2.0 + 1.0);

            Sum += ((2.0 * ave1 * ave2 + c1) * (2.0 * cov + c2)) / ((SQR(ave1) + SQR(ave2) + c1) * (var1 + var2 + c2));
        }
    }

    return Sum / (width * height);
}