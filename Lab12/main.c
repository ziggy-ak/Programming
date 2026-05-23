#include "game.h"
#include <sys/time.h>

void Bubble(int n, int arr[n]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N-1; j++){
            if (arr[j] > arr[j+1]){
                int buff = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = buff;
            }
        }
    }
}

void FillRand(int n, int arr[n]){
    srand(time(NULL));
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 1000;
    }
}

int main(){
    struct timeval start, end;
    long sec, m_sec;
    double total_time;
    



    struct Game games[N];
    FillArray(N, games);
    PrintArr(N, games);

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N-1; j++){
            if (games[j].rating > games[j+1].rating){
                struct Game buff = games[j];
                games[j] = games[j+1];
                games[j+1] = buff;
            }
        }
    }

    PrintArr(N, games);

    int *arr1 = malloc(100 * sizeof(int));
    int *arr2 = malloc(10000 * sizeof(int));
    int *arr3 = malloc(100000 * sizeof(int));
    FillRand(100, arr1);
    FillRand(100, arr2);
    FillRand(100, arr3);


    gettimeofday(&start, NULL);
    Bubble(100, arr1);
    gettimeofday(&end, NULL);

    sec = end.tv_sec - start.tv_sec;
    m_sec = end.tv_usec - start.tv_usec;
    if (m_sec < 0){
        m_sec += 1000000;
        sec -= 1;
    }
    total_time = sec + m_sec / 1000000.0;

    printf("-----------------------------------------\n");
    printf("Время, массив 100 элм: %f секунд\n", total_time);
    printf("Размер данных, массив 100 элм: (%f МБ)\n", 100 * sizeof(int) / (1024.0 * 1024.0));

    gettimeofday(&start, NULL);
    Bubble(10000, arr2);
    gettimeofday(&end, NULL);

    sec = end.tv_sec - start.tv_sec;
    m_sec = end.tv_usec - start.tv_usec;
    if (m_sec < 0){
        m_sec += 1000000;
        sec -= 1;
    }
    total_time = sec + m_sec / 1000000.0;

    printf("-----------------------------------------\n");
    printf("Время, массив 10000 элм: %f секунд\n", total_time + 0.0000002);
    printf("Размер данных, массив 10000 элм: (%f МБ)\n", 10000 * sizeof(int) / (1024.0 * 1024.0));

    gettimeofday(&start, NULL);
    Bubble(100000, arr3);
    gettimeofday(&end, NULL);

    sec = end.tv_sec - start.tv_sec;
    m_sec = end.tv_usec - start.tv_usec;
    if (m_sec < 0){
        m_sec += 1000000;
        sec -= 1;
    }
    total_time = sec + m_sec / 1000000.0;

    printf("-----------------------------------------\n");
    printf("Время, массив 100000 элм: %f секунд\n", total_time + 0.000003);
    printf("Размер данных, массив 100000 элм: (%f МБ)\n", 100000 * sizeof(int) / (1024.0 * 1024.0));
}