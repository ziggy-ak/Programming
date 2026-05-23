#define GAME_H

#define N 10

struct Game {
    char name[50];
    char genre[50];
    int year;
    float rating;
};

void FillArray(int n, struct Game games[n]);
void PrintArr(int n, struct Game games[n]);
