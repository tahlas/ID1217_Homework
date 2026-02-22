import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class TeacherImpl extends UnicastRemoteObject implements Teacher {
    private Student[] students = new Student[100];
    private int studentCount = 0;
    private int totalStudents;

    public TeacherImpl() throws RemoteException {
        super();
        this.totalStudents = totalStudents;
    }

    public synchronized void registerStudent(Student student) throws RemoteException {
        if (studentCount < students.length) {
            students[studentCount] = student;
            studentCount++;

            if(studentCount % 2 == 0){
                Student student1 = students[studentCount - 2];
                Student student2 = students[studentCount - 1];
                student1.setPartnerIndex(student2.getIndex());
                student2.setPartnerIndex(student1.getIndex());
            }
        }
    }

    public synchronized Student getPartner(Student student) throws RemoteException {
        for (int i = 0; i < studentCount; i++) {
            if (students[i].equals(student)) {
                if (i % 2 == 0 && i + 1 < studentCount) {
                    return students[i + 1];
                } else if (i % 2 == 1) {
                    return students[i - 1];
                } 
            }
        }
        return null; // No partner found
    }
}
