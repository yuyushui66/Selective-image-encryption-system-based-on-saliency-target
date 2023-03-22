#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Chaos.h"

void Iter_JiaSys(JiaPara* JiaVal) {
    double a = 10;
    double r = 28;
    double b = 8 / 3;
    double d = 0.96;
    double h = 0.0005;

    double K1, L1, M1, N1, K2, L2, M2, N2, K3, L3, M3, N3, K4, L4, M4, N4;

    K1 = -a * (JiaVal->x - JiaVal->y) + JiaVal->u;
    L1 = -JiaVal->x * JiaVal->z + r * JiaVal->x - JiaVal->y;
    M1 = JiaVal->x * JiaVal->y - b * JiaVal->z;
    N1 = -JiaVal->x * JiaVal->z + d * JiaVal->u;

    K2 = -a * ((JiaVal->x + h / 2 * K1) - (JiaVal->y + h / 2 * L1)) + (JiaVal->u + h / 2 * N1);
    L2 = -(JiaVal->x + h / 2 * K1) * (JiaVal->z + h / 2 * M1) + r * (JiaVal->x + h / 2 * K1) - (JiaVal->y + h / 2 * L1);
    M2 = (JiaVal->x + h / 2 * K1) * (JiaVal->y + h / 2 * L1) - b * (JiaVal->z + h / 2 * M1);
    N2 = -(JiaVal->x + h / 2 * K1) * (JiaVal->z + h / 2 * M1) + d * (JiaVal->u + h / 2 * N1);

    K3 = -a * (((JiaVal->x + h / 2 * K2)) - (JiaVal->y + h / 2 * L2)) + (JiaVal->u + h / 2 * N2);
    L3 = -((JiaVal->x + h / 2 * K2)) * (JiaVal->z + h / 2 * M2) + r * ((JiaVal->x + h / 2 * K2)) - (JiaVal->y + h / 2 * L2);
    M3 = ((JiaVal->x + h / 2 * K2)) * (JiaVal->y + h / 2 * L2) - b * (JiaVal->z + h / 2 * M2);
    N3 = -((JiaVal->x + h / 2 * K2)) * (JiaVal->z + h / 2 * M2) + d * (JiaVal->u + h / 2 * N2);

    K4 = -a * ((JiaVal->x + h * K3) - (JiaVal->y + h * L3)) + (JiaVal->u + h * N3);
    L4 = -(JiaVal->x + h * K3) * (JiaVal->z + h * M3) + r * (JiaVal->x + h * K3) - (JiaVal->y + h * L3);
    M4 = ((JiaVal->x + h * K3)) * (JiaVal->y + h * L3) - b * (JiaVal->z + h * M3);
    N4 = -((JiaVal->x + h * K3)) * (JiaVal->z + h * M3) + d * (JiaVal->u + h * N3);

    JiaVal->x += h / 6 * (K1 + 2 * K2 + 2 * K3 + K4);
    JiaVal->y += h / 6 * (L1 + 2 * L2 + 2 * L3 + L4);
    JiaVal->z += h / 6 * (M1 + 2 * M2 + 2 * M3 + M4);
    JiaVal->u += h / 6 * (N1 + 2 * N2 + 2 * N3 + N4);
}

void pre_iter_JiaSys(JiaPara* JiaVal) {
    int i;
    for (i = 0; i < 200; ++i)
        Iter_JiaSys(JiaVal);
}


void Iter_lorenz(Lorenz* lorenz) {
    double h = 0.0005;
    double K1, L1, M1, K2, L2, M2, K3, L3, M3, K4, L4, M4;
    double sigma = 10;
    double rho = 28;
    double beta = 8 / 3;

    K1 = sigma * (lorenz->y - lorenz->x);
    L1 = lorenz->x * (rho - lorenz->z) - lorenz->y;
    M1 = lorenz->x * lorenz->y - beta * lorenz->z;

    K2 = sigma * ((lorenz->y + h / 2 * L1) - (lorenz->x + h / 2 * K1));
    L2 = (lorenz->x + h / 2 * K1) * (rho - (lorenz->z + h / 2 * M1)) - (lorenz->y + h / 2 * L1);
    M2 = (lorenz->x + h / 2 * K1) * (lorenz->y + h / 2 * L1) - beta * (lorenz->z + h / 2 * M1);

    K3 = sigma * ((lorenz->y + h / 2 * L2) - (lorenz->x + h / 2 * K2));
    L3 = (lorenz->x + h / 2 * K2) * (rho - (lorenz->z + h / 2 * M2)) - (lorenz->y + h / 2 * L2);
    M3 = (lorenz->x + h / 2 * K2) * (lorenz->y + h / 2 * L2) - beta * (lorenz->z + h / 2 * M2);

    K4 = sigma * ((lorenz->y + h * L3) - (lorenz->x + h * K3));
    L4 = (lorenz->x + h * K3) * (rho - (lorenz->z + h * M3)) - (lorenz->y + h * L3);
    M4 = (lorenz->x + h * K3) * (lorenz->y + h * L3) - beta * (lorenz->z + h * M3);

    lorenz->x += h / 6 * (K1 + 2 * K2 + 2 * K3 + K4);
    lorenz->y += h / 6 * (L1 + 2 * L2 + 2 * L3 + L4);
    lorenz->z += h / 6 * (M1 + 2 * M2 + 2 * M3 + M4);
}

void pre_iter_lorenz(Lorenz* lorenz) {
    int i;
    for (i = 0; i < 199; ++i)
        Iter_lorenz(lorenz);
}



void Iter_logistic(double* x_logistic) {
    *x_logistic = 4 * (*x_logistic) * (1 - *x_logistic);
    if (*x_logistic == 0.5 || *x_logistic == 0.75)
        *x_logistic += 0.0001;
}

void pre_iter_logistic(double* x) {
    int i;
    double* x_logistic = x;
    for (i = 0; i < 500; ++i) {
        *x_logistic = 4 * (*x_logistic) * (1 - *x_logistic);
        if (*x_logistic == 0.5 || *x_logistic == 0.75)
            *x_logistic += 0.001;
    }
}




