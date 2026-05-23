#include "game.h"

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