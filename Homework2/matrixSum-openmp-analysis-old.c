/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp-analysis-old matrixSum-openmp-analysis-old.c
     ./matrixSum-openmp-analysis-old size numWorkers

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

void initializeMatrix(int size);

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

  int row, column, total = 0;
  srand(time(NULL));
  
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 5; j++){
        initializeMatrix(size);
        start_time = omp_get_wtime();
        TaskResult result = task();
        end_time = omp_get_wtime();
        printf("%g seconds | size %d | %d workers\n", end_time - start_time, size, numWorkers);
    }
    size = size * 2;
    printf("\n");
  }
}

TaskResult task()
{
  // runs the loop in parallel
  // use reduction to sum up the total from each thread
  // make row and column private to each thread
  int row, column, total = 0;
  int minRowPosition = 0;
  int maxRowPosition = 0;
  int minColumnPosition = 0;
  int maxColumnPosition = 0;
  //reduction causes each thread to have its own copy of total and 
  //then at the end of the parallel region all the copies are summed 
  //together into a single value
  #pragma omp parallel for reduction(+ : total) private(row, column)
  for (row = 0; row < size; row++)
    for (column = 0; column < size; column++)
    {
      total += matrix[row][column];

      #pragma omp critical
      {
        if (matrix[row][column] < matrix[minRowPosition][minColumnPosition])
        {
          minRowPosition = row;
          minColumnPosition = column;
        }

        if (matrix[row][column] > matrix[maxRowPosition][maxColumnPosition])
        {
          maxRowPosition = row;
          maxColumnPosition = column;
        }
      }
    }
  // implicit barrier

  return (TaskResult){total, minRowPosition, maxRowPosition, minColumnPosition, maxColumnPosition};
}

void printMatrix()
{
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

void initializeMatrix(int size)
{
  int row, column;
  for (row = 0; row < size; row++){
      for (column = 0; column < size; column++){
          matrix[row][column] = rand() % size;
      }
  }
}
