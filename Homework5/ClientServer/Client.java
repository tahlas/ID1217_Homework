package ClientServer;
import java.rmi.*;

public class Client {

    
    public static void main(String[] args) {
        int host = 1099;
        //String rmiregURL = "rmi//" + host;
        String rmiregURL = "rmi://localhost:1099/teacher"; // Construct the RMI registry URL to look up the teacher object
        try {
            Teacher teacher = (Teacher) Naming.lookup(rmiregURL); 
            int index = Integer.parseInt(args[0]);  
            Student student = new StudentImpl(index); 
            teacher.registerStudent(student); 

            Student partner = teacher.getPartner(student); // Get the partner assigned by the teacher
            System.out.println("Student " + student.getIndex() + " is paired with Student " + partner.getIndex()); // Print the partner's index

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
