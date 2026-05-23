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
}typedef game_t;

struct node {
    struct node *prev;
    game_t data;
    struct node *next;
}typedef node_t;

game_t NewData(int n){

    n = n % 10;

    game_t newData;
    char names[N][30] = {"Doom", "Portal", "Terraria", "Minecraft", "Among Us", "Limbo", "Hades", "Braid", "Fez", "Inside"};
    char genres[N][30] = {"Shooter", "Puzzle", "Sandbox", "Sandbox", "Social", "Platformer", "Roguelike", "Puzzle", "Platformer", "Adventure"};

    strcpy(newData.name, names[n]);
    strcpy(newData.genre, genres[n]);

    newData.year = 1900 + rand() % (2026-1900+1);
    newData.rating = (rand() % 100) / 10.0;
    
    return newData;
}

node_t *NewNode(game_t data){
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

node_t *AddNode(node_t *head, node_t *new_node){
    node_t *cur = head;
    if (cur == NULL){
        return new_node;
    }
    while (cur->next != NULL){
        cur = cur->next;
    }
    cur->next = new_node;
    return head;
}

node_t *Bind(node_t *k_head, node_t *n_head, int num){
    int cnt = 1;
    node_t *cur = k_head;
    node_t *cur2 = n_head;
    while (cur->next != NULL && cnt != num){
        cnt++;
        cur = cur->next;
    }
    cnt = 1;
    while(cur2->next != NULL && cnt != num){
        cnt++;
        cur2 = cur2->next;
    }
    cur->prev = cur2;
}

void PrintSpisok(node_t *k_head, node_t *n_head, int n){
    int i = 1;
    node_t *cur = k_head;
    node_t *cur2 = n_head;  
    printf("S -> "); 
    printf("%d. %p :%p %s %p", n+i, cur, cur->prev, cur->data.name, cur->next);
    while (cur->next != NULL ){
        i++;
        cur = cur->next;
        printf(" | %d. %p :%p %s %p", n+i, cur, cur->prev, cur->data.name, cur->next);
    }

    i = 1;
    printf("\n%d. %p :%p %s %p", i, cur2, cur2->prev, cur2->data.name, cur2->next);
    while (cur2->next != NULL ){
        i++;
        cur2= cur2->next;
        printf(" | %d. %p :%p %s %p", i, cur2, cur2->prev, cur2->data.name, cur2->next);
    }
    printf("\n");
}

node_t *PrintKey(node_t *node, int k, int *x){
    node_t *cur = node;

    switch (k)
    {
    case 1:
        if (cur->next == NULL){
            *x = 1;
            return cur;
        }else {
            cur = cur->next;
            printf(" | %p :%p %s %s %d %f %p\n", cur, cur->prev, cur->data.name, cur->data.genre, cur->data.year, cur->data.rating, cur->next);
            return cur;
        }
        break;
    
    case 0:
        if (cur->prev == NULL){
            *x = 1;
            return cur;
        }else {
            cur = cur->prev;
            printf(" | %p :%p %s %s %d %f %p\n", cur, cur->prev, cur->data.name, cur->data.genre, cur->data.year, cur->data.rating, cur->next);
            return cur;
        }
        break;
    }
}

void FreeList(node_t *head){
    node_t *cur = head;
    node_t *next;
    node_t *prev;
    if (head == NULL){
        printf("Память очищена.");
    }
    while(cur != NULL){
        next = cur->next;
        prev = cur->prev;
        if (prev != NULL){
            free(prev);
        }
        free(cur);
        cur = next;
    }
    
}

int main(){
    srand(time(NULL));
    int n;
    printf("Введите число узлов в списке: ");
    scanf("%d", &n);
    node_t *node_1n = NewNode(NewData(0));
    node_t *head = node_1n;

    for (int i = 1; i < n; i++){
        AddNode(head, NewNode(NewData(i)));
    }
    int k = n;
    node_t *node_1k = NewNode(NewData(0));
    node_t *head_k = node_1k;
    Bind(head_k, head, 1);
    for (int i = 1; i < k; i++){
        AddNode(head_k, NewNode(NewData(i))); 
        Bind(head_k, head, i+1);
    }
    PrintSpisok(head_k, head, n);
    int user_input = 0;
    node_t *cur = head_k;

    while (1){
        printf("Введите команду: ");
        scanf(" %d", &user_input);
        int x = 0;
        switch (user_input) {
            case 8:
                printf("Нет возможности выполнить данное действие.\n");
                break; 
            case 2:
                cur = PrintKey(cur, 0, &x);
                break; 
            case 4:
                printf("Нет возможности выполнить данное действие.\n");
                break; 
            case 6:
                cur = PrintKey(cur, 1, &x);
                break; 
            default:
                printf("Не та кнопка!");

        }    
        if (x){
            char user_inp2;
            printf("Дальше нельзя! Вернутся в начало? y/n: ");
            scanf(" %c", &user_inp2);
            if (user_inp2 == 'y'){
                cur = head_k;
                printf(" | %p :%p %s %s %d %f %p\n", cur, cur->prev, cur->data.name, cur->data.genre, cur->data.year, cur->data.rating, cur->next);
            }
            else{
                FreeList(head_k);
                return -1;
            }
        }    
    }
}