#include <stdio.h>
#include<string.h>
#include<iostream>
#include "ReadBMP.h"

#define FILEHEADER_SIZE 14  // bmp �ļ�ͷ�ṹ���С
#define INFOHEADER_SIZE 40

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//int offset, width, height, img_size;   // ͼ����ײ�ƫ����, ���, �߶�, ͼ���С(��ɫͼ�� 3 �� width*height)

// ��������: ��ȡͼ���ƫ����, ���, �߶���Ϣ, ��������Ӧ��ȫ�ֱ���
message img_info(char* file) {
    int offset, width, height, img_size;
    FILE* img_fp;
    // colormap_size �洢��ɫ���С
    int num_bytes, bit_count, colormap_size = 0;

    struct BITMAPFILEHEADER file_header;
    struct BITMAPINFOHEADER info_header;
    
    img_fp = fopen(file, "rb");
    
    // ��ȡ FILEHEADER �ṹ�壬����ļ�ͷ�Ƿ����λͼ�ļ���ʽҪ��
    
    num_bytes = fread(&file_header, 1, FILEHEADER_SIZE, img_fp);
    
    if (num_bytes < FILEHEADER_SIZE) {
        printf("File format error, this program only supports the encryption of BMP format FILES!\n");
        
        fclose(img_fp);
        
    }

    // ��ȡ INFOHEADER �ṹ�壬���� biBitCount �жϵ�ɫ���С
    num_bytes = fread(&info_header, 1, INFOHEADER_SIZE, img_fp);
    if (num_bytes < INFOHEADER_SIZE) {
        printf("File format error, this program only supports the encryption of BMP format FILES!\n");
        
        fclose(img_fp);
        
    }

    // ��ȡ��Ⱥ͸߶�
    width = info_header.biWidth;
    height = info_header.biHeight;

    // ���� biBitCount �����ɫ���С
    bit_count = info_header.biBitCount;
    switch (bit_count) {

    case  1:  colormap_size = 2 * 4;   break;
    case  4:  colormap_size = 16 * 4;  break;
    case  8:  colormap_size = 256 * 4; break;
    case  24: colormap_size = 0;     break;
    }

    // ��ƫ���� offset = FILEHEADER_SIZE + INFOHEADER_SIZE + colormap_size
    offset = FILEHEADER_SIZE + INFOHEADER_SIZE + colormap_size;
    // ͼ���С(���ֽ�Ϊ��λ), malloc �ڴ������
    if (bit_count == 24)
        width *= 3;
    img_size = height * width;

    // �ر��ļ�
    
    fclose(img_fp);
    message t;
    t.height = height;
    t.img_size = img_size;
    t.offset = offset;
    t.width = width;
    return t;
}

void WriteBmpImage(char* filename, char* suffix, unsigned char* imgheader,
    unsigned char* imgdata, int offset, int imgsize) {
    printf("2\n");
    FILE* fp;
    char img_path[100] = { '\0' };
    strncpy(img_path, filename, strlen(filename) - 4);
    //strcat(img_path, filename);
    
    //strcpy(suffix, "_de.bmp");
      
    strcat(img_path, suffix);
    std::cout << suffix;
    //printf("%s", img_path);
    fp = fopen(img_path, "wb");
    fwrite(imgheader, 1, offset, fp);
    fwrite(imgdata, 1, imgsize, fp);
    fclose(fp);

}

void ReadBMPFile(char* img_path, unsigned char* img_header, unsigned char* img_data) {
    // ��ȡ�ļ��ײ�ƫ����, ���, �߶���Ϣ, ��malloc�ڴ�ʹ��

}
