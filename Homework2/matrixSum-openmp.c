/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 8  /* maximum number of workers */

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

typedef struct
{
  int total;
  int minRowPosition;
  int maxRowPosition;
  int minColumnPosition;
  int maxColumnPosition;
} TaskResult;

TaskResult task();

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE)
    size = MAXSIZE;
  if (numWorkers > MAXWORKERS)
    numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  int row, column, total = 0;
  /* initialize the matrix */
  for (row = 0; row < size; row++)
  {
    printf("[ ");
    for (column = 0; column < size; column++)
    {
      matrix[row][column] = rand() % 99;
      printf(" %d", matrix[row][column]);
    }
    printf(" ]\n");
  }

  int minRowPosition = 0;
  int maxRowPosition = 0;
  int minColumnPosition = 0;
  int maxColumnPosition = 0;

  start_time = omp_get_wtime();

  TaskResult result = task();

  end_time = omp_get_wtime();

  total = result.total;
  minRowPosition = result.minRowPosition;
  maxRowPosition = result.maxRowPosition;
  minColumnPosition = result.minColumnPosition;
  maxColumnPosition = result.maxColumnPosition;

  printf("the total is %d\n", total);
  printf("the minimum is %d at column:%d and row: %d)\n", matrix[minRowPosition][minColumnPosition], minColumnPosition, minRowPosition);
  printf("the maximum is %d at column:%d and row: %d)\n", matrix[maxRowPosition][maxColumnPosition], maxColumnPosition, maxRowPosition);
  printf("it took %g seconds\n", end_time - start_time);
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
  #pragma omp parallel for reduction(+ : total) private(row, column)
  for (row = 0; row < size; row++)
    for (column = 0; column < size; column++)
    {
      total += matrix[row][column];

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
    // implicit barrier

    return (TaskResult){total, minRowPosition, maxRowPosition, minColumnPosition, maxColumnPosition};
}
