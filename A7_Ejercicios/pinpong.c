#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semP, semp;

void* printP(void* arg) {
    while (1) {
        sem_wait(&semP);
        printf("P");
        fflush(stdout);
        sem_post(&semp);
    }
    return NULL;
}

void* printp(void* arg) {
    while (1) {
        sem_wait(&semp);
        printf("p");
        fflush(stdout);
        sem_post(&semP);
    }
    return NULL;
}

int main() {
    pthread_t threadP, threadp;
    sem_init(&semP, 0, 1);
    sem_init(&semp, 0, 0);

    pthread_create(&threadP, NULL, printP, NULL);
    pthread_create(&threadp, NULL, printp, NULL);

    pthread_join(threadP, NULL);
    pthread_join(threadp, NULL);

    sem_destroy(&semP);
    sem_destroy(&semp);

    return 0;
}
