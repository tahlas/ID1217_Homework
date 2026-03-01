package Philosophers;

import java.rmi.*;

public interface Table extends Remote {
    void takeForks(int philosopherIndex) throws RemoteException;
    void putDownForks(int philosopherIndex) throws RemoteException;
    
}
