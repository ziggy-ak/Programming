#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define N 5

struct ThreadData{
    int id;
    char message[256];
}typedef ThreadData;

void deadThread(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    printf("Я умер! :(\n");
    free(data);
}

void* printHello(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    pthread_cleanup_push(deadThread, data);
    for (int i=0; i < N; i++){
        printf("Поток %d: %s\n", data->id, data->message);
        sleep(1);
    }
    pthread_cleanup_pop(1);
}

int main(){
    pthread_t thread[4];
    char *message[] = {"Мир", "Земля", "Планета", "Жизнь"};
    for (int i=0; i < 4; i++){
        ThreadData *data = malloc(sizeof(ThreadData));
        data->id = i+1;
        strcpy(data->message, message[i]);
        pthread_create(&thread[i], NULL, &printHello, data);
    }
    sleep(2);
    for  (int i=0; i < 4; i++){
        pthread_cancel(thread[i]);
    }
    for  (int i=0; i < 4; i++){
        pthread_join(thread[i], NULL);
    }
    for (int i=0; i < N; i++){
        printf("Родитель: Привет Всем!\n");
    }
}