package Philosophers;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class TableImpl extends UnicastRemoteObject implements Table {
    private boolean[] forks = new boolean[5]; // true if fork is available, false if taken

    public TableImpl() throws RemoteException {
        for (int i = 0; i < forks.length; i++) {
            forks[i] = true; // all forks are initially available
        }
    }

    public synchronized void takeForks(int philosopherId){
        int leftFork = philosopherId;
        int rightFork = (philosopherId + 1) % 5;
        while(!forks[leftFork] || !forks[rightFork]){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //take forks
        forks[leftFork] = false;
        forks[rightFork] = false;
        System.out.println("Philosopher " + philosopherId + " takes forks " + leftFork + " and " + rightFork);
    }

    public synchronized void putDownForks(int philosopherId){
        int leftFork = philosopherId;
        int rightFork = (philosopherId + 1) % 5;
        //put down forks
        forks[leftFork] = true;
        forks[rightFork] = true;
        System.out.println("Philosopher " + philosopherId + " puts down forks " + leftFork + " and " + rightFork);
        notifyAll();
    }
}
