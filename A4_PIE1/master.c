#include <stdio.h>
#include <stdlib.h>
#include <pvm3.h>

// Función principal del programa
int main(int argc, char *argv[]) {
int inicio, fin, suma_total = 0;
int num_workers = 4; // Número de trabajadores
int tids[num_workers]; // IDs de los trabajadores

// Verificar si el número correcto de argumentos ha sido proporcionado
if (argc != 3) {
fprintf(stderr, "Uso: %s <inicio> <fin>\n", argv[0]);
return 1;
}

// Convertir los argumentos de la línea de comandos a enteros
inicio = atoi(argv[1]);
fin = atoi(argv[2]);

// Verificar que los argumentos sean positivos
if (inicio < 0 || fin < 0) {
fprintf(stderr, "Error: Los números deben ser positivos\n");
return 1;
}

// Inicializar PVM y obtener el ID de la tarea máster
int mytid = pvm_mytid();
if (mytid < 0) {
fprintf(stderr, "Error inicializando PVM\n");
return -1;
}

// Crear los trabajadores
int numt = pvm_spawn("worker", NULL, PvmTaskDefault, "", num_workers, tids);
if (numt < num_workers) {
fprintf(stderr, "Error al crear tareas de trabajadores\n");
pvm_exit();
return -1;
}

// Dividir el rango entre los trabajadores
int rango = (fin - inicio + 1) / num_workers;
int extra = (fin - inicio + 1) % num_workers;

// Enviar tareas a los trabajadores
for (int i = 0; i < num_workers; i++) {
int start = inicio + i * rango;
int end = start + rango - 1;
if (i == num_workers - 1) {
end += extra; // Agregar cualquier sobrante al último trabajador
}

// Inicializar el buffer de envío
pvm_initsend(PvmDataDefault);
pvm_pkint(&start, 1, 1);
pvm_pkint(&end, 1, 1);
pvm_send(tids[i], 1); // Enviar datos al trabajador
}

// Recibir resultados de los trabajadores
for (int i = 0; i < num_workers; i++) {
int partial_sum;
pvm_recv(tids[i], 1); // Recibir mensaje del trabajador
pvm_upkint(&partial_sum, 1, 1); // Desempaquetar la suma parcial
suma_total += partial_sum; // Sumar la suma parcial al total
}

// Mostrar el resultado total
printf("La suma de los números de %d a %d es: %d\n", inicio, fin, suma_total);

// Finalizar PVM
pvm_exit();
return 0;
}
