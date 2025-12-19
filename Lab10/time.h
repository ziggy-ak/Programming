#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <stdlib.h>

struct Time{
    int hours;
    int minutes;
};

void inputStages(struct Time* stages, int n);
void totalTime(struct Time* stages, int n, struct Time* result);
void minTime(struct Time* stages, int n, struct Time* result);

#endif
