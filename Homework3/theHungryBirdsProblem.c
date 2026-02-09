#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SHARED 1

//mutex to protect access to the dish
sem_t mutex;
//used by the baby birds to wake up the parent bird when the dish is empty
sem_t parentSleep; 

sem_t wormsAvailable; 

int dish = 0;                  // w = number of worms in the dish
int numberOfBabyBirds = 4;     // n = number of baby birds
int maximumNumberOfWorms = 10; // maximum number of worms the dish can hold

void *parentBird(void *);
void *babyBird(void *);

int main() {
    pthread_t parentBirdId;
    pthread_t babyBirdIds[numberOfBabyBirds];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&mutex, SHARED, 1);
    sem_init(&parentSleep, SHARED, 0);
    sem_init(&wormsAvailable, SHARED, 0);

    pthread_create(&parentBirdId, &attr, parentBird, NULL);
    for (int i = 0; i < numberOfBabyBirds; i++) {
        pthread_create(&babyBirdIds[i], &attr, babyBird, (void *)(long)i);
    }

    sem_post(&parentSleep); // Intial chirp to wake up the parent bird to fill the dish for the first time

    pthread_join(parentBirdId, NULL);
    for (int i = 0; i < numberOfBabyBirds; i++) {
        pthread_join(babyBirdIds[i], NULL);
    }
}

void *parentBird(void *arg) {
    while (true) {
        sem_wait(&parentSleep); // wait for the chirp from the baby birds
        printf("Parent bird is waking up to fill the dish.\n");
        sem_wait(&mutex); // lock the dish
        for (int i = 0; i < maximumNumberOfWorms; i++) {
            dish++;
            printf("Parent bird added a worm. Total worms in dish: %d\n", dish);
            sem_post(&wormsAvailable); // signal that a worm is available
            sleep(1);
        }
        printf("Parent bird has filled the dish with worms and is going back "
               "to sleep.\n");
        sem_post(&mutex); // unlock the dish
    }
}

void *babyBird(void *arg) {
    long myId = (long)arg;
    while (true) {
        sem_wait(&wormsAvailable); // wait for a worm to be available
        sem_wait(&mutex);          // lock the dish
        if (dish > 0) {
            dish--; // eat a worm
            printf("Baby bird %ld ate a worm. Total worms left in the dish: %d\n", myId, dish);
            if (dish == 0) {
                printf("Baby bird %ld is chirping to wake up the parent bird "
                       "because the dish is empty!\n", myId);
                sem_post(&parentSleep); // Chirp (signal) the parent bird to wake up
            }
        }
        sem_post(&mutex); // unlock the dish
        sleep(1);
    }
}