import java.rmi.*;


public class Server {
    public static void main(String[] args) {
        try{
            Teacher teacher = new TeacherImpl();
            //Student student = new StudentImpl();
            Naming.rebind("teacher", teacher);
            System.out.println("Teacher is ready.");
        }
        catch(Exception e){
            e.printStackTrace();
        }      
    }
}
