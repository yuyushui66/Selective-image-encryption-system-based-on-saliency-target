#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "EncryptionProcess.h"

void KeyStreamGenLogistic(double* keystream, int kslen, double* lgVal) {
    int i;
    pre_iter_logistic(lgVal);
    for (i = 0; i < kslen; ++i) {
        Iter_logistic(lgVal);
        *(keystream++) = *lgVal;
    }
}

void KeyStreamGen(double* permutation_keystream, int roi_size, JiaPara* JiaVal) {
    int i;
    pre_iter_JiaSys(JiaVal);
    for (i = 0; i < roi_size / 4; ++i) {
        Iter_JiaSys(JiaVal);
        *(permutation_keystream++) = JiaVal->x;
        *(permutation_keystream++) = JiaVal->y;
        *(permutation_keystream++) = JiaVal->z;
        *(permutation_keystream++) = JiaVal->u;
    }
    switch (roi_size % 4) {
    case 0:
        break;
    case 1:
        Iter_JiaSys(JiaVal);
        *(permutation_keystream++) = JiaVal->x;
        break;
    case 2:
        Iter_JiaSys(JiaVal);
        *(permutation_keystream++) = JiaVal->x;
        *(permutation_keystream++) = JiaVal->y;
        break;
    case 3:
        Iter_JiaSys(JiaVal);
        *(permutation_keystream++) = JiaVal->x;
        *(permutation_keystream++) = JiaVal->y;
        *(permutation_keystream++) = JiaVal->z;
    default:
        break;
    }
    //permutation_keystream -= img_size;
}


void Permutation(unsigned char* img_data, double* permutation_keystream, int roi_size, int* position) {
    int i, tmp_pos;
    for (i = 0; i < roi_size - 1; ++i) {
        permutation_keystream[i] = fabs(permutation_keystream[i]);
        while (permutation_keystream[i] > 1) {
            permutation_keystream[i] /= 10;
        }
        permutation_keystream[i] *= 1.0e+15;
        position[i] = i + (1 + (long long)(permutation_keystream[i]) % ((roi_size - 1) - i));
    }
    position[i] = roi_size - 1;
    for (i = 0; i < roi_size; ++i) {
        tmp_pos = img_data[i];
        img_data[i] = img_data[position[i]];
        img_data[position[i]] = tmp_pos;
    }
}
void DePermutation(unsigned char* img_data, double* permutation_keystream, int img_size, int* position) {
    int i;
    unsigned char temp;
    //    for (i = 0; i < img_size-1; ++i) {
    //        permutation_keystream[i] *= 1.0e+15;
    //        position[i] = i + (1 + (long long)(permutation_keystream[i]) % ((img_size - 1) - i));
    //    }
    //         int i, temp;
    //    for (i = 0; i < img_size - 1; ++i) {
    ////        permutation_keystream[i] *= 1.0e+15;
    ////        position[i] = i + (1 + (long long)(permutation_keystream[i]) % ((img_size - 1) - i));
    //        permutation_keystream[i] = fabs(permutation_keystream[i]);
    //        while (permutation_keystream[i] > 1) {
    //            permutation_keystream[i] /= 10;
    //
    //        }
    //        //printf("%lf\n", permutation_keystream[i]);
    //        permutation_keystream[i] *= 1.0e+15;
    //        position[i] = i + (1 + (long long)(permutation_keystream[i]) % ((img_size - 1) - i));
    //        //printf("%d\n", position[i]);
    //    }
    //    position[i] = img_size - 1;
        // 像素点交换(解密操作为从后至前)
    for (i = img_size - 1; i >= 0; --i) {
        temp = img_data[i];
        img_data[i] = img_data[position[i]];
        img_data[position[i]] = temp;
    }

}


void EnDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = img_data, * key_stream_val = (unsigned char*)diffusion_keystream;      //虽然管用，这种转换会不会有点？？？？

    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;
    for (i = 0; i < img_size; ++i) {
        diffusion_keystream[i] = fabs(diffusion_keystream[i]);
        while (diffusion_keystream[i] > 1)
            diffusion_keystream[i] /= 10;
        diffusion_keystream[i] *= (1.0e+15);
        *tmp_ks_val = (long long)(diffusion_keystream[i]) & 255;
        ++tmp_ks_val;
    }

    // 第一个点由seed参与运算, 特殊处理
    *img_data_val = *key_stream_val ^ ((*img_data_val + *key_stream_val) & 255) ^ seed;
    img_data_val++;
    key_stream_val++;

    for (i = 1; i < img_size; ++i) {
        *img_data_val = *key_stream_val ^ ((*img_data_val + *key_stream_val) & 255) ^ *(img_data_val - 1);
        img_data_val++;
        key_stream_val++;
    }
}

void DeDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = img_data, * key_stream_val = (unsigned char*)diffusion_keystream;
    unsigned char pre_ciph_px, tmp;

    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;
    for (i = 0; i < img_size; ++i) {
        diffusion_keystream[i] = fabs(diffusion_keystream[i]);
        while (diffusion_keystream[i] > 1)
            diffusion_keystream[i] /= 10;
        diffusion_keystream[i] *= (1.0e+15);
        *tmp_ks_val = (long long)(diffusion_keystream[i]) & 255;
        ++tmp_ks_val;
    }
    pre_ciph_px = *img_data_val;   // 解密前将其值保存起来，解密下一个sub-pixel需要使用
    *img_data_val = ((*key_stream_val ^ *img_data_val ^ seed) + 256 - *key_stream_val) & 255;
    ++img_data_val;
    ++key_stream_val;

    for (i = 1; i < img_size; ++i) {
        tmp = *img_data_val;
        *img_data_val = ((*key_stream_val ^ *img_data_val ^ pre_ciph_px) + 256 - *key_stream_val) & 255;
        ++img_data_val;
        ++key_stream_val;
        pre_ciph_px = tmp;
    }
}

void XORDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = img_data, * key_stream_val = (unsigned char*)diffusion_keystream;      //虽然管用，这种转换会不会有点？？？？

    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;
    for (i = 0; i < img_size; ++i) {
        //        diffusion_keystream[i] = fabs(diffusion_keystream[i]);
        //        while (diffusion_keystream[i] > 1)
        //            diffusion_keystream[i] /= 10;
        diffusion_keystream[i] *= (1.0e+15);
        *tmp_ks_val = (long long)(diffusion_keystream[i]) & 255;
        ++tmp_ks_val;
    }

    // 第一个点由seed参与运算, 特殊处理
    *img_data_val = *key_stream_val ^ *img_data_val;
    img_data_val++;
    key_stream_val++;

    for (i = 1; i < img_size; ++i) {
        *img_data_val = *key_stream_val ^ *img_data_val;
        img_data_val++;
        key_stream_val++;
    }
}

void XORDeDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = img_data, * key_stream_val = (unsigned char*)diffusion_keystream;
    unsigned char pre_ciph_px, tmp;

    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;
    for (i = 0; i < img_size; ++i) {
        //        diffusion_keystream[i] = fabs(diffusion_keystream[i]);
        //        while (diffusion_keystream[i] > 1)
        //            diffusion_keystream[i] /= 10;
        //        diffusion_keystream[i] *= (1.0e+15);
        //        *tmp_ks_val = (long long)(diffusion_keystream[i]) & 255;
        //        ++tmp_ks_val;
    }
    pre_ciph_px = *img_data_val;   // 解密前将其值保存起来，解密下一个sub-pixel需要使用
    *img_data_val = *key_stream_val ^ *img_data_val;
    ++img_data_val;
    ++key_stream_val;

    for (i = 1; i < img_size; ++i) {
        tmp = *img_data_val;
        *img_data_val = *key_stream_val ^ *img_data_val;
        ++img_data_val;
        ++key_stream_val;
        pre_ciph_px = tmp;
    }
}


