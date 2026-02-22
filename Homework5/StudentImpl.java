import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class StudentImpl extends UnicastRemoteObject implements Student {
    private int index;
    private int partnerIndex = -1;

    public StudentImpl() throws RemoteException {
        super();
    }

    public StudentImpl(int index) throws RemoteException {
        this.index = index;
    }

    public int getIndex() throws RemoteException {
        return index;
    }

    public void setPartnerIndex(int partnerIndex) throws RemoteException {
        this.partnerIndex = partnerIndex;
    }

    public int getPartnerIndex() throws RemoteException {
        return partnerIndex;
    }
    
}
