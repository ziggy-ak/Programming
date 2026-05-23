#include <stdio.h>
#include <stdlib.h>

// Скомпилируй и запусти меня

void init(int **arr, int n) { 
    *arr = malloc(n * sizeof(int));
    for (long i = 0; i < n; ++i) { 
        (*arr)[i] = i;    
    }
}

int main() {
    int *arr = NULL;
    int n = 10;
    init(&arr, n);
    for (int i = 0; i < n; ++i) {
        printf("%d\n", arr[i]);
    }
    free(arr);
    return 0;
}