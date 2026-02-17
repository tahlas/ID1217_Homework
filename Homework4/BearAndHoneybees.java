package Homework4;

public class BearAndHoneybees {
    /*
     * The solution is not entirely fair because it is not guaranteed that all
     * threads will get a chance to run. Although, because there are only a few
     * threads and each thread sleeps after performing its action, it is likely
     * that all threads will get a chance to run.
     * 
     * However, in a more complex
     * system with many threads, it is possible that some threads may be starved
     * and never get a chance to run.
     */
    public static void main(String[] args) {
        System.out.println("Starting the Bear and Honeybees simulation...");
        int initialPortionsOfHoney = 0;
        int potCapacity = 10;
        int numberOfHoneybees = 3;
        Pot pot = new Pot(initialPortionsOfHoney, potCapacity);
        Bear bear = new Bear(pot);
        Honeybee[] honeybees = new Honeybee[numberOfHoneybees];
        for (int i = 0; i < honeybees.length; i++) {
            honeybees[i] = new Honeybee(pot, i + 1);
        }

        bear.start();
        for (Honeybee honeybee : honeybees) {
            honeybee.start();
        }
    }
}

class Pot {
    int portionsOfHoney;
    int capacity;

    public Pot(int intialPortionsOfHoney, int capacity) {
        this.portionsOfHoney = intialPortionsOfHoney;
        this.capacity = capacity;
    }

    public synchronized void eatHoney() {
        while (portionsOfHoney < capacity) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        for (int i = 0; i < capacity; i++) {
            portionsOfHoney--;
            System.out.println("Bear is eating honey. Number of portions left: " + portionsOfHoney);
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to eat each portion of honey
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("\nBear ate the last portion of honey! Waking up the honeybees to fill the pot.\n");
        notifyAll();
    }

    public synchronized void fillPot(int id) {
        while (portionsOfHoney == capacity) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        portionsOfHoney++;
        System.out.println("Honeybee " + id + " filled the pot with honey. Portions now: " + portionsOfHoney);
        if (portionsOfHoney == capacity) {
            System.out.println("\nThe pot is full! Honeybee " + id + " wakes up the bear to eat the honey.\n");
            notifyAll();
        }

    }
}

class Bear extends Thread {
    Pot pot;

    public Bear(Pot pot) {
        this.pot = pot;
    }

    public void run() {
        while (true) {
            pot.eatHoney();
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to eat honey
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

class Honeybee extends Thread {
    Pot pot;
    int id;

    public Honeybee(Pot pot, int id) {
        this.pot = pot;
        this.id = id;
    }

    public void run() {
        while (true) {
            pot.fillPot(id);
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to fill honey
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
