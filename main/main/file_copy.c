

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>
//#include <sys/stat.h>
#include "ica.h"

void my_copy(char* from[], char* to[]) {
    FILE* f1, * f2;
    int c;

    f1 = fopen(from, "r");
    if (!f1) err(from);

    //file_exist(to);
    f2 = fopen(to, "w");
    if (!f2) err(to);

    while ((c = fgetc(f1)) != EOF) {
        if (fputc(c, f2) == EOF) err(to);
    }

    fclose(f2);
    fclose(f1);
}

int err(const char* s) {
    perror(s);
    exit(1);
}

//コピー先のファイルが既に存在しているならエラー終了
//void file_exist(char* fname[]) {
//    struct stat st;    //#include <sys/stat.h>が必要
//
//    if (!stat(fname, &st)) {
//        printf("File %s already exists.\n", fname);
//        exit(1);
//    }
//}