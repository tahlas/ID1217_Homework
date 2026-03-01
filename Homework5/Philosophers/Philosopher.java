package Philosophers;

import java.rmi.Naming;
import java.util.Random;

public class Philosopher {
    public static void main(String[] args) {
        int philosopherId = Integer.parseInt(args[0]);

        String rmiregURL = "rmi://localhost:1099/table";
        try{
            Table table = (Table) Naming.lookup(rmiregURL);
            Random rand = new Random();
            while (true) {
                

                //Thinking
                System.out.println("Philosopher " + philosopherId + " is thinking.");
                Thread.sleep(rand.nextInt(1000) + 500);

                //Taking forks
                table.takeForks(philosopherId);

                //Eating
                System.out.println("Philosopher " + philosopherId + " is eating.");
                Thread.sleep(rand.nextInt(1000) + 500);

                //Putting down forks
                table.putDownForks(philosopherId);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
