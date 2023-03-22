#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "ROI_Info.h"
#include "ReadBMP.h"
#include "Chaos.h"
#include "NormalEncryption.h"
#include"EncryptionProcess.h"
#include "RDH_hist.h"

#include<thread>
#include<iostream>
#include<fstream>
#include <io.h>
#include <string>
#include <vector>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//�����ʽ��.exe·�� .bmp·�� .txt·�� en(����)/de(����)

//#define ObjNum 22


void getFiles(string path, vector<string>& files) {
    //�ļ����  
    intptr_t hFile = 0;
    //�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
    struct _finddata_t fileinfo;
    string p;  //�ַ��������·��
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
    {
        do
        {
            //�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //�ļ���������"."&&�ļ���������".."
                //.��ʾ��ǰĿ¼
                //..��ʾ��ǰĿ¼�ĸ�Ŀ¼
                //�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            //�������,�����б�  
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        //_findclose������������
        _findclose(hFile);
    }
}


void do_encry(char* str1, char* str2, char* str3, char* str4)
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
    //unsigned char img_header[offset];        //���￴������ĳ󣬸����п��ٸ�
    //unsigned char img_data[img_size];

    img_fp_original = fopen(str1, "rb");
    fread(img_header, 1, offset, img_fp_original);      // ��ȡ����ͼ���ײ�, ͼ�����ݼ��ܺ��ײ���Ϣԭ��д��
    fread(img_data, 1, img_size, img_fp_original);      // ��ȡͼ��������Ϣ
    fclose(img_fp_original);

    //������Ҫ����Ҫ����Ҫ�ģ�Ӧ��һ��һ�еĸ�ֵ
    k = 0;
    printf("%d\t%d\n", height, width);
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

    //�ǵ�free

    FILE* fp_ob;
    fp_ob = fopen(str2, "r");
    fscanf(fp_ob, "%d", &ObjNum);
    //printf("%d\n", ObjNum);
    fclose(fp_ob);


    int RoiPixelNum;
    int CoorTagByteNum;
    //ROI cooroi[ObjNum];
    ROI* cooroi = (ROI*)malloc(ObjNum * sizeof(ROI));
    double* ks_permutation;
    int* pos_permutation;
    double* ks_diffusion;



    //unsigned char imgRoiData[img_size];
    unsigned char* imgRoiData = (unsigned char*)malloc(img_size);
    memset(imgRoiData, 0, img_size);
    //int w = 0;

    JiaPara* JiaVal = (JiaPara*)malloc(sizeof(JiaPara));
    FILE* fjiaval;
    fjiaval = fopen(str3, "r");
    fscanf(fjiaval, "%lf%lf%lf%lf", &JiaVal->x, &JiaVal->y, &JiaVal->z, &JiaVal->u);
    fclose(fjiaval);
    //printf("666\n");
    printf("%.14lf\n%.14lf\n%.14lf\n%.14lf\n", JiaVal->x, JiaVal->y, JiaVal->z, JiaVal->u);


    JiaPara* JiaVal1 = (JiaPara*)malloc(sizeof(JiaPara));
    JiaVal1->x = 6.13455323257449;//9
    JiaVal1->y = -6.76623087823196;//6
    JiaVal1->z = 7.52223762673178;//8
    JiaVal1->u = 6.22045403584687;//7

//for (w = 0; w < 3; ++w) {

//

    CoorTagByteNum = ReadObjInfo(cooroi, ObjNum, str2,t);//////////////////////////
    //printf("%d\n", img_size);
    //printf("666\n");
    RoiPixelNum = GetRoiPixelNum(cooroi, ObjNum, img_data2D, imgRoiData, 0);////////////////////////////////


    ks_permutation = (double*)malloc(sizeof(double) * RoiPixelNum);
    pos_permutation = (int*)malloc(sizeof(int) * RoiPixelNum);
    ks_diffusion = (double*)malloc(sizeof(double) * RoiPixelNum);
    KeyStreamGen(ks_permutation, RoiPixelNum, JiaVal);
    Permutation(imgRoiData, ks_permutation, RoiPixelNum, pos_permutation);
    KeyStreamGen(ks_diffusion, RoiPixelNum, JiaVal);
    EnDiffusion(imgRoiData, ks_diffusion, RoiPixelNum);

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

    GetRoiPixelNum(cooroi, ObjNum, img_data2D, imgRoiData, 1);/////////////////////////

    k = 0;
    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            img_data[k++] = img_data2D[i][j].roiVal;
        }
    }
    char str[10] = "_en.bmp";
    WriteBmpImage(str4, str, img_header, img_data, offset, img_size);
    printf("Picture generated");

    free(ks_permutation);
    free(pos_permutation);
    free(ks_diffusion);
    free(imgRoiData);
    //free(JiaVal);
    for (i = 0; i < width; ++i)
        free(img_data2D[i]);
    free(img_data2D);

}
void connect_components(string filename, string Targetpath, int len_bef_photo, string kuang, string key,int i)
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
    str.erase(str.length() - 3);

    str += "txt";
    Targetpath = Targetpath + t;
    kuang += str;

    do_encry((char*)filename.c_str(), (char*)kuang.c_str(), (char*)key.c_str(), (char*)Targetpath.c_str());
    cout << endl;
    cout << "Thread" << i << "Finished" << endl;
}
int main()
{
    string FilePath = "D:\\Users\\86152\\Desktop\\final_work\\final_work\\U2Netmaster\\choosepic\\20220524140900";//�Լ�����Ŀ¼ 
    string kuang = "D:\\Users\\86152\\Desktop\\final_work\\final_work\\U2Netmaster\\coordinates\\20220524140900";//����ļ���
    string key = "D:\\Users\\86152\\Desktop\\jiqidaima\\roiencry\\roiencry\\darknet\\build\\darknet\\x64\\jiaval\\jiaval.txt";//��Կ
    string Targetpath = "D:\\Users\\86152\\Desktop\\jieguo";//
    //string FilePath = argv[1];//Դ��ַ 
    //string kuang = argv[2];//����ļ���
    //string key = argv[3];//��Կ   int argc, char* argv[]
   // string Targetpath = argv[4];//Ŀ���ַ
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