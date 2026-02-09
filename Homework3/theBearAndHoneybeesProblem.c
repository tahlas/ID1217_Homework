#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SHARED 1

int numberOfHoneybees = 3; // n = number of honeybees
int pot = 0; // the current amount of honey in the pot
int potCapacity = 10; // H = number of portions of honey the pot can hold

sem_t mutex;
sem_t wakeUpBear;


void *honeybee(void *arg);
void *bear(void *arg);

int main(){
    pthread_t bearId;
    pthread_t honeybeeIds[numberOfHoneybees];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&mutex, SHARED, 1);
    sem_init(&wakeUpBear, SHARED, 0);

    pthread_create(&bearId, &attr, bear, NULL);
    for(int i = 0; i < numberOfHoneybees; i++){
        pthread_create(&honeybeeIds[i], &attr, honeybee, (void *)(long)i);
    }

    pthread_join(bearId, NULL);
    for(int i = 0; i < numberOfHoneybees; i++){
        pthread_join(honeybeeIds[i], NULL);
    }


}

void *honeybee(void *arg){
    long honeybeeId = (long)arg;
    while(true){
        if(pot < potCapacity){
            sem_wait(&mutex); //lock the pot
            pot++;
            printf("Honeybee %ld added honey to the pot. Current honey in the pot: %d\n", honeybeeId, pot);
            if(pot == potCapacity){
                printf("Honeybee %ld is waking up the bear because the pot is full!\n", honeybeeId);
                sem_post(&wakeUpBear); // signal the bear to wake up
            }
            sem_post(&mutex); //unlock the pot
            sleep(1);
        }
    }
}

void *bear(void *arg){
    while(true){
        sem_wait(&wakeUpBear); // wait for the signal from the honeybees that the pot is full
        printf("Bear is waking up to eat the honey.\n");
        sem_wait(&mutex); // lock the pot
        for(int i = 0; i < potCapacity; i++){
            pot--;
            printf("Bear ate a portion of honey. Remaining honey in the pot: %d\n", pot);
            sleep(1);
        }
        sem_post(&mutex); // unlock the pot
        printf("Bear has finished eating the honey and is going back to sleep.\n");
    }
}