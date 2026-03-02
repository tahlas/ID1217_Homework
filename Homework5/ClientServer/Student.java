package ClientServer;
import java.rmi.*;

public interface Student extends Remote { //define what student object is allowed to do
    void setPartnerIndex(int partnerIndex) throws RemoteException;
    int getIndex() throws RemoteException;
    int getPartnerIndex() throws RemoteException;
}
