import java.rmi.*;

public class Client {

    public static void main(String[] args) {
        int host = 1099;
        //String rmiregURL = "rmi//" + host;
        String rmiregURL = "rmi://localhost:1099/teacher";
        try {
            Teacher teacher = (Teacher) Naming.lookup(rmiregURL);
            int index = Integer.parseInt(args[0]);
            Student student = new StudentImpl(index);
            teacher.registerStudent(student);

            Student partner = teacher.getPartner(student);
            System.out.println("Student " + student.getIndex() + " is paired with Student " + partner.getIndex());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
