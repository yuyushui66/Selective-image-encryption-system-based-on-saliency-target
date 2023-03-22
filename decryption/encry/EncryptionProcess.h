#pragma once
#ifndef ENCRYPTIONPROCESS_H_INCLUDED
#define ENCRYPTIONPROCESS_H_INCLUDED

#include "Chaos.h"

void KeyStreamGen(double* permutation_keystream, int img_size, JiaPara* JiaVal);
void KeyStreamGenLogistic(double* keystream, int kslen, double* lgVal);
void Permutation(unsigned char* img_data, double* permutation_keystream, int roi_size, int* position);
void DePermutation(unsigned char* img_data, double* permutation_keystream, int roi_size, int* position);

#endif // ENCRYPTIONPROCESS_H_INCLUDED
