import java.rmi.*;

public interface Teacher extends Remote {
    void registerStudent(Student student) throws RemoteException;
    Student getPartner(Student student) throws RemoteException;
}
