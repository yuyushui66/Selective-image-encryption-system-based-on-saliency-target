#include <stdio.h>
#include <string.h>
#include "ReadBMP.h"
#include "ROI_Info.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

int ReadObjInfo(ROI* detObjInfo, int objNum, char* coorfile,message t) {
    int height = t.height;
    int width = t.width;
    int i, j = 0;
    int RoiInfoByteNum = 0;
    FILE* fp_obj;
    fp_obj = fopen(coorfile, "r");
    fscanf(fp_obj, "%d", &j);
    //printf("%d\n", j);
    //printf("%s\n", coorfile);
    for (i = 0; i < objNum; ++i) {
        detObjInfo[i].objName[10] = '\0';
        fscanf(fp_obj, "%d%d%d%d%s", &detObjInfo[i].left, &detObjInfo[i].top, &detObjInfo[i].width, &detObjInfo[i].height, detObjInfo[i].objName);
        printf("%d\t %d\t %d\t %d\t %s\t\n", detObjInfo[i].left, detObjInfo[i].top, detObjInfo[i].width, detObjInfo[i].height, detObjInfo[i].objName);
        detObjInfo[i].objNameLen = strlen(detObjInfo[i].objName);
        detObjInfo[i].left = (detObjInfo[i].left - 1) * 3 + 1;
        if (detObjInfo[i].left < 0)detObjInfo[i].left = 0;
        detObjInfo[i].width *= 3;
        detObjInfo[i].right = detObjInfo[i].left + detObjInfo[i].width + 2;
        detObjInfo[i].bot = detObjInfo[i].top + detObjInfo[i].height;                //ROI那里标记的bot其实读取图像顺序的“top”
        detObjInfo[i].top = height - detObjInfo[i].bot;                              //ROI那里标记的top其实是读取图像顺序的"bot",来回转的还真麻烦，上下左右傻傻分不清
        if (detObjInfo[i].top <= 0)detObjInfo[i].top = 1;
        
        detObjInfo[i].bot = detObjInfo[i].top + detObjInfo[i].height;
        RoiInfoByteNum += 20;
        RoiInfoByteNum += detObjInfo[i].objNameLen;
        printf("%d\t%d\n", height, width);
        if (detObjInfo[i].top >= height)detObjInfo[i].top = height-1;
        if (detObjInfo[i].bot >= height)detObjInfo[i].bot = height-1;
        if (detObjInfo[i].left >= width)detObjInfo[i].left = width-1;
        if (detObjInfo[i].right >= width)detObjInfo[i].right = width-1;
        printf("%d %d %d %d\n", detObjInfo[i].left, detObjInfo[i].right, detObjInfo[i].bot, detObjInfo[i].top);
    }

    fclose(fp_obj);
    return RoiInfoByteNum;
    {
        //        int i;
        //        unsigned char *img_header, *img_data;               // malloc内存返回值, 指向头部信息区和数据区
        //        FILE *img_fp_original;
        //
        //        img_info(argv[1]);
        //        img_header = (unsigned char*)malloc(offset);        //这里看着是真的丑，改日有空再改
        //        img_data = (unsigned char*)malloc(img_size);
        //        img_fp_original = fopen(argv[1], "rb");
        //        fread(img_header, 1, offset, img_fp_original);      // 读取明文图像首部, 图像内容加密后首部信息原样写回
        //        fread(img_data, 1, img_size, img_fp_original);      // 读取图像数据信息
        //        fclose(img_fp_original);
        //
        //        unsigned char* original = (unsigned char*)malloc(sizeof(unsigned char) * img_size);
        //        for (i = 0; i < img_size; ++i) {
        //            original[i] = img_data[i];
        //        }
        //
        //        double x = 0.13249654876;
        //        int* position = (int*)malloc(sizeof(int) * img_size);
        //        pre_iter_logistic(&x);
        //        double *permutation_ks = (double*)malloc(sizeof(double) * img_size);
        //        for (i = 0; i < img_size; ++i) {
        //            Iter_logistic(&x);
        //            permutation_ks[i] = x;
        //        }
        //        //NormalEnPermutation(img_data, permutation_ks, img_size, position);
        //    //    double *diffusion_ks = (double*)malloc(sizeof(double) * img_size);
        //    //    for (i = 0; i < img_size; ++i) {
        //    //        Iter_logistic(&x);
        //    //        diffusion_ks[i] = x;
        //    //    }
        //        NormalEnDiffusion(img_data, permutation_ks, img_size);
        //    //
        //        //XORDiffusion(img_data, permutation_ks, img_size);
        //        //XORDeDiffusion(img_data, permutation_ks, img_size);
        //        unsigned char* tempdata = (unsigned char*)malloc(sizeof(unsigned char) * img_size);
        //
        //        for (i = 0; i < img_size; ++i) {
        //            tempdata[i] = img_data[i]*2;
        //            permutation_ks[i] *= 2;
        //        }
        //
        //        double *permutation_ks1 = (double*)malloc(sizeof(double) * img_size);
        //        for (i = 0; i < img_size; ++i) {
        //            Iter_logistic(&x);
        //            permutation_ks1[i] = x;
        //        }
        //        NormalEnDiffusion(img_data, permutation_ks1, img_size);
        //        unsigned char* tempdata1 = (unsigned char*)malloc(sizeof(unsigned char) * img_size);
        //
        //        for (i = 0; i < img_size; ++i) {
        //            tempdata1[i] = img_data[i]*2;
        //            permutation_ks1[i] *= 2;
        //        }
        //        //XORDe7bit(img_data, tempdata, permutation_ks, img_size);
        //        //NormalDeDiffusion(img_data, permutation_ks, img_size);
        //        //NormalDePermutation(img_data, permutation_ks, img_size, position);
        //
        //        Decryption7bit(tempdata1, permutation_ks1, img_size);
        //        Decryption7bit(tempdata, permutation_ks, img_size);
        //        for (i = 0; i < img_size; ++i) {
        //            original[i] = original[i] & 128;
        //            tempdata[i] = tempdata[i] &127;
        //            tempdata[i] = tempdata[i] ^ original[i];
        //        }
        //        WriteBmpImage(argv[1], "_cipher.bmp", img_header, tempdata, offset, img_size);
    }
}

int GetRoiPixelNum(ROI* cooroi, int n, roiData** img_Data2D, unsigned char* imgRoiData, int WriteOrRead) {
    int i, j, k, objN;
    k = 0;
    //printf("%d\n", k);
    for (objN = 0; objN < n; ++objN) {
        printf("%d\n", objN);
        for (j = cooroi[objN].top; j <= cooroi[objN].bot; ++j) {
            //printf("\t1\n");
            //printf("%d\n", k);//边界应该都是要的吧
            for (i = cooroi[objN].left; i <= cooroi[objN].right; ++i) {
                //printf("\t2\n");
                //printf("%d\n", k);
                if (WriteOrRead == 0) {
                    //printf("%d\n", k);
                    //printf("\t3\n");
                    if (img_Data2D[i][j].readFlag == 0) {
                        
                        //printf("%d\n", k);
                        //if (k >= img_size)continue;
                        //else printf("%d", k);
                        imgRoiData[k++] = img_Data2D[i][j].roiVal;
                        img_Data2D[i][j].readFlag = 1;
                    }
                }
                else {
                    if (img_Data2D[i][j].readFlag == 1) {
                        //printf("%d\n", k);
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
