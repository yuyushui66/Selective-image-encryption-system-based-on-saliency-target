#include "EncryptionTest.h"

void Decryption7bit(unsigned char* img_data, unsigned char* tmp_data, double* diffusion_ks, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = tmp_data, * key_stream_val = (unsigned char*)diffusion_ks;
    unsigned char pre_ciph_px, tmp;

    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;

    pre_ciph_px = (*img_data_val);   // 解密前将其值保存起来，解密下一个sub-pixel需要使用
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
    for (i = 0; i < img_size; ++i) {
        tmp_data[i] /= 2;
        img_data[i] = img_data[i] & 128;
        tmp_data[i] = tmp_data[i] | img_data[i];
    }
}

void XORDe7bit(unsigned char* img_data, unsigned char* tmp_data, double* diffusion_keystream, int img_size) {
    int i;
    int seed = 128;
    unsigned char* img_data_val = img_data, * key_stream_val = (unsigned char*)diffusion_keystream;


    //处理一下密钥流
    unsigned char* tmp_ks_val = key_stream_val;

    *img_data_val = *key_stream_val ^ *img_data_val;
    ++img_data_val;
    ++key_stream_val;

    for (i = 1; i < img_size; ++i) {

        *img_data_val = *key_stream_val ^ *img_data_val;
        ++img_data_val;
        ++key_stream_val;

    }
    for (i = 0; i < img_size; ++i) {
        tmp_data[i] = tmp_data[i] & 128;
        img_data[i] = img_data[i] & 127;
        img_data[i] = img_data[i] ^ tmp_data[i];
    }
}
