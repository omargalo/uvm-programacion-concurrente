#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//mutex permite a un hilo bloquear el acceso a una sección crítica
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* task1(void* arg) {
    pthread_mutex_lock(&lock1);
    // Sección crítica
    printf("Task1 adquiere lock1\n");

    // Simulamos alguna operación prolongada
    sleep(1);

    // Sección crítica
    pthread_mutex_lock(&lock2);
    printf("Task1 adquiere lock2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* task2(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Task2 adquiere lock2\n");

    // Simulamos alguna operación prolongada
    sleep(2); 

    pthread_mutex_lock(&lock1);
    printf("Task2 adquiere lock1\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, task1, NULL);
    pthread_create(&t2, NULL, task2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
