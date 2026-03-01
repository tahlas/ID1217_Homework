package Philosophers;

import java.rmi.*;

public class Server {
    public static void main(String[] args) {
        try {
            TableImpl table = new TableImpl();
            Naming.rebind("table", table);
            System.out.println("Table is ready.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
