#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

double **DSize(int size);

double **sum(double **Matrix1, double **Matrix2, int size);

double **substract(double **Matrix1, double **Matrix2, int size);

double **multiply(double **Matrix1, double **Matrix2, int size);

void printm(double **yu, int size);

#endif
