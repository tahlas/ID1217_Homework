package ClientServer;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class TeacherImpl extends UnicastRemoteObject implements Teacher {
    private Student[] students = new Student[100];
    private int studentCount = 0;
    private int totalStudents;

    private boolean paired = false;

    public TeacherImpl(int totalStudents) throws RemoteException {
        super();
        this.totalStudents = totalStudents;
    }

    public synchronized void registerStudent(Student student) throws RemoteException {
        if (studentCount < students.length) {
            students[studentCount] = student;
            studentCount++;

            if(studentCount == totalStudents){
                pairStudents();
                paired = true;
                notifyAll(); 
            }
            // if(studentCount % 2 == 0){
            //     Student student1 = students[studentCount - 2];
            //     Student student2 = students[studentCount - 1];
            //     student1.setPartnerIndex(student2.getIndex());
            //     student2.setPartnerIndex(student1.getIndex());
            // }
        }
    }

    private void pairStudents() throws RemoteException {

        boolean isOddNumberOfStudents = (studentCount % 2 != 0);
        if(isOddNumberOfStudents){
            students[studentCount - 1].setPartnerIndex(students[studentCount - 1].getIndex()); // If there's an odd number of students, the last student is paired with themselves
        }

        int lastIndex = studentCount;
        if(isOddNumberOfStudents){
            lastIndex--;
        }
        for (int i = 0; i < lastIndex; i += 2) {
            if (i + 1 < lastIndex) {
                Student student1 = students[i];
                Student student2 = students[i + 1];
                student1.setPartnerIndex(student2.getIndex());
                student2.setPartnerIndex(student1.getIndex());
            }
        }
    }

    public synchronized Student getPartner(Student student) throws RemoteException {// Wait until all students are paired before returning the partner
        while(!paired){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < studentCount; i++) {// Find the student in the array and return their partner
            if (students[i].equals(student)) {
                if(studentCount % 2 != 0 && i == studentCount - 1){// If there's an odd number of students and this is the last student, return themselves as their partner
                    return students[i];
                }
                if (i % 2 == 0 && i + 1 < studentCount) {// If the student is at an even index, return the student at the next index as their partner
                    return students[i + 1];
                } else if (i % 2 == 1) {
                    return students[i - 1];// If the student is at an odd index, return the student at the previous index as their partner
                } 
            }
        }
        return null; // No partner found
    }
}
