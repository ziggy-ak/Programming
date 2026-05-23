#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL){
        printf("Не получилось открыть файл!\n");
        return -1;
    }
    char buff[256];
    int c = 0;
    printf("==========================================\n");
    printf("               Миллиардеры                \n");
    printf("==========================================\n");
    while (fgets(buff, sizeof(buff), file)){
        char *name = strtok(buff, ";");
        char *id = strtok(NULL, ";");
        char *level = strtok(NULL, ";");
        printf("Имя: %s | Номер в Списке Forbs: %s | Состояние: %s", name, id, level);
    }
    printf("\n");
    fclose(file);
}

