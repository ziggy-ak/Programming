#include "time.h"

void inputStages(struct Time* stages, int n){
    printf("Введите время каждого этапа:\n");

    for(int i = 0; i < n; i++){
        printf("Этап %d:\n", i+1);
        
        printf("Часы: ");
        scanf("%d", &stages[i].hours);

        printf("Минуты: ");
        scanf("%d", &stages[i].minutes);

        if (stages[i].minutes >= 60){
            stages[i].hours += stages[i].minutes / 60;
            stages[i].minutes = stages[i].minutes % 60;
        }
    }
}


void totalTime(struct Time* stages, int n, struct Time* result){
    int total_h = 0;
    int total_m = 0;

    for (int i = 0; i < n; i++){
        total_h += stages[i].hours;
        total_m += stages[i].minutes;
    }

    result->hours = total_h + total_m / 60;
    result->minutes = total_m % 60;
}


void minTime(struct Time* stages, int n, struct Time* result){
    int min_Time = stages[0].hours * 60 + stages[0].minutes;

    for (int i = 0; i < n; i++){
        int time = stages[i].hours * 60 + stages[i].minutes;

        if (time < min_Time){
            min_Time = time;
        }
    }

    result->hours = min_Time / 60;
    result->minutes = min_Time % 60;
}