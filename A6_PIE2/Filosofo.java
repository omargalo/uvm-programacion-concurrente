import java.util.concurrent.Semaphore;

public class Filosofo implements Runnable {
    private final Semaphore[] tenedores;
    private final int id;

    public Filosofo(int id, Semaphore[] tenedores) {
        this.id = id;
        this.tenedores = tenedores;
    }

    private void pensar() throws InterruptedException {
        System.out.println("Filósofo " + id + " está pensando.");
        Thread.sleep((long) (Math.random() * 1000));
    }

    private void comer() throws InterruptedException {
        // Adquirir los semáforos antes de entrar en la sección crítica
        // Adquiere el semáforo (tenedor) a la izquierda
        tenedores[id].acquire();
        // Adquiere el semáforo (tenedor) a la derecha
        tenedores[(id + 1) % tenedores.length].acquire();

        try {
            // Sección crítica: el filósofo está comiendo
            System.out.println("Filósofo " + id + " está comiendo.");
            Thread.sleep((long) (Math.random() * 1000));
        } finally {
            // Liberar los semáforos después de salir de la sección crítica
            // Libera el semáforo (tenedor) a la izquierda
            tenedores[id].release();
            // Libera el semáforo (tenedor) a la derecha
            tenedores[(id + 1) % tenedores.length].release();
        }
    }

    @Override
    public void run() {
        try {
            while (true) {
                // El filósofo está pensando
                pensar();
                // El filósofo intenta comer
                comer();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public static void main(String[] args) {
        Semaphore[] tenedores = new Semaphore[5];
        for (int i = 0; i < tenedores.length; i++) {
            // Inicializa cada semáforo (tenedor) con un permiso
            tenedores[i] = new Semaphore(1);
        }

        for (int i = 0; i < 5; i++) {
            // Crea y arranca un hilo para cada filósofo
            new Thread(new Filosofo(i, tenedores)).start();
        }
    }
}
