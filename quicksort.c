#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000

int array[ARRAY_SIZE];

int main(){
    for(int i = 0; i< ARRAY_SIZE; i++){
        array[i] = rand() % 10000;
    }

    int middle = ARRAY_SIZE / 2;
    int pivot = array[middle];

    return 0;
}

// void quicksort(int arr[], int low, int high){
//     if (low < high){
//         int pivot = partition(arr, low, high);

//         quicksort(arr, low, pivot - 1);
//         quicksort(arr, pivot + 1, high);
//     }
// }

// int partition(int arr[], int low, int high){
//     int pivot = arr[high];
//     int i = (low - 1);

//     for (int j = low; j <= high - 1; j++){
//         if (arr[j] < pivot){
//             i++;
//             swap(&arr[i], &arr[j]);
//         }
//     }
//     swap(&arr[i + 1], &arr[high]);
//     return (i + 1);
// }