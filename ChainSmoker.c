#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_ITERATIONS 3

int ingredients[3] = {0, 0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tobacco_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t paper_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t matches_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t agent_cond = PTHREAD_COND_INITIALIZER;

// function for agent thread 1
void* agent_thread1(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[0] != 0 || ingredients[1] != 0) {
            pthread_cond_wait(&agent_cond, &mutex);
        }
        ingredients[0] = 1; // tobacco
        ingredients[1] = 1; // paper
        printf("Agent 1 produced tobacco and paper\n");
        pthread_cond_signal(&tobacco_cond);
        pthread_cond_signal(&paper_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// function for agent thread 2
void* agent_thread2(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[1] != 0 || ingredients[2] != 0) {
            pthread_cond_wait(&agent_cond, &mutex);
        }
        ingredients[1] = 1; // paper
        ingredients[2] = 1; // matches
        printf("Agent 2 produced paper and matches\n");
        pthread_cond_signal(&paper_cond);
        pthread_cond_signal(&matches_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// function for agent thread 3
void* agent_thread3(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[0] != 0 || ingredients[2] != 0) {
            pthread_cond_wait(&agent_cond, &mutex);
        }
        ingredients[0] = 1; // tobacco
        ingredients[2] = 1; // matches
        printf("Agent 3 produced tobacco and matches\n");
        pthread_cond_signal(&tobacco_cond);
        pthread_cond_signal(&matches_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// function for smoker with tobacco
void* smoker_tobacco(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[0] != 1) {
            pthread_cond_wait(&tobacco_cond, &mutex);
        }
        ingredients[0] = 0;
        printf("Smoker with tobacco is smoking\n");
        usleep(500);
        pthread_cond_signal(&agent_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// function for smoker with paper
void* smoker_paper(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[1] != 1) {
            pthread_cond_wait(&paper_cond, &mutex);
        }
        ingredients[1] = 0;
        printf("Smoker with paper picks up paper\n");
        sleep(1);
        pthread_cond_signal(&agent_cond);
        pthread_mutex_unlock(&mutex);
        printf("Smoker with paper is smoking\n");
        sleep(1);
    }
    return NULL;
}

// function for smoker with matches
void* smoker_matches(void* arg) {
    int i;
    for (i = 0; i < N_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        while (ingredients[2] != 1) {
            pthread_cond_wait(&matches_cond, &mutex);
        }
        ingredients[2] = 0;
        printf("Smoker with matches picks up matches\n");
        sleep(1);
        pthread_cond_signal(&agent_cond);
        pthread_mutex_unlock(&mutex);
        printf("Smoker with matches is smoking\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t smoker1, smoker2, smoker3, agent1, agent2, agent3;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&tobacco_cond, NULL);
    pthread_cond_init(&paper_cond, NULL);
    pthread_cond_init(&matches_cond, NULL);
    pthread_cond_init(&agent_cond, NULL);
    pthread_create(&smoker1, NULL, smoker_tobacco, NULL);
    pthread_create(&smoker2, NULL, smoker_paper, NULL);
    pthread_create(&smoker3, NULL, smoker_matches, NULL);
    pthread_create(&agent1, NULL, agent_thread1, NULL);
    pthread_create(&agent2, NULL, agent_thread2, NULL);
    pthread_create(&agent3, NULL, agent_thread3, NULL);
    pthread_join(smoker1, NULL);
    pthread_join(smoker2, NULL);
    pthread_join(smoker3, NULL);
    pthread_join(agent1, NULL);
    pthread_join(agent2, NULL);
    pthread_join(agent3, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&tobacco_cond);
    pthread_cond_destroy(&paper_cond);
    pthread_cond_destroy(&matches_cond);
    pthread_cond_destroy(&agent_cond);
    return 0;
}
