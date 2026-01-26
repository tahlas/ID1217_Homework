#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_ARRAY_SIZE 10000

int array[MAX_ARRAY_SIZE];

void initializeAttributes(pthread_attr_t *attr);

int partition(int array[], int low, int high);

void initializeArray(int array[], int size);

double read_timer();

void swap(int *a, int *b);

void *worker(void *arg);

//Needed to pass low and high indexes to each thread
typedef struct{
    int low;
    int high;   
} ThreadArguments;

int main(int argc, char *argv[])
{
    int arraySize = (argc > 1) ? atoi(argv[1]) : MAX_ARRAY_SIZE;

    pthread_attr_t attr;
    pthread_t thread;
    
    initializeAttributes(&attr);

    initializeArray(array, arraySize);

    #ifdef DEBUG
    printf("Unsorted array:\n");
    printArray(array, arraySize);
    #endif

    ThreadArguments* args = malloc(sizeof(ThreadArguments));
    args->low = 0;
    args->high = arraySize - 1;
    double start_timer = read_timer();
    pthread_create(&thread, &attr, worker, args);
    pthread_join(thread, NULL);
    free(args);
    double end_timer = read_timer();

    #ifdef DEBUG
    printf("Sorted array:\n");
    printArray(array, arraySize);
    if(isSorted(array, arraySize)){
        printf("Array is sorted.\n");
    } else {
        printf("Array is NOT sorted.\n");
    }
    #endif

    printf("Time taken for sorting: %f seconds\n", end_timer - start_timer);

    return 0;
}

void initializeAttributes(pthread_attr_t *attr)
{
    pthread_attr_init(attr);
    pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
}

void initializeArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 100;
    }
}

void printArray(int array[], int size)
{
    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("]\n");
}

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

void *worker(void *arg)
{
    ThreadArguments *args = (ThreadArguments *)arg;

    int low = args->low; 
    int high = args->high;

    
    if(low < high){
        int pivotIndex = partition(array, low, high);
        pthread_t leftThread;
        pthread_t rightThread;
        ThreadArguments *leftArguments = malloc(sizeof(ThreadArguments));
        ThreadArguments *rightArguments = malloc(sizeof(ThreadArguments));
        leftArguments->low = low;
        leftArguments->high = pivotIndex - 1;
        rightArguments->low = pivotIndex + 1;
        rightArguments->high = high;

        pthread_create(&leftThread, NULL, worker, leftArguments);
        pthread_create(&rightThread, NULL, worker, rightArguments);

        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);
        free(leftArguments);
        free(rightArguments);
    }
    
    return NULL;
}

int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool isSorted(int array[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            return false;
        }
    }
    return true;
}