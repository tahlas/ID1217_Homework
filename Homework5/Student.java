import java.rmi.*;

public interface Student extends Remote {
    void setPartnerIndex(int partnerIndex) throws RemoteException;
    int getIndex() throws RemoteException;
    int getPartnerIndex() throws RemoteException;
}
