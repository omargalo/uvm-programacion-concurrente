import java.util.concurrent.Semaphore;

public class LectoresEscritores {

    // Semáforo para la exclusión mutua del recurso de escritura
    private final Semaphore wrt = new Semaphore(1);

    // Semáforo para la exclusión mutua del contador de lectores
    private final Semaphore mutex = new Semaphore(1);

    // Contador de lectores
    private int readCount = 0;

    // Recurso compartido (simulado)
    private int recursoCompartido = 0;

    // Clase interna para el lector
    class Lector implements Runnable {
        @Override
        public void run() {
            try {
                // Inicio de la transacción para lectura
                mutex.acquire();
                readCount++;
                if (readCount == 1) {
                    // El primer lector bloquea la escritura
                    wrt.acquire();
                }
                mutex.release();

                // Sección crítica: leer el recurso compartido
                System.out.println(Thread.currentThread().getName() + " está leyendo el recurso: " + recursoCompartido);

                // Fin de la transacción para lectura
                mutex.acquire();
                readCount--;
                if (readCount == 0) {
                    // El último lector libera la escritura
                    wrt.release();
                }
                mutex.release();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    // Clase interna para el escritor
    class Escritor implements Runnable {
        @Override
        public void run() {
            try {
                // Adquirir el semáforo de escritura antes de entrar en la sección crítica
                wrt.acquire();

                // Sección crítica: escribir en el recurso compartido
                recursoCompartido++;
                System.out.println(Thread.currentThread().getName() + " está escribiendo el recurso: " + recursoCompartido);

                // Liberar el semáforo de escritura después de salir de la sección crítica
                wrt.release();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    public static void main(String[] args) {
        LectoresEscritores ejemplo = new LectoresEscritores();

        // Crear y arrancar hilos lectores y escritores
        for (int i = 0; i < 5; i++) {
            new Thread(ejemplo.new Lector(), "Lector-" + i).start();
            new Thread(ejemplo.new Escritor(), "Escritor-" + i).start();
        }
    }
}
