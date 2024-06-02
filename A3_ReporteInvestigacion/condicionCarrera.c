#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 5; i++) {
        // Sección crítica por que no se sabe cuál hilo llegará primero        
        counter++;
        // aqui se muestra el valor de counter en cada iteración
        printf("Hilo %ld: %d\n", pthread_self(), counter);
    }
    return NULL;
}

int main() {

    printf("Valor del contador inicial: %d\n", counter);
    // Se crean dos hilos
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    printf("Hilo1 %ld: %d\n", pthread_self(), counter);
    pthread_create(&t2, NULL, increment, NULL);    
    printf("Hilo2 %ld: %d\n", pthread_self(), counter);

    // espera a que el hilo t1 termine
    pthread_join(t1, NULL);
    printf("Join Hilo1 %ld: %d\n", pthread_self(), counter);
    // espera a que el hilo t2 termine
    pthread_join(t2, NULL);
    printf("Join Hilo2 %ld: %d\n", pthread_self(), counter);

    printf("Valor del contador final: %d\n", counter);
    return 0;
}
