#pragma once
#ifndef CHAOS_H_INCLUDED
#define CHAOS_H_INCLUDED

struct LorenzPra {
    double x;
    double y;
    double z;
};

typedef struct {
    double x;
    double y;
    double z;
    double u;
} JiaPara;


typedef struct LorenzPra Lorenz;

void pre_iter_lorenz(Lorenz* lorenz);           //Ĭ��Ԥ����200��
void Iter_lorenz(Lorenz* lorenz);

void pre_iter_logistic(double* x_logistic);     //Ĭ��Ԥ����200��
void Iter_logistic(double* x_logistic);

void pre_iter_JiaSys(JiaPara* JiaVal);
void Iter_JiaSys(JiaPara* JiaVal);



#endif // CHAOS_H_INCLUDED
