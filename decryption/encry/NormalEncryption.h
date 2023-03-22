#pragma once
#ifndef NORMALENORDE_H_INCLUDED
#define NORMALENORDE_H_INCLUDED
#include "Chaos.h"

void NormalPermutationKSGn(double* permutation_keystream, int img_size, Lorenz* initial_value);
void NormalDiffusionKSGn(double* diffusion_keystream, int img_size, double initial_value);

void NormalEnPermutation(unsigned char* img_data, double* permutation_keystream, int img_size, int* position);      //���ҿ�����ͨ�õ�
void NormalDePermutation(unsigned char* img_data, double* permutation_keystream, int img_size, int* position);      //������Ҳ�ǿ���ͨ�õ�

void EnDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size);                         //��ɢҪ����д
void DeDiffusion(unsigned char* img_data, double* diffusion_keystream, int img_size);                         //done!


void NormalEnDiffusionPermAndCom(unsigned char* img_data, double* diffusion_keystream, int img_size);
void NormalDeDiffusionPermAndCom(unsigned char* img_data, double* diffusion_keystream, int img_size);

#endif // NORMALENORDE_H_INCLUDED
