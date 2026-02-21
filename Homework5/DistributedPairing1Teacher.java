import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.*;

public class DistributedPairing1Teacher {
    public static void main(String[] args) {
        int numberOfStudents = 5;

        ServerSocket serverSocket;
        try {
            serverSocket = new ServerSocket(8080);
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        for (int i = 0; i < numberOfStudents; i++) {
            try {
                Socket socket = serverSocket.accept();
                Handler handler = new Handler(socket);
                //handler.setPriority(handler.getPriority() + 1);
                handler.start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

class Handler extends Thread {
    private Socket socket;

    Handler(Socket socket) throws IOException {
        this.socket = socket;
    }

    public void run() {
        // communicate with the client
        try {
            BufferedReader rd = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            String indexString = rd.readLine();
            int studentIndex = Integer.parseInt(indexString);
            

                       

            rd.close();
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }


    }
}