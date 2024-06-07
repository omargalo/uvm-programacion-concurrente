import java.util.concurrent.locks.ReentrantLock;

public class SeccionCritica {

    // Recurso compartido
    private int recursoCompartido = 0;

    // Lock para asegurar la exclusión mutua
    private final ReentrantLock lock = new ReentrantLock();

    public void incrementarRecurso() {
        // Adquirir el lock antes de entrar en la sección crítica
        lock.lock();
        try {
            // Sección crítica: acceso y modificación del recurso compartido
            recursoCompartido++;
            System.out.println(Thread.currentThread().getName() + " incrementó el recurso a: " + recursoCompartido);
        } finally {
            // Liberar el lock después de salir de la sección crítica
            lock.unlock();
        }
    }

    public static void main(String[] args) {
        SeccionCritica ejemplo = new SeccionCritica();

        // Crear múltiples hilos que acceden al recurso compartido
        Runnable tarea = ejemplo::incrementarRecurso;

        // Crear y arrancar 10 hilos
        for (int i = 0; i < 10; i++) {
            new Thread(tarea, "Hilo-" + i).start();
        }
    }
}
