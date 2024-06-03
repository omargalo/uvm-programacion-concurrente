#include <stdio.h>
#include <stdlib.h>

// Función principal del programa
int main(int argc, char *argv[]) {
// Verificar si el número correcto de argumentos ha sido proporcionado
if (argc != 3) {
fprintf(stderr, "Uso: %s <inicio> <fin>\n", argv[0]);
return 1;
}

// Convertir los argumentos de la línea de comandos a enteros
int inicio = atoi(argv[1]);
int fin = atoi(argv[2]);

// Verificar que los argumentos sean positivos
if (inicio < 0 || fin < 0) {
fprintf(stderr, "Error: Los números deben ser positivos\n");
return 1;
}

// Inicializar la suma
int suma = 0;

// Realizar la suma de los números en el rango especificado
for (int i = inicio; i <= fin; i++) {
suma += i;
}

// Mostrar el resultado de la suma
printf("La suma de los números de %d a %d es: %d\n", inicio, fin, suma);

return 0;
}
