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
    char bfType[2];    // �ļ���ʶ��Ϊ��ĸ ASCII �� "BM"
    int bfSize;        // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
    short int bfReserved1;    // λͼ�ļ������֣�����Ϊ 0
    short int bfReserved2;    // λͼ�ļ������֣�����Ϊ 0
    int bfOffbits;            // �ļ���ʼ��λͼ���ݿ�ʼ֮���ƫ�����ֽ�
};    // �ýṹ��ռ�� 14 ���ֽ�

struct BITMAPINFOHEADER {
    int biSize;          // ���ṹ��ռ���ֽ���
    int biWidth;         // λͼ�Ŀ�ȣ�������Ϊ��λ
    int biHeight;        // λͼ�ĸ߶ȣ�������Ϊ��λ
    short biPlanes;      // Ŀ���豸��ƽ����������Ϊ 1
    short biBitCount;    // ÿ�����������λ���������� 1(˫ɫ), 4(16 ɫ)��8(256 ɫ)��24(���ɫ)֮һ
    int biCompression;   // λͼѹ�����ͣ������� 0(��ѹ��),1(BI_RLE8 ѹ������)�� 2(BI_RLE4 ѹ������)֮һ
    int biSizeImage;     // λͼ�Ĵ�С�����ֽ�Ϊ��λ
    int biXPelsPerMeter;   // λͼˮƽ�ֱ��ʣ�ÿ��������
    int biYPelsPerMeter;   // λͼ��ֱ�ֱ��ʣ�ÿ��������
    int biClrUsed;         // λͼʵ��ʹ�õ���ɫ���е���ɫ��
    int biClrImportant;    // λͼ��ʾ��������Ҫ����ɫ��
};    // �ýṹ��ռ�� 40 ���ֽ�



message img_info(char* file);

void ReadBMPFile(char* file, unsigned char* img_header, unsigned char* img_data);
void WriteBmpImage(char* filename, char* suffix, unsigned char* imgheader,
    unsigned char* imgdata, int offset, int imgsize);

//void ReadBMPFile(char *img_path, unsigned char* img_header, unsigned char* img_data);


#endif // READBMP_H_INCLUDED
