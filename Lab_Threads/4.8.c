#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

struct ThreadData{
    int nomer_str;
    int str_count;
    int n;
    int *A;
    int *B;
    int *C;
}typedef ThreadData;

void* ProizvedMat(void* arg){
    ThreadData *data = (ThreadData*)arg;
    for (int i = data->nomer_str; i < (data->nomer_str + data->str_count); i++) {          
        for (int j = 0; j < data->n; j++) {     
            data->C[i * data-> n + j] = 0;                    
            for (int k = 0; k < data->n; k++) {   
                data->C[i * data->n + j] += data->A[i * data->n + k] * data->B[k * data->n + j];
            }
        }
    }
    free(data);
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Неверное количество аргументов! Пример: ./4 размер-матрицы колво-потоков\n");
        return -1;
    }

    if (atoi(argv[1]) < atoi(argv[2])){
        printf("Число поток не должно быть больше размерности матрицы! Пример: ./4 размер-матрицы колво-потоков\n");
        return -1;
    }
    int n = atoi(argv[1]);

    pthread_t thread[atoi(argv[2])];
    int i = 0;

    int *A = malloc(n*n * sizeof(int));
    int *B = malloc(n*n * sizeof(int));
    int *C = malloc(n*n * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Ошибка выделения памяти\n");
        return -1;
    }

    for (int i = 0; i < n; i++) {          
        for (int j = 0; j < n; j++) {     
            A[i * n + j] = 1;
            B[i * n + j] = 1;
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i=0; i < atoi(argv[2]); i++){
        ThreadData *data = malloc(sizeof(ThreadData));
        if (data == NULL){
            printf("Ошибка выделения памяти\n");
            return -1;   
        }
        data->str_count = n / atoi(argv[2]);
        data->nomer_str = i * data->str_count;
        data->n = n;
        data->A = A;
        data->B = B;
        data->C = C;
        pthread_create(&thread[i], NULL, &ProizvedMat, data);
    }

    for  (int i=0; i < atoi(argv[2]); i++){
        pthread_join(thread[i], NULL);
    }

    if (n % atoi(argv[2]) != 0){
        ThreadData *data = malloc(sizeof(ThreadData));
        data->nomer_str = n - (n % atoi(argv[2]));
        data->str_count = n % atoi(argv[2]);
        data->n = n;
        data->A = A;
        data->B = B;
        data->C = C;
        ProizvedMat(data);
    }
        gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 +
                    (end.tv_usec - start.tv_usec) / 1000.0;
    printf("N=%d, threads=%d, time=%.3f мс\n", n, atoi(argv[2]), elapsed);

    if (n <= 5){
        for (int i = 0; i < n; i++) {          
            for (int j = 0; j < n; j++) {     
                printf("%d ", A[i * n + j]);
            }
            printf("\n");
        }
        printf("\n    X   \n");
        for (int i = 0; i < n; i++) {          
            for (int j = 0; j < n; j++) {     
                printf("%d ", B[i * n + j]);
            }
            printf("\n");
        }
        printf("\n    ||   \n");
        for (int i = 0; i < n; i++) {          
            for (int j = 0; j < n; j++) {     
                printf("%d ", C[i * n + j]);
            }
            printf("\n");
        }
    }
    free(A);
    free(B);
    free(C);
}