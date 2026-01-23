/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 10 /* maximum number of workers */

int numWorkers; /* number of workers */

/* timer */
double read_timer()
{
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if (!initialized)
  {
    gettimeofday(&start, NULL);
    initialized = true;
  }
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time;  /* start and end times */
int size, stripSize;          /* assume size is multiple of numWorkers */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */
int globalSum;
int globalMin;
int globalMax;
int globalMinRow;
int globalMaxRow;
int globalMinCol;
int globalMaxCol;

pthread_mutex_t resultLock; /* mutex lock for updating global results */
pthread_mutex_t rowLock;

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE)
    size = MAXSIZE;
  if (numWorkers > MAXWORKERS)
    numWorkers = MAXWORKERS;

  /*matrix is parted into equal sizes between the workers*/
  stripSize = size / numWorkers;

  srand(time(NULL));

  /* initialize the matrix */
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      matrix[i][j] = rand() % 99;
    }
  }

  pthread_mutex_init(&resultLock, NULL);
  pthread_mutex_init(&rowLock, NULL);

  globalSum = 0;
  globalMin = matrix[0][0];
  globalMax = matrix[0][0];
  globalMinRow = 0;
  globalMinCol = 0;
  globalMaxRow = 0;
  globalMaxCol = 0;

  /* print the matrix */
#ifdef DEBUG
  printf("     ");
  for (j = 0; j < size; j++)
    printf("%3d", j);
  printf("\n");

  for (i = 0; i < size; i++)
  {
    printf("%3d [ ", i);

    for (j = 0; j < size; j++)
    {
      if (matrix[i][j] < 10)
        printf(" %d ", matrix[i][j]);
      else
        printf(" %d", matrix[i][j]);
    }
    printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *)l);

  for (l = 0; l < numWorkers; l++)
    pthread_join(workerid[l], NULL);

  end_time = read_timer();

  printf("The total is: %d\n", globalSum);
  printf("The minimum element is: %d at column: %d and row: %d\n", globalMin, globalMinCol, globalMinRow);
  printf("The maximum element is: %d at column: %d and row: %d\n", globalMax, globalMaxCol, globalMaxRow);
  printf("The execution time is: %g sec\n", end_time - start_time);

  return 0;
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
int nextRow = 0;
void *Worker(void *arg)
{
  long myid = (long)arg;
  int total, i, j, first, last;

#ifdef DEBUG
  printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
#endif

  /* determine first and last rows of my strip */
  // first = myid * stripSize;
  // last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

  /* sum values in row and find min/max in rows that this worker will process */
  total = 0;

  int minRow = -1;
  int minCol = -1;
  int maxRow = -1;
  int maxCol = -1;

  int localMin = INT_MAX;
  int localMax = INT_MIN;

  int localMinRow;
  int localMinCol;

  int row;

  while (true)
  {
    // get a task: a row to process
    pthread_mutex_lock(&rowLock);
    row = nextRow;
    nextRow++;
    pthread_mutex_unlock(&rowLock);

    // if the row the worker is trying to process is out of bounds, exit
    // size is the matrix size (number of rows/columns)
    if (row >= size)
    {
      break;
    }

    for (j = 0; j < size; j++)
    {
      int value = matrix[row][j];
      total += value;
      if (value < localMin)
      {
        localMin = value;
        minRow = row;
        minCol = j;
      }
      if (value > localMax)
      {
        localMax = value;
        maxRow = row;
        maxCol = j;
      }
    }
  }
  pthread_mutex_lock(&resultLock);
  globalSum += total;
  if (localMin < globalMin)
  {
    globalMin = localMin;
    globalMinRow = minRow;
    globalMinCol = minCol;
  }

  if (localMax > globalMax)
  {
    globalMax = localMax;
    globalMaxRow = maxRow;
    globalMaxCol = maxCol;
  }
  pthread_mutex_unlock(&resultLock);
  return NULL;
}