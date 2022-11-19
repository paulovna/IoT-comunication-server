import java.net.Socket;
import java.io.*;
import java.util.Random;


public class ThreadSensor extends Thread{
    Socket cliente;

    public ThreadSensor(Socket cliente){
        this.cliente = cliente;
    }

    public String definirHoraAtual(){
        String horaatual = "";
        horaatual = horaatual+Global.hora;
        horaatual = horaatual+":";
        horaatual = horaatual+Global.minuto;
        horaatual = horaatual+":";
        horaatual = horaatual+Global.segundo;
    
        return horaatual;
    }

    public static String medicao() throws IOException {
        Random random = new Random();
        int sensor = random.nextInt(100);
        sensor+=1; //valores de 1 a 100;
        String valor = Integer.toString(sensor);
        return valor;
    }

    public static String novamedicao(String ultimovalor) throws IOException{
        Random random = new Random();
        int sensor = random.nextInt(7);
        int ultimonum = Integer.parseInt(ultimovalor);

        sensor = sensor - 3 + ultimonum;

        if(sensor<0){
            sensor = 0;
        }
        if(sensor>100){
            sensor = 100;
        }
        String valor = Integer.toString(sensor);
        return valor;
    }

    public void run(){
    try {
        Thread.sleep(1000);

        PrintStream saida = new PrintStream(cliente.getOutputStream());
        saida.println("C");
        
        while (Global.hora == "1" && Global.minuto == "2" && Global.segundo == "3"){
            continue;
        }
        
        int x=0;

        String ultimovalor = medicao();
        String horaAtual = definirHoraAtual();
        String dados = horaAtual+"*"+ultimovalor+"%\n";


        saida.println(dados);//Enviando o primeiro dado

        while(x<10){
            if(Global.enviar == true)//So vai enviar o dado novamente depois que mudar o ciclo, controlado pela Thread Clock
            {
                horaAtual = definirHoraAtual();
                ultimovalor = novamedicao(ultimovalor);

                dados = horaAtual+"*"+ultimovalor+"%\n";

                saida.println(dados);
                x+=1;

                Global.enviar = false;
            }
        }
        cliente.close();
      }
      catch(Exception e) {
        System.out.println("Erro: " + e.getMessage());
      }
    }
}