#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_ITER 1000
#define TOL 1e-6

// Estructura para pasar parámetros a los hilos
typedef struct {
    int id;
    int n;
    double *x_new;
    double *x_old;
    double **A;
    double *b;
} thread_data_t;

// Función que ejecutará cada hilo para actualizar una componente del vector solución
void* jacobi_update(void *arg) {
    thread_data_t *data = (thread_data_t*) arg;
    int i = data->id;
    int n = data->n;
    double sum = data->b[i];
    
    // Calculamos la suma de A[i][j] * x_old[j] para j != i
    for (int j = 0; j < n; j++) {
        if (j != i) {
            sum -= data->A[i][j] * data->x_old[j];
        }
    }
    // Actualizamos la componente i del vector solución
    data->x_new[i] = sum / data->A[i][i];
    
    pthread_exit(NULL);
}

// Función para verificar si la matriz es diagonalmente dominante
int es_diagonalmente_dominante(double **A, int n) {
    for (int i = 0; i < n; i++) {
        double suma = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                suma += fabs(A[i][j]);
            }
        }
        if (fabs(A[i][i]) <= suma) {
            return 0;
        }
    }
    return 1;
}

// Función principal
int main() {
    int n;
    do {
        printf("Ingrese el tamaño del sistema de ecuaciones (mayor a 5): ");
        scanf("%d", &n);
        if (n <= 5) {
            printf("El tamaño del sistema debe ser mayor a 5. Inténtelo de nuevo.\n");
        }
    } while (n <= 5);
    
    double **A, *b, *x_old, *x_new;
    pthread_t *threads;
    thread_data_t *thread_data;

    // Reserva de memoria para la matriz, vectores y estructuras de hilos
    A = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        A[i] = (double*) malloc(n * sizeof(double));
    }
    b = (double*) malloc(n * sizeof(double));
    x_old = (double*) malloc(n * sizeof(double));
    x_new = (double*) malloc(n * sizeof(double));
    threads = (pthread_t*) malloc(n * sizeof(pthread_t));
    thread_data = (thread_data_t*) malloc(n * sizeof(thread_data_t));

    // Inicializamos el sistema de ecuaciones
    // A (matriz nxn) y b (vector de términos independientes)
    printf("Ingrese los elementos de la matriz A:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("A[%d][%d]: ", i, j);
            scanf("%lf", &A[i][j]);
        }
    }

    if (!es_diagonalmente_dominante(A, n)) {
        printf("La matriz no es diagonalmente dominante. El método de Jacobi puede no converger.\n");
        return 1;
    }

    printf("Ingrese los elementos del vector b:\n");
    for (int i = 0; i < n; i++) {
        printf("b[%d]: ", i);
        scanf("%lf", &b[i]);
        x_old[i] = 0.0; // Solución inicial
        x_new[i] = 0.0; // Solución inicial
    }

    // Algoritmo de Jacobi iterativo
    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Creación de hilos para la actualización del vector solución
        for (int i = 0; i < n; i++) {
            thread_data[i].id = i;
            thread_data[i].n = n;
            thread_data[i].x_new = x_new;
            thread_data[i].x_old = x_old;
            thread_data[i].A = A;
            thread_data[i].b = b;
            pthread_create(&threads[i], NULL, jacobi_update, (void*) &thread_data[i]);
        }

        // Espera a que todos los hilos terminen su ejecución
        for (int i = 0; i < n; i++) {
            pthread_join(threads[i], NULL);
        }

        // Verificamos la convergencia comparando x_new y x_old
        double max_diff = 0.0;
        for (int i = 0; i < n; i++) {
            double diff = fabs(x_new[i] - x_old[i]);
            if (diff > max_diff) {
                max_diff = diff;
            }
            x_old[i] = x_new[i];
        }

        // Si la diferencia máxima es menor que la tolerancia, hemos convergido
        if (max_diff < TOL) {
            break;
        }
    }

    // Imprimimos la solución
    printf("Solución aproximada:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %f\n", i, x_new[i]);
    }

    // Liberamos la memoria
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(x_old);
    free(x_new);
    free(threads);
    free(thread_data);

    return 0;
}
