#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include "RDH_hist.h"
#include"Chaos.h"

int histogram_map[256] = { 0 };

PointPair ReadHistMap(int* histogram_map, unsigned char* img_data, int imglen) {
    int i;
    PointPair pointPair;
    for (i = 0; i < imglen; ++i)
        ++histogram_map[img_data[i]];
    int tmpPeak = histogram_map[0];
    int tmpZero = histogram_map[0];
    for (i = 0; i < 256; ++i) {
        if (histogram_map[i] >= tmpPeak) {
            tmpPeak = histogram_map[i];
            pointPair.peakPoint = i;
        }
        if (histogram_map[i] <= tmpZero) {
            tmpZero = histogram_map[i];
            pointPair.zeroPoint = i;
        }
    }
    //printf("%d %d\n", pointPair.peakPoint, pointPair.zeroPoint);
    return pointPair;
}


void EmbedInfo(unsigned char* img_data, int width, int img_size, unsigned char* roiCoorTagData, int CoorTagByteNum, double* lgVal) {
    //74个byte,592个bit
    int i, j;
    int emdpos, leftpos;
    bitval curbit;
    diffEx* diffExPair = (diffEx*)malloc(sizeof(diffEx) * CoorTagByteNum * 8);
    for (i = 0; i < CoorTagByteNum; ++i) {
        for (j = 0; j < 8; ++j) {
            Iter_logistic(lgVal);
            emdpos = i * width + (long long)(*lgVal * (1.0e+15)) % width;
            leftpos = emdpos - 1;
            diffExPair[i].l = (int)floor((img_data[emdpos] + img_data[leftpos]) / 2.0);
            diffExPair[i].h = abs(img_data[leftpos] - img_data[emdpos]);
            curbit.bitv = (roiCoorTagData[i] >> j) & 0x01;     //要嵌入的bit值
            diffExPair[i].h = 2 * diffExPair[i].h + curbit.bitv;

            if (img_data[leftpos] > img_data[emdpos]) {
                img_data[leftpos] = diffExPair[i].l + (int)floor((diffExPair[i].h + 1) / 2.0);
                img_data[emdpos] = diffExPair[i].l - (int)floor(diffExPair[i].h / 2.0);              //新的像素值
                diffExPair[i].l = (int)floor((img_data[emdpos] + img_data[leftpos]) / 2.0);
                diffExPair[i].h = abs(img_data[emdpos] - img_data[leftpos]);
                bitval etbit;
                etbit.bitv = diffExPair[i].h & 0x01;    //这里只是做了比较，但是并没有提取出来啊哈哈哈，不过先不管了
                if (etbit.bitv != curbit.bitv)
                    printf("wrong\n");
                img_data[leftpos] = diffExPair[i].l + (int)floor((diffExPair[i].h + 1) / 2.0);  //其实是重复代码了
                img_data[emdpos] = diffExPair[i].l - (int)floor(diffExPair[i].h / 2.0);
            }
            else {
                img_data[emdpos] = diffExPair[i].l + (int)floor((diffExPair[i].h + 1) / 2.0);
                img_data[leftpos] = diffExPair[i].l - (int)floor(diffExPair[i].h / 2.0);
                diffExPair[i].l = (int)floor((img_data[emdpos] + img_data[leftpos]) / 2.0);
                diffExPair[i].h = abs(img_data[emdpos] - img_data[leftpos]);
                bitval etbit;
                etbit.bitv = diffExPair[i].h & 0x01;
                if (etbit.bitv != curbit.bitv)
                    printf("wrong\n");
                img_data[emdpos] = diffExPair[i].l + (int)floor((diffExPair[i].h + 1) / 2.0);
                img_data[leftpos] = diffExPair[i].l - (int)floor(diffExPair[i].h / 2.0);
            }
        }
    }
    free(diffExPair);
}

