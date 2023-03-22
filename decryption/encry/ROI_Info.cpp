#include <stdio.h>
#include <string.h>
#include "ReadBMP.h"
#include "ROIInfo.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

int ReadObjInfo(ROI* detObjInfo, int objNum, char* coorfile,message t) {
    int height = t.height;
    int width = t.width;
    int i,j;
    int RoiInfoByteNum = 0;
    FILE* fp_obj;
    fp_obj = fopen(coorfile, "r");
    fscanf(fp_obj, "%d", &j);
    //printf("%s\n", coorfile);
    for (i = 0; i < objNum; ++i) {
        detObjInfo[i].objName[10] = '\0';
        fscanf(fp_obj, "%d%d%d%d%s", &detObjInfo[i].left, &detObjInfo[i].top, &detObjInfo[i].width, &detObjInfo[i].height, detObjInfo[i].objName);
        printf("%d %d %d %d\n", detObjInfo[i].left, detObjInfo[i].right, detObjInfo[i].bot, detObjInfo[i].top);
        detObjInfo[i].objNameLen = strlen(detObjInfo[i].objName);
        detObjInfo[i].left = (detObjInfo[i].left - 1) * 3 + 1;
        if (detObjInfo[i].left < 0)detObjInfo[i].left = 0;
        detObjInfo[i].width *= 3;
        detObjInfo[i].right = detObjInfo[i].left + detObjInfo[i].width + 2;
        detObjInfo[i].bot = detObjInfo[i].top + detObjInfo[i].height;                //ROI那里标记的bot其实读取图像顺序的“top”
        detObjInfo[i].top = height - detObjInfo[i].bot;    
        if (detObjInfo[i].top <= 0)detObjInfo[i].top = 1;//ROI那里标记的top其实是读取图像顺序的"bot",来回转的还真麻烦，上下左右傻傻分不清
        detObjInfo[i].bot = detObjInfo[i].top + detObjInfo[i].height;
        
        RoiInfoByteNum += 20;
        RoiInfoByteNum += detObjInfo[i].objNameLen;
        if (detObjInfo[i].top >= height)detObjInfo[i].top = height - 1;
        if (detObjInfo[i].bot >= height)detObjInfo[i].bot = height - 1;
        if (detObjInfo[i].left >= width)detObjInfo[i].left = width - 1;
        if (detObjInfo[i].right >= width)detObjInfo[i].right = width - 1;
        printf("%d %d %d %d\n", detObjInfo[i].left, detObjInfo[i].right, detObjInfo[i].bot, detObjInfo[i].top);
    }
    fclose(fp_obj);
    return RoiInfoByteNum;
    
}

int GetRoiPixelNum(ROI* cooroi, int n, roiData** img_Data2D, unsigned char* imgRoiData, int WriteOrRead) {
    int i, j, k, objN;
    k = 0;
    for (objN = 0; objN < n; ++objN) {
        for (j = cooroi[objN].top; j <= cooroi[objN].bot; ++j) {                //边界应该都是要的吧
            for (i = cooroi[objN].left; i <= cooroi[objN].right; ++i) {
                if (WriteOrRead == 0) {
                    if (img_Data2D[i][j].readFlag == 0) {
                        imgRoiData[k++] = img_Data2D[i][j].roiVal;
                        img_Data2D[i][j].readFlag = 1;
                    }
                }
                else {
                    if (img_Data2D[i][j].readFlag == 1) {
                        img_Data2D[i][j].roiVal = imgRoiData[k++];
                        img_Data2D[i][j].readFlag = 0;
                    }
                }
            }
        }
    }
    return k;
}

void CoorTagInfoInt2Unsign(ROI* detObjInfo, int n, int CoorTagByteNum, unsigned char* roiCoorTagData) {
    int i, j;
    int k = 0;
    for (i = 0; i < n; ++i) {
        roiCoorTagData[k++] = (detObjInfo[i].left >> 24) | 0x00;        //32位4byte
        roiCoorTagData[k++] = (detObjInfo[i].left >> 16) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].left >> 8) | 0x00;
        roiCoorTagData[k++] = detObjInfo[i].left | 0x00;

        roiCoorTagData[k++] = (detObjInfo[i].right >> 24) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].right >> 16) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].right >> 8) | 0x00;
        roiCoorTagData[k++] = detObjInfo[i].right | 0x00;

        roiCoorTagData[k++] = (detObjInfo[i].top >> 24) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].top >> 16) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].top >> 8) | 0x00;
        roiCoorTagData[k++] = detObjInfo[i].top | 0x00;

        roiCoorTagData[k++] = (detObjInfo[i].bot >> 24) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].bot >> 16) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].bot >> 8) | 0x00;
        roiCoorTagData[k++] = detObjInfo[i].bot | 0x00;

        roiCoorTagData[k++] = (detObjInfo[i].objNameLen >> 24) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].objNameLen >> 16) | 0x00;
        roiCoorTagData[k++] = (detObjInfo[i].objNameLen >> 8) | 0x00;
        roiCoorTagData[k++] = detObjInfo[i].objNameLen | 0x00;

        for (j = 0; j < detObjInfo[i].objNameLen; ++j)
            roiCoorTagData[k++] = detObjInfo[i].objName[j];
    }
}
