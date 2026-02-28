import java.rmi.*;


public class Server {
    public static void main(String[] args) {
        try{
            Teacher teacher = new TeacherImpl(Integer.parseInt(args[0]));
            System.out.println("There are " + args[0] + " students in total.");
            //Student student = new StudentImpl();
            Naming.rebind("teacher", teacher);
            System.out.println("Teacher is ready.");
        }
        catch(Exception e){
            e.printStackTrace();
        }      
    }
}
