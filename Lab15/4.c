#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Milliarders {
    char name[100];
    char id[20];
    char level[100];
}typedef Milliarder;

void Search(int c, char *ptr){
    FILE *file;
    file = fopen(ptr, "rb");
    if (file == NULL){
        printf("Ошибка\n");
        return;
    }
    int L = 0;
    int R = c-1;
    int id;
    Milliarder buff[100];
    fread(buff, sizeof(Milliarder), c, file);
    fclose(file);
    printf("Введите ID: ");
    scanf("%d", &id);
    while (L < R){
        int m = (L + R)/2;
        if (atoi(buff[m].id) < id){
            L = m + 1;
        }
        else{
            R = m;
        }
    }
    if (atoi(buff[L].id) == id){
        printf("Миллиардер Найден!\n");
        printf("Имя: %s | Номер в Списке Forbs: %s | Состояние: %s\n", buff[L].name, buff[L].id, buff[L].level);
    }else{
        printf("Ошибка\n");
    }

}
int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Неверное количество аргументов!\n");
        return -1;
    }

    FILE *file;
    file = fopen(argv[1], "r+");
    FILE *file1;
    file1 = fopen(argv[2], "w+");
    if (file == NULL){
        printf("Не получилось открыть файл!\n");
        return -1;
    }
    if (file1 == NULL){
        printf("Не получилось открыть файл!\n");
        return -1;
    } 
    char buff[256];
    int c = 0;
    Milliarder arr[100];
    while (fgets(buff, sizeof(buff), file)){
        char *name = strtok(buff, ";");
        char *id = strtok(NULL, ";");
        char *level = strtok(NULL, ";");
        strcpy(arr[c].name, name);
        strcpy(arr[c].id, id);
        strcpy(arr[c].level, level);
        c++;
    }
    fwrite(arr, sizeof(Milliarder), c, file1);
    fflush(file1);
    printf("\n");
    Search(c, argv[2]);
    fclose(file);
}