#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_SMOKERS 3

sem_t tobacco_paper;
sem_t paper_matches;
sem_t matches_tobacco;
sem_t agent;

void* tobacco_smoker(void* arg);
void* paper_smoker(void* arg);
void* matches_smoker(void* arg);
void* tobacco_supplier(void* arg);

int main()
{
    pthread_t smokers[NUM_SMOKERS];
    pthread_t supplier;

    // Initialize semaphores
    sem_init(&tobacco_paper, 0, 0);
    sem_init(&paper_matches, 0, 0);
    sem_init(&matches_tobacco, 0, 0);
    sem_init(&agent, 0, 1);

    // Create threads
    
    pthread_create(&supplier, NULL, tobacco_supplier, NULL);
    pthread_create(&smokers[0], NULL, tobacco_smoker, NULL);
    pthread_create(&smokers[1], NULL, paper_smoker, NULL);
    pthread_create(&smokers[2], NULL, matches_smoker, NULL);

    // Wait for threads to finish
    pthread_join(smokers[0], NULL);
    pthread_join(smokers[1], NULL);
    pthread_join(smokers[2], NULL);
    pthread_join(supplier, NULL);

    // Destroy semaphores
    sem_destroy(&tobacco_paper);
    sem_destroy(&paper_matches);
    sem_destroy(&matches_tobacco);
    sem_destroy(&agent);

    return 0;
}

void* tobacco_smoker(void* arg)
{
    while (1) {
        // Wait for paper and matches to be put on the table
        
		sem_wait(&paper_matches);
//        sem_wait(&matches);
        
        printf("Tobacco smoker picked up paper and matches and is smoking\n");
        sleep(1); // smoking time
        sem_post(&agent); // signal agent that smoking is finished
    }
}

void* paper_smoker(void* arg)
{
    while (1) {
        // Wait for tobacco and matches to be put on the table
        sem_wait(&matches_tobacco);
        //sem_wait(&matches);
        printf("Paper smoker picked up tobacco and matches and is smoking\n");
        sleep(1); // smoking time
        sem_post(&agent); // signal agent that smoking is finished
    }
}

void* matches_smoker(void* arg)
{
    while (1) {
        // Wait for tobacco and paper to be put on the table
        sem_wait(&tobacco_paper);
        //sem_wait(&paper);
        printf("Matches smoker picked up tobacco and paper and is smoking\n");
        sleep(1); // smoking time
        sem_post(&agent); // signal agent that smoking is finished
    }
}

void* tobacco_supplier(void* arg)
{
	
	int choice = 0, x;
    while (1) {
        // Wait for the smokers to finish smoking
        sem_wait(&agent);

        // Randomly choose two ingredients to put on the table
    x = choice;
	while(choice==x){
	
		choice = rand() % 3;
	}
        if (choice == 0) {
            sem_post(&paper_matches);
//            sem_post(&matches);
            printf("\nThread 3 supplier put paper and matches on the table\n");
        } else if (choice == 1) {
            sem_post(&matches_tobacco);
//            sem_post(&matches);
            printf("\nThread 2 supplier put tobacco and matches on the table\n");
        } else {
            sem_post(&tobacco_paper);
//            sem_post(&paper);
            printf("\nThread 1 supplier put tobacco and paper on the table\n");
        }
    }
}
