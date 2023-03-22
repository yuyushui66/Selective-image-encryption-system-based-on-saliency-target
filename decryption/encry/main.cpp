#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "ROIInfo.h"
#include "ReadBMP.h"
#include "Chaos.h"
#include "NormalEncryption.h"
#include"EncryptionProcess.h"
#include "RDHhist.h"
#include<thread>
#include<fstream>
#include <io.h>
#include <string>
#include <vector>
#include<iostream>
using namespace std;
//#define ObjNum 3
int ObjNum = 0;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

void getFiles(string path, vector<string>& files) {
    //文件句柄  
    intptr_t hFile = 0;
    //文件信息，声明一个存储文件信息的结构体  
    struct _finddata_t fileinfo;
    string p;  //字符串，存放路径
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
    {
        do
        {
            //如果是目录,迭代之（即文件夹内还有文件夹）  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //文件名不等于"."&&文件名不等于".."
                //.表示当前目录
                //..表示当前目录的父目录
                //判断时，两者都要忽略，不然就无限递归跳不出去了！
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            //如果不是,加入列表  
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        //_findclose函数结束查找
        _findclose(hFile);
    }
}

void do_encry(char *str1,char *str2,char *str3,char *str4)
{
    int ObjNum = 0;
    int i, j, k, objN;
    FILE* img_fp_original, * predictions_coor;
    message t;
    t=img_info(str1);
    int offset, width, height, img_size;
    offset = t.offset;
    width = t.width;
    height = t.height;
    img_size = t.img_size;
    unsigned char* img_header = (unsigned char*)malloc(offset * sizeof(unsigned char));
    unsigned char* img_data = (unsigned char*)malloc(img_size * sizeof(unsigned char));
    //unsigned char img_header[offset];        //这里看着是真的丑，改日有空再改
    //unsigned char img_data[img_size];

    img_fp_original = fopen(str1, "rb");
    
    fread(img_header, 1, offset, img_fp_original);      // 读取明文图像首部, 图像内容加密后首部信息原样写回
    fread(img_data, 1, img_size, img_fp_original);      // 读取图像数据信息
    fclose(img_fp_original);
    
    //这里需要改需要改需要改，应该一行一行的给值
    k = 0;
    roiData** img_data2D = (roiData**)malloc(sizeof(roiData*) * width);
    for (i = 0; i < width; ++i) {
        img_data2D[i] = (roiData*)malloc(sizeof(roiData) * height);
    }
    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            img_data2D[i][j].roiVal = img_data[k++];
            img_data2D[i][j].readFlag = 0;
        }
    }

    //记得free


    FILE* fp_ob;
    fp_ob = fopen(str2, "r");
    
    fscanf(fp_ob, "%d", &ObjNum);
    //printf("%d\n", ObjNum);
    printf("bbbb4");
    fclose(fp_ob);
    

    int RoiPixelNum;
    int CoorTagByteNum;
    //ROI cooroi[ObjNum];
    ROI* cooroi = (ROI*)malloc(ObjNum * sizeof(ROI));
    double* ks_permutation;
    int* pos_permutation;
    double* ks_diffusion;

    unsigned char* imgRoiData = (unsigned char*)malloc(img_size);
    memset(imgRoiData, 0, img_size);
 
    JiaPara* JiaVal1 = (JiaPara*)malloc(sizeof(JiaPara));
    
    FILE* fjiaval;
    fjiaval = fopen(str3, "r");
    fscanf(fjiaval, "%lf%lf%lf%lf", &JiaVal1->x, &JiaVal1->y, &JiaVal1->z, &JiaVal1->u);
    fclose(fjiaval);
    //printf("666\n");
    printf("%.14lf\n%.14lf\n%.14lf\n%.14lf\n", JiaVal1->x, JiaVal1->y, JiaVal1->z, JiaVal1->u);

    CoorTagByteNum = ReadObjInfo(cooroi, ObjNum, str2,t);//解密的时候，在这个地方要获取到框框信息
    printf("%d\n", img_size);

    RoiPixelNum = GetRoiPixelNum(cooroi, ObjNum, img_data2D, imgRoiData, 0);

    double* ks_permutation2 = (double*)malloc(sizeof(double) * RoiPixelNum);
    int* position2 = (int*)malloc(sizeof(int) * RoiPixelNum);
    double* ks_diffusion2 = (double*)malloc(sizeof(double) * RoiPixelNum);
    KeyStreamGen(ks_permutation2, RoiPixelNum, JiaVal1);
    for (i = 0; i < RoiPixelNum - 1; ++i) {
        ks_permutation2[i] = fabs(ks_permutation2[i]);
        while (ks_permutation2[i] > 1) {
            ks_permutation2[i] /= 10;
        }
        ks_permutation2[i] *= 1.0e+15;
        position2[i] = i + (1 + (long long)(ks_permutation2[i]) % ((RoiPixelNum - 1) - i));
    }
    position2[i] = RoiPixelNum - 1;

    KeyStreamGen(ks_diffusion2, RoiPixelNum, JiaVal1);

    DeDiffusion(imgRoiData, ks_diffusion2, RoiPixelNum);//解密，在这个地方解密，框框被嵌入的信息也被解出来了
    DePermutation(imgRoiData, ks_permutation2, RoiPixelNum, position2);//解密
    
//    DeDiffusion(imgRoiData, kss, RoiPixelNum);
    GetRoiPixelNum(cooroi, ObjNum, img_data2D, imgRoiData, 1);

    k = 0;
    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            img_data[k++] = img_data2D[i][j].roiVal;
        }
    }

    printf("1\n");
    char str[10] = "_de.bmp";
    WriteBmpImage(str4, str, img_header, img_data, offset, img_size);//另外一个文件夹

   
    free(imgRoiData);
    for (i = 0; i < width; ++i)
        free(img_data2D[i]);
    free(img_data2D);

    
}
void connect_components(string filename, string Targetpath, int len_bef_photo,string kuang,string key,int i)
{



    string str = filename;
    str = str.erase(0, len_bef_photo);
    string t = str;
    //cout << str << endl;
    //string str = filename.substr(filename.length() - 7,filename.length());
    //printf("%s\n", str);

    //filename.erase(filename.length() - 13);
    //printf("%s\n", filename);
    //filename += "bound\\";
    str.erase(str.length() - 7);

    str += ".txt";
    Targetpath = Targetpath + t;
    kuang += str;
    
    do_encry((char*)filename.c_str(), (char*)kuang.c_str(), (char*)key.c_str(), (char*)Targetpath.c_str());
    cout << endl;
    cout << "Thread" << i << "Finished" << endl;
}
int main()
{
    string FilePath = "D:\\Users\\86152\\Desktop\\jieguo";//自己设置目录 
    string kuang = "D:\\Users\\86152\\Desktop\\final_work\\final_work\\U2Netmaster\\coordinates\\20220524140900";//框框文件夹
    string key = "D:\\Users\\86152\\Desktop\\jiqidaima\\roiencry\\roiencry\\darknet\\build\\darknet\\x64\\jiaval\\jiaval.txt";//密钥
    string Targetpath = "D:\\Users\\86152\\Desktop\\test";
    //string FilePath = argv[1];//源地址 
    //string kuang = argv[2];//框框文件夹
    //string key = argv[3];//密钥   int argc, char* argv[]D:\Users\86152\Desktop\jieguo D:\Users\86152\Desktop\kuang  D:\Users\86152\Desktop\jiqidaima\roiencry\roiencry\darknet\build\darknet\x64\jiaval\jiaval.txt D:\Users\86152\Desktop\test
    //string Targetpath = argv[4];//目标地址
    vector<string> FileName_List;
    //vector<string> kuang_list;
    int len_bef_photo = FilePath.length();

    getFiles(FilePath, FileName_List);

    int th_n = FileName_List.size();
    thread th[10];
    int n = th_n / 10;
    //thread* th = new thread[th_n];
    for (int k = 0; k < n + 1; k++)
    {
        for (int i = 0; i < 10; i++)
        {
            if (i + k * 10 == th_n)break;
            //connect_components(FileName_List[i].c_str(), Targetpath, len_bef_photo, kuang, key,i);
            th[i] = thread(connect_components, FileName_List[i + k * 10].c_str(), Targetpath, len_bef_photo, kuang, key, i + k * 10);
        }
        for (int i = 0; i < 10; i++)
        {
            if (i + k * 10 == th_n)break;
            th[i].join();
        }
    }
    cout << th_n << endl;

    //imshow("src", src);
    //waitKey(0);
    return 0;
}