/*
    THIS WAS NEVER COMPLETED
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

char userText[256];
char filename[256];

void *Worker(void *arg);

int main()
{
    pthread_attr_t attr;
    pthread_t workerid[3];
    int numWorkers = 3; /* number of workers */
    for (long i = 0; i < numWorkers; i++){
        pthread_create(&workerid[i], &attr, Worker, (void *)i);
    }

    for (long i = 0; i < numWorkers; i++){
        pthread_join(workerid[i], NULL);
    }
    
    printf("Enter filename to write to\n");
    //TODO: change to scanf?
    fgets(filename, sizeof(filename), stdin); 
    FILE *file;
    while (true)
    {
        file = fopen(filename, "a"); // opens file in append mode

        printf("Enter text to write to file\n");

        fgets(userText, sizeof(userText), stdin);
        fprintf(file, "%s", userText); // write to file
        fclose(file);

        printf("%s", userText);
    }

    return 0;
}

void *readInputProducer(void *arg){
    while(fgets(userText, sizeof(userText), stdin) != NULL){
        
    }
}

void *Worker(void *arg)
{
    long myId = (long)arg;

    if (myId == 0)
    {
    }

    if (myId == 1)
    {
    }

    if (myId == 2)
    {
    }

    return NULL;
}
