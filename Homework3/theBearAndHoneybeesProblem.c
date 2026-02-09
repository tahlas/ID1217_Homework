#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SHARED 1

int numberOfHoneybees = 3; // n = number of honeybees
int pot = 0;               // the current amount of honey in the pot
int potCapacity = 10;      // H = number of portions of honey the pot can hold

sem_t mutex;
sem_t wakeUpBear;
sem_t bearSleeping; // used by the bear to signal the honeybees that it has
                    // finished eating and is going back to sleep

void *honeybee(void *arg);
void *bear(void *arg);

int main() {
    pthread_t bearId;
    pthread_t honeybeeIds[numberOfHoneybees];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&mutex, SHARED, 1);
    sem_init(&wakeUpBear, SHARED, 0);
    sem_init(&bearSleeping, SHARED, 0);

    pthread_create(&bearId, &attr, bear, NULL);
    for (int i = 0; i < numberOfHoneybees; i++) {
        pthread_create(&honeybeeIds[i], &attr, honeybee, (void *)(long)i);
    }

    pthread_join(bearId, NULL);
    for (int i = 0; i < numberOfHoneybees; i++) {
        pthread_join(honeybeeIds[i], NULL);
    }
}

void *honeybee(void *arg) {
    long honeybeeId = (long)arg + 1;
    while (true) {
        sem_wait(&bearSleeping); // wait for the bear to finish eating and go
                                 // back to sleep
        while (true) {
            sem_wait(&mutex); // lock the pot
            if (pot < potCapacity) {
                pot++;
                printf(
                    "Honeybee %ld added honey to the pot. Current honey in the "
                    "pot: "
                    "%d\n",
                    honeybeeId, pot);
                if (pot == potCapacity) {
                    printf(
                        "Honeybee %ld is waking up the bear because the pot is "
                        "full!\n",
                        honeybeeId);
                    sem_post(&wakeUpBear); // signal the bear to wake up
                }
                sem_post(&mutex); // unlock the pot
                sleep(1);
            }
            else{
                sem_post(&mutex); // unlock the pot
                break; // break out of the inner loop to wait for the bear to eat
            }
        }
    }
}

void *bear(void *arg) {
    while (true) {
        for (int i = 0; i < numberOfHoneybees; i++) {
            sem_post(
                &bearSleeping); // signal the honeybees that the bear has
                                // finished eating and is going back to sleep
        }
        sem_wait(&wakeUpBear); // wait for the signal from the honeybees that
                               // the pot is full
        printf("Bear is waking up to eat the honey.\n");
        sem_wait(&mutex); // lock the pot
        for (int i = 0; i < potCapacity; i++) {
            pot--;
            printf(
                "Bear ate portion %d honey. Remaining honey in the pot: %d\n",
                i + 1, pot);
            sleep(1);
        }
        sem_post(&mutex); // unlock the pot
        printf(
            "Bear has finished eating the honey and is going back to sleep.\n");
        sem_post(&bearSleeping); // signal the honeybees that the bear has
                                 // finished eating and is going back to sleep
    }
}