#ifndef READBMP_H_INCLUDED
#define READBMP_H_INCLUDED

#include <stdio.h>
//extern int offset, width, height, img_size;
typedef struct message
{
    int offset;
    int width;
    int height;
    int img_size;
}message;
struct BITMAPFILEHEADER {
    char bfType[2];    // 文件标识，为字母 ASCII 码 "BM"
    int bfSize;        // 位图文件的大小，以字节为单位
    short int bfReserved1;    // 位图文件保留字，必须为 0
    short int bfReserved2;    // 位图文件保留字，必须为 0
    int bfOffbits;            // 文件开始到位图数据开始之间的偏移量字节
};    // 该结构体占据 14 个字节

struct BITMAPINFOHEADER {
    int biSize;          // 本结构所占用字节数
    int biWidth;         // 位图的宽度，以像素为单位
    int biHeight;        // 位图的高度，以像素为单位
    short biPlanes;      // 目标设备的平面数，必须为 1
    short biBitCount;    // 每个像素所需的位数，必须是 1(双色), 4(16 色)，8(256 色)或24(真彩色)之一
    int biCompression;   // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8 压缩类型)或 2(BI_RLE4 压缩类型)之一
    int biSizeImage;     // 位图的大小，以字节为单位
    int biXPelsPerMeter;   // 位图水平分辨率，每米像素数
    int biYPelsPerMeter;   // 位图垂直分辨率，每米像素数
    int biClrUsed;         // 位图实际使用的颜色表中的颜色数
    int biClrImportant;    // 位图显示过程中重要的颜色数
};    // 该结构体占据 40 个字节



message img_info(char* file);

void ReadBMPFile(char* file, unsigned char* img_header, unsigned char* img_data);
void WriteBmpImage(char* filename, char* suffix, unsigned char* imgheader,
    unsigned char* imgdata, int offset, int imgsize);

//void ReadBMPFile(char *img_path, unsigned char* img_header, unsigned char* img_data);


#endif // READBMP_H_INCLUDED
