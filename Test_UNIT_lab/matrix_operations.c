#include <stdio.h>
#include <stdlib.h>
#include "matrix_operations.h"

double **DSize(int size) {
	
	double **ResultMatrix;
	int i;
	
	ResultMatrix = (double**)malloc(size * sizeof(double*));
    for (i = 0; i < size; i++) {
        ResultMatrix[i] = (double*)malloc(size*sizeof(double));
    }
    return ResultMatrix;
}
	

double **sum(double **Matrix1, double **Matrix2, int size) {
    
    double **ResultMatrix;
    int i, j, n;
    
	ResultMatrix = DSize(size);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ResultMatrix[i][j] = Matrix1[i][j] + Matrix2[i][j];
        }
    }
    return ResultMatrix;
}




double **substract(double **Matrix1, double **Matrix2, int size) {

    double **ResultMatrix;
    int i, j, n;

    ResultMatrix = DSize(n);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ResultMatrix[i][j] = Matrix1[i][j] - Matrix2[i][j];
        }
    }
    return ResultMatrix;
}




double **multiply(double **Matrix1, double **Matrix2, int size) {

    double **ResultMatrix;
    int i, j, k, n;

    ResultMatrix = DSize(n);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ResultMatrix[i][j] = 0;
            for (k = 0; k < size; k++) {
                ResultMatrix[i][j] += Matrix1[i][k] + Matrix2[k][j];
            }
        }
    }
    return ResultMatrix;
}

void printm(double **yu, int size) {
	
	int i,j,k;
	
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                printf("%0.2lf ", yu[i][j]);
            }
         printf("\n");       
        }
}
    

