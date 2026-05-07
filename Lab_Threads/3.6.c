#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define N 1

struct ThreadData{
    int id;
    int sleep;
}typedef ThreadData;

void deadThread(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    printf("Поток %d: Я умер! :(\n", data->id);
    free(data);
}

void* printHello(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    pthread_cleanup_push(deadThread, data);
    for (int i=0; i < N; i++){
        sleep(data->sleep);
        printf("Поток %d: Привет, я спал %d с.\n", data->id, data->sleep);
    }
    pthread_cleanup_pop(1);
}

void SleepSort(int *sleep){
    pthread_t thread[20];
    for (int i = 0; i < 20; i++){
        ThreadData *data = malloc(sizeof(ThreadData));
        data->id = i+1;
        data->sleep = sleep[i];
        pthread_create(&thread[i], NULL, &printHello, data);
    }
    for  (int i=0; i < 20; i++){
        pthread_join(thread[i], NULL);
    }
}

int main(){
    srand(time(NULL));
    int sleep_count[20];
    for (int i = 0; i < 20; i++){
        sleep_count[i] = (rand() % 15) + 1;
    }
    SleepSort(sleep_count);
    for (int i=0; i < N; i++){
        printf("Родитель: Привет Всем!\n");
    }
}