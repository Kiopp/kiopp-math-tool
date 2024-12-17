#ifndef KIOPP_MATH_H
#define KIOPP_MATH_H

// Calculating functions
double** DD(double* x, double* y, int n);
double** wDD(double* delta, double* f, int n);
double** simplex(double** tableau, int rows, int cols);
double determinant_2x2(double a, double b, double c, double d);

#endif