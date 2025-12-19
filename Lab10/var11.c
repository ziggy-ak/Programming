#include "time.h"

int main(){
    int n;

    printf("Введите количество этапов: ");
    scanf("%d", &n);

    struct Time* stages = NULL;
    struct Time total_time;
    struct Time min_time;

    stages = (struct Time*)malloc(n * sizeof(struct Time));

    if (stages==NULL){
        return 1;
    }

    inputStages(stages, n);
    totalTime(stages, n, &total_time);
    minTime(stages, n, &min_time);

    printf("Общее время: %d часов %d минут\n", total_time.hours, total_time.minutes);
    printf("Мин время: %d часов %d минут\n", min_time.hours, min_time.minutes);
    free(stages);
}

