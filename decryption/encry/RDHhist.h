#pragma once
#ifndef RDH_HIST_H_INCLUDED
#define RDH_HIST_H_INCLUDED

typedef struct {
    int peakPoint;
    int zeroPoint;
} PointPair;

typedef struct {
    unsigned char bitv : 1;
} bitval;

typedef struct {
    int h;              //像素对差值
    unsigned char l;    //像素对均值
    unsigned char hset; //差值集合
} diffEx;

extern int histogram_map[256];

PointPair ReadHistMap(int* histogram_map, unsigned char* img_data, int img_size);

void EmbedInfo(unsigned char* img_data, int width, int img_size, unsigned char* roiCoorTagData, int CoorTagByteNum, double* lgVal);
void ExtractInfo();

#endif // RDH_HIST_H_INCLUDED
