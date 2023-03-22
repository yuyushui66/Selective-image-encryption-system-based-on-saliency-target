#pragma once
#ifndef ENCRYPTIONTEST_H_INCLUDED
#define ENCRYPTIONTEST_H_INCLUDED


void Decryption7bit(unsigned char* img_data, unsigned char* tmp_data, double* diffusion_ks, int imgsize);
void XORDe7bit(unsigned char* img_data, unsigned char* tmp_data, double* diffusion_keystream, int img_size);

#endif // ENCRYPTIONTEST_H_INCLUDED
