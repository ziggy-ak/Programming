#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define N 10

struct Game{
    char name[50];
    char genre[50];
    int year;
    float rating;
};

void FillArray(int n, struct Game games[n]){

    char names[N][30] = {"Doom", "Portal", "Terraria", "Minecraft", "Among Us", "Limbo", "Hades", "Braid", "Fez", "Inside"};
    char genres[N][30] = {"Shooter", "Puzzle", "Sandbox", "Sandbox", "Social", "Platformer", "Roguelike", "Puzzle", "Platformer", "Adventure"};
    
    srand(time(NULL));
    for (int i = 0; i < N; i++){
        strcpy(games[i].name, names[i]);
        strcpy(games[i].genre, genres[i]);

        games[i].year = 1900 + rand() % (2026-1900+1);
        games[i].rating = (rand() % 100) / 10.0;
    }


}

void PrintArr(int n, struct Game games[n]){
    printf("     СПИСОК ИГР:     \n");
    printf("№ Название; Жанр; Год; Рейтинг\n");
    for (int i = 0; i < N; i++){
        printf("%d. %s; %s; %d; %.1f\n", i+1, games[i].name, games[i].genre, games[i].year, games[i].rating);
    }
}

int main(){
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
}



