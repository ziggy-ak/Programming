// Лабораторная работа 9. Git.
// Группа ИКС-531
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}


void main() {
    int a[] = {2, 0, 7, 3}; 
    int n = sizeof(a) / sizeof(a[0]);
    
    bubbleSort(a, n);
}