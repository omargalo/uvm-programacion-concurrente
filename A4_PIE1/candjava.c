#include <stdio.h>
#include <pvm3.h>

int main() {
int mytid, numt, tids[10];
char *argv[] = {"java", "MiProgramaJava", NULL};

// Inicializar PVM
mytid = pvm_mytid();
if (mytid < 0) {
fprintf(stderr, "Error inicializando PVM\n");
return -1;
}

// Crear instancias de la JVM para ejecutar el programa Java
numt = pvm_spawn("java", argv, PvmTaskDefault, "", 3, tids);
if (numt < 1) {
fprintf(stderr, "Error al crear tareas\n");
pvm_exit();
return -1;
}

// Comunicación con las tareas creadas (ejemplo de envío y recepción de mensajes)
for (int i = 0; i < numt; i++) {
pvm_initsend(PvmDataDefault);
pvm_pkstr("Mensaje de inicio");
pvm_send(tids[i], 1);
}

for (int i = 0; i < numt; i++) {
pvm_recv(tids[i], 1);
char buf[100];
pvm_upkstr(buf);
printf("Recibido de tarea %d: %s\n", tids[i], buf);
}

// Finalizar PVM
pvm_exit();
return 0;
}
