import java.util.concurrent.locks.ReentrantLock;

public class Deadlock {

    // Dos locks para simular el deadlock
    private final ReentrantLock lock1 = new ReentrantLock();
    private final ReentrantLock lock2 = new ReentrantLock();

    // Método que simula el deadlock
    public void metodoA() {
        lock1.lock();
        try {
            // Simulación de trabajo
            Thread.sleep(50);
            lock2.lock();
            try {
                System.out.println(Thread.currentThread().getName() + " ejecutando metodoA");
            } finally {
                lock2.unlock();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            lock1.unlock();
        }
    }

    // Método que simula el deadlock
    public void metodoB() {
        lock2.lock();
        try {
            // Simulación de trabajo
            Thread.sleep(50);
            lock1.lock();
            try {
                System.out.println(Thread.currentThread().getName() + " ejecutando metodoB");
            } finally {
                lock1.unlock();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            lock2.unlock();
        }
    }

    public static void main(String[] args) {
        Deadlock ejemplo = new Deadlock();

        // Crear dos hilos que provocarán un deadlock
        Thread hilo1 = new Thread(ejemplo::metodoA, "Hilo-1");
        Thread hilo2 = new Thread(ejemplo::metodoB, "Hilo-2");

        // Arrancar los hilos
        hilo1.start();
        hilo2.start();

        // Inanición (starvation) se puede simular creando hilos de alta prioridad
        Thread hiloPrioritario = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                ejemplo.incrementarRecurso();
            }
        }, "Hilo-Prioritario");
        hiloPrioritario.setPriority(Thread.MAX_PRIORITY);

        Thread hiloBajaPrioridad = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                ejemplo.incrementarRecurso();
            }
        }, "Hilo-BajaPrioridad");
        hiloBajaPrioridad.setPriority(Thread.MIN_PRIORITY);

        hiloPrioritario.start();
        hiloBajaPrioridad.start();
    }

    // Recurso compartido para simular inanición
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
}
