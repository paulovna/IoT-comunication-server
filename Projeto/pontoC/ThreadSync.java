import java.io.*;
import java.net.Socket;

public class ThreadSync extends Thread{
    Socket cliente;

    public ThreadSync(Socket cliente){
        this.cliente = cliente;
    }

    public void run(){
        try{
            BufferedReader in = new BufferedReader(new InputStreamReader(cliente.getInputStream()));
            String recebimento;
            int x=1;
            while (x<1000){
                recebimento = in.readLine();
                System.out.println("recebido do cliente: " + recebimento);
            }

        }
        

        catch (IOException e){
            e.printStackTrace();
        }
    }
}