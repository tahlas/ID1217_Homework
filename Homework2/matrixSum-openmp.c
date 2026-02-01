/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c
     ./matrixSum-openmp size numWorkers

*/  

#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 8  /* maximum number of workers */

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

//helper struct to hold results
typedef struct
{
  int total;
  int minRowPosition;
  int maxRowPosition;
  int minColumnPosition;
  int maxColumnPosition;
} TaskResult;

TaskResult task();

void printMatrix();

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE){
    size = MAXSIZE;
  }
  if (numWorkers > MAXWORKERS){
    numWorkers = MAXWORKERS;
  }

  omp_set_num_threads(numWorkers);

  int row, column;
  srand(time(NULL));

  /* initialize the matrix */
  for (row = 0; row < size; row++)
  {
    for (column = 0; column < size; column++)
    {
      matrix[row][column] = rand() % size;
    }
  }

  #ifdef DEBUG
    printMatrix();
  #endif

  start_time = omp_get_wtime();

  TaskResult r = task();

  end_time = omp_get_wtime();

  printf("the total is %d\n", r.total);
  printf("the maximum is %d at column:%d and row: %d\n", matrix[r.maxRowPosition][r.maxColumnPosition], r.maxColumnPosition, r.maxRowPosition);
  printf("the minimum is %d at column:%d and row: %d\n", matrix[r.minRowPosition][r.minColumnPosition], r.minColumnPosition, r.minRowPosition);
  printf("it took %g seconds\n", end_time - start_time);
}

TaskResult task()
{
  int total = 0;
  int globalMinRowPosition = 0;
  int globalMaxRowPosition = 0;
  int globalMinColumnPosition = 0;
  int globalMaxColumnPosition = 0;

  #pragma omp parallel
  {
    int localSum = 0;
    int localMinRowPosition = 0;
    int localMaxRowPosition = 0;
    int localMinColumnPosition = 0;
    int localMaxColumnPosition = 0;

    //nowait removes barrier at the end of the for loop
    //each thread doesn't need to wait for others to finish before proceeding
    //to the critical section
    #pragma omp for nowait
    for(int row = 0; row < size; row++){
      for(int column = 0; column < size; column++){
        int value = matrix[row][column];
        localSum += value;
        if(value > matrix[localMaxRowPosition][localMaxColumnPosition]){
          localMaxRowPosition = row;
          localMaxColumnPosition = column;
        }
        if(value < matrix[localMinRowPosition][localMinColumnPosition]){
          localMinRowPosition = row;
          localMinColumnPosition = column;
        }
      }
    }
    //critical ensures only one thread at a time can update the shared variables
    //(only one thread can be in this section at a time)
    #pragma omp critical
    {
      total+=localSum;
      if(matrix[localMinRowPosition][localMinColumnPosition] < matrix[globalMinRowPosition][globalMinColumnPosition]){
        globalMinRowPosition = localMinRowPosition;
        globalMinColumnPosition = localMinColumnPosition;
      }
      if(matrix[localMaxRowPosition][localMaxColumnPosition] > matrix[globalMaxRowPosition][globalMaxColumnPosition]){
        globalMaxRowPosition = localMaxRowPosition;
        globalMaxColumnPosition = localMaxColumnPosition;
      }
    }
  }
  return (TaskResult){total, globalMinRowPosition, globalMaxRowPosition, globalMinColumnPosition, globalMaxColumnPosition};
}

void printMatrix(){
  int row, column;
  for (row = 0; row < size; row++)
  {
    printf("[ ");
    for (column = 0; column < size; column++)
    {
      printf(" %d", matrix[row][column]);
    }
    printf(" ]\n");
  }
}