import java.net.Socket;

public class pontoC{
    public static void main(String[] args) {
        System.out.println("Medidor de vazão no ponto C:\n");
        ThreadClock tclock = new ThreadClock();
        tclock.start();

        try {
          Socket cliente = new Socket("localhost",12000);
          ThreadSensor tsensor = new ThreadSensor(cliente);
          tsensor.start();
          ThreadSync tsync = new ThreadSync(cliente);
          tsync.start();
        }
        catch(Exception e) {
          System.out.println("Erro: " + e.getMessage());
        }
    }
}    