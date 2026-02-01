/* quicksort using OpenMP

   NOTE: This is only used for analysis purposes.
         Please refer to quicksort-openmp.c for the actual implementation.

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o quicksort-openmp quicksort-openmp.c
     ./quicksort-openmp size numberOfWorkers
*/

#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double startTime, endTime;

//#define MAX_ARRAY_SIZE 100000

#define MAX_ARRAY_SIZE 100000000

#define MINIMUM_PARALLEL_SIZE 10000

void swap(int *a, int *b);

void initializeArray(int array[], int size);

void quicksort(int array[], int low, int high);

int partition(int array[], int low, int high);

bool isSorted(int array[], int size);

void printArray(int array[], int size);

void startQuicksort(int array[], int size);

int arraySize;

int numberOfWorkers;

int array[MAX_ARRAY_SIZE];

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int maxNumberOfThreads = omp_get_max_threads();

  arraySize = (argc > 1) ? atoi(argv[1]) : MAX_ARRAY_SIZE;
  numberOfWorkers = (argc > 2) ? atoi(argv[2]) : maxNumberOfThreads;
  if (arraySize > MAX_ARRAY_SIZE) {
    printf("Array size too large, setting to %d\n", MAX_ARRAY_SIZE);
    arraySize = MAX_ARRAY_SIZE;
  }
  if (numberOfWorkers > maxNumberOfThreads) {
    printf("Number of workers too large, setting to %d\n", maxNumberOfThreads);
    numberOfWorkers = maxNumberOfThreads;
  }
  omp_set_num_threads(numberOfWorkers);

  for(int j = 1; j <= 3; j++){
    for(int i = 1; i <= 5; i++){
        initializeArray(array, arraySize*j);
        startTime = omp_get_wtime();
        startQuicksort(array, arraySize*j);
        endTime = omp_get_wtime();
        printf("%g seconds | size %d | %d workers\n", endTime - startTime, arraySize*j, numberOfWorkers);
    }
    printf("\n");
  }
}

void startQuicksort(int array[], int size) {
  #pragma omp parallel
  {
    #pragma omp single
    { 
      quicksort(array, 0, size - 1); 
    }
  }
}

void quicksort(int array[], int low, int high) {
  if (low < high) {
    int pivotIndex = partition(array, low, high);
    //shared(array) is already default
    //shared(array) is added for clarity
    int subArraySize = high - low;
    if(subArraySize > MINIMUM_PARALLEL_SIZE) {
      #pragma omp task shared(array)
      { 
        quicksort(array, low, pivotIndex - 1); 
      }
      #pragma omp task shared(array)
      { 
        quicksort(array, pivotIndex + 1, high); 
      }
      //parent task suspended until children tasks complete
      #pragma omp taskwait 
      } 
    else {
      quicksort(array, low, pivotIndex - 1);
      quicksort(array, pivotIndex + 1, high); 
    }
  }
}

int partition(int array[], int low, int high) {
  int pivot = array[high];
  //boundary for smaller elements
  //elements on the left side of i are smaller than pivot
  //elements on the right side of i are greater than or equal to pivot (or
  //unprocessed)
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    if (array[j] < pivot) {
      i++;
      swap(&array[i], &array[j]); // moves smaller elements to the left side
    }
  }
  // swaps the pivot element to its correct position
  //(just after all smaller elements)
  swap(&array[i + 1], &array[high]);
  // returns the pivot index
  return (i + 1);
}

void initializeArray(int array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i] = rand() % size;
  }
}

void printArray(int array[], int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
  printf("]\n");
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

bool isSorted(int array[], int size) {
  for (int i = 0; i < size - 1; i++) {
    if (array[i] > array[i + 1]) {
      return false;
    }
  }
  return true;
}