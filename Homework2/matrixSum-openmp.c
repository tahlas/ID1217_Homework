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

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
  int i, j, total = 0;

  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE)
    size = MAXSIZE;
  if (numWorkers > MAXWORKERS)
    numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++)
  {
    printf("[ ");
    for (j = 0; j < size; j++)
    {
      matrix[i][j] = rand() % 99;
      printf(" %d", matrix[i][j]);
    }
    printf(" ]\n");
  }

  start_time = omp_get_wtime();
#pragma omp parallel for reduction(+ : total) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++)
    {
      total += matrix[i][j];
    }
  // implicit barrier

  int minRowPosition = 0;
  int maxRowPosition = size - 1;
  int minColumnPosition = 0;
  int maxColumnPosition = size - 1;
  int row, column;
#pragma omp parallel for private(row, column)
  for (row = 0; row < size; row++)
  {
    for (column = 0; column < size; column++)
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

  end_time = omp_get_wtime();

  printf("the total is %d\n", total);
  printf("the minimum is %d at column:%d and row: %d)\n", matrix[minRowPosition][minColumnPosition], minColumnPosition, minRowPosition);
  printf("the maximum is %d at column:%d and row: %d)\n", matrix[maxRowPosition][maxColumnPosition], maxColumnPosition, maxRowPosition);
  printf("it took %g seconds\n", end_time - start_time);
}
