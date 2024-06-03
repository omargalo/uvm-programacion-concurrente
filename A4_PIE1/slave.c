#include <stdio.h>
#include <pvm3.h>

// Función principal del programa
int main() {
int start, end, sum = 0;

// Recibir los datos del máster
pvm_recv(-1, 1); // Recibir mensaje del máster con etiqueta 1
pvm_upkint(&start, 1, 1); // Desempaquetar el inicio del rango
pvm_upkint(&end, 1, 1);   // Desempaquetar el fin del rango

// Realizar la suma en el rango especificado
for (int i = start; i <= end; i++) {
sum += i;
}

// Enviar el resultado de vuelta al máster
pvm_initsend(PvmDataDefault); // Inicializar el buffer de envío
pvm_pkint(&sum, 1, 1);        // Empaquetar la suma parcial
pvm_send(pvm_parent(), 1);    // Enviar el mensaje al máster con etiqueta 1

// Finalizar PVM
pvm_exit();
return 0;
}
