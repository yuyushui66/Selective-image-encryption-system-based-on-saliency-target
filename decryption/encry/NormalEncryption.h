#pragma once
#ifndef NORMALENORDE_H_INCLUDED
#define NORMALENORDE_H_INCLUDED
#include "Chaos.h"

void NormalPermutationKSGn(double* permutation_keystream, int img_size, Lorenz* initial_value);
void NormalDiffusionKSGn(double* diffusion_keystream, int img_size, double initial_value);

void NormalEnPermutation(unsigned char* img_data, double* permutation_keystream, int img_size, int* position);      //置乱可以是通用的
void NormalDePermutation(unsigned char* img_data, double* permutation_keystream, int img_size, int* position);      //逆置乱也是可以通用的

void EnDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size);                         //扩散要重新写
void DeDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size);                         //done!


void NormalEnDiffusionPermAndCom(unsigned char* img_data, double* diffusion_keystream, int img_size);
void NormalDeDiffusionPermAndCom(unsigned char* img_data, double* diffusion_keystream, int img_size);

#endif // NORMALENORDE_H_INCLUDED
