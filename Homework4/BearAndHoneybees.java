package ID1217_Homework.Homework4;

public class BearAndHoneybees {
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
        while (portionsOfHoney <  capacity) {
            System.out.println("Bear is waiting for the pot to be filled with honey.");
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        for(int i = 0; i < capacity; i++) {
            portionsOfHoney--;
            System.out.println("Bear is eating honey. Number of portions left: " + portionsOfHoney);
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to eat each portion of honey
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        if (portionsOfHoney == 0) {
            System.out.println("\nBear ate the last portion of honey! Waking up the honeybees to fill the pot.\n");
            notifyAll();
        }

    }

    public synchronized void fillHoney(int id) {
        // TODO: multiple honeybees fill the pot with honey
        while (portionsOfHoney == capacity) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (portionsOfHoney < capacity) {
            portionsOfHoney++;
            System.out.println("Honeybee " + id + " filled the pot with honey. Portions now: " + portionsOfHoney);
            if (portionsOfHoney == capacity) {
                System.out.println("\nThe pot is full! Waking up the bear to eat the honey.\n");
                notifyAll();
            }
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
            pot.fillHoney(id);
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to fill honey
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
