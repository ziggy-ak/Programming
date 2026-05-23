#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"

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