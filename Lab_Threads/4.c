#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define N 5


int numb = 0;
int stop = 0;
pthread_mutex_t mutex;

struct ThreadData{
    int id;
    char message[256];
}typedef ThreadData;

void deadThread(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    printf("Поток %d: Я умер! :(\n", data->id);
    free(data);
}

void* printHello(void* arg) {
    int i = 0;
    ThreadData *data = (ThreadData*)arg;
    pthread_cleanup_push(deadThread, data);

    while(i < N && stop == 0){
        pthread_mutex_lock(&mutex);
        if (numb == 1){
            printf("Поток %d: %s\n", data->id, data->message);
            numb = 0;
            i++;
        }
        pthread_mutex_unlock(&mutex);
        
    }
    pthread_cleanup_pop(1);
}

int main(){
    pthread_t thread[4];
    int i = 0;
    char *message[] = {"Мир", "Земля", "Планета", "Жизнь"};
    for (int i=0; i < 4; i++){
        ThreadData *data = malloc(sizeof(ThreadData));
        data->id = i+1;
        strcpy(data->message, message[i]);
        pthread_create(&thread[i], NULL, &printHello, data);
    }

    while(i < N){
        pthread_mutex_lock(&mutex);
        if (numb == 0){
            printf("Родитель: Привет Всем!\n");
            numb = 1;
            i++;
        }
        pthread_mutex_unlock(&mutex);
        
    }
    // sleep(2);
    stop = 1;
    // for  (int i=0; i < 4; i++){
    //     pthread_cancel(thread[i]);
    // }
    for  (int i=0; i < 4; i++){
        pthread_join(thread[i], NULL);
    }
}