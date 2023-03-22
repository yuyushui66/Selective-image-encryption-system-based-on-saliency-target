#include <stdio.h>
#include<string.h>
#include<iostream>
#include "ReadBMP.h"

#define FILEHEADER_SIZE 14  // bmp 文件头结构体大小
#define INFOHEADER_SIZE 40

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//int offset, width, height, img_size;   // 图像的首部偏移量, 宽度, 高度, 图像大小(彩色图像 3 倍 width*height)

// 函数功能: 获取图像的偏移量, 宽度, 高度信息, 并赋给相应的全局变量
message img_info(char* file) {
    int offset, width, height, img_size;
    FILE* img_fp;
    // colormap_size 存储调色板大小
    int num_bytes, bit_count, colormap_size = 0;

    struct BITMAPFILEHEADER file_header;
    struct BITMAPINFOHEADER info_header;
    
    img_fp = fopen(file, "rb");
    
    // 读取 FILEHEADER 结构体，检测文件头是否符合位图文件格式要求
    
    num_bytes = fread(&file_header, 1, FILEHEADER_SIZE, img_fp);
    
    if (num_bytes < FILEHEADER_SIZE) {
        printf("File format error, this program only supports the encryption of BMP format FILES!\n");
        
        fclose(img_fp);
        
    }

    // 读取 INFOHEADER 结构体，根据 biBitCount 判断调色板大小
    num_bytes = fread(&info_header, 1, INFOHEADER_SIZE, img_fp);
    if (num_bytes < INFOHEADER_SIZE) {
        printf("File format error, this program only supports the encryption of BMP format FILES!\n");
        
        fclose(img_fp);
        
    }

    // 获取宽度和高度
    width = info_header.biWidth;
    height = info_header.biHeight;

    // 根据 biBitCount 计算调色板大小
    bit_count = info_header.biBitCount;
    switch (bit_count) {

    case  1:  colormap_size = 2 * 4;   break;
    case  4:  colormap_size = 16 * 4;  break;
    case  8:  colormap_size = 256 * 4; break;
    case  24: colormap_size = 0;     break;
    }

    // 总偏移量 offset = FILEHEADER_SIZE + INFOHEADER_SIZE + colormap_size
    offset = FILEHEADER_SIZE + INFOHEADER_SIZE + colormap_size;
    // 图像大小(以字节为单位), malloc 内存的依据
    if (bit_count == 24)
        width *= 3;
    img_size = height * width;

    // 关闭文件
    
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
    // 获取文件首部偏移量, 宽度, 高度信息, 供malloc内存使用

}
