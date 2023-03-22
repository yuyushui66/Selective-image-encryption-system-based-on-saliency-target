#pragma once
#ifndef ROI_INFO_H_INCLUDED
#define ROI_INFO_H_INCLUDED
#include"ReadBMP.h"
typedef struct {
    unsigned char roiVal;
    int readFlag;
} roiData;

typedef struct {
    int left;
    int right;
    int top;
    int bot;
    int width;
    int height;
    char objName[10];
    int objNameLen;
} ROI;

int ReadObjInfo(ROI* detObjInfo, int objNum, char* coorfile, message t);
int GetRoiPixelNum(ROI* detObjInfo, int n, roiData** img_Data2D, unsigned char* imgRoiData, int WriteOrRead);
void CoorTagInfoInt2Unsign(ROI* detObjInfo, int n, int CoorTagByteNum, unsigned char* roiCoorTagData);

#endif // ROI_INFO_H_INCLUDED
