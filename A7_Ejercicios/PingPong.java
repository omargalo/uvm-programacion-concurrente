package A7_Ejercicios;
public class PingPong {
    private final Object lock = new Object();
    private boolean isP = true;

    public static void main(String[] args) {
        PingPong pingPong = new PingPong();
        Thread threadP = new Thread(pingPong::printP);
        Thread threadp = new Thread(pingPong::printp);

        threadP.start();
        threadp.start();
    }

    public void printP() {
        while (true) {
            synchronized (lock) {
                while (!isP) {
                    try {
                        lock.wait();
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }
                System.out.print("P");
                isP = false;
                lock.notify();
            }
        }
    }

    public void printp() {
        while (true) {
            synchronized (lock) {
                while (isP) {
                    try {
                        lock.wait();
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }
                System.out.print("p");
                isP = true;
                lock.notify();
            }
        }
    }
}
