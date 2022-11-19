import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

public class pontoCcopy{

    public static String ultimovalor() throws IOException{
        String path = "logpontoC.txt";
        String result = "";
        BufferedReader leitor = new BufferedReader(new FileReader(path));
        String linha = "";
        while (true) {
            if (linha != null) {
				result = linha;

			} else
				break;
			linha = leitor.readLine();
        }
        leitor.close();
        return result;
    }
       
    public static void registrarValor(String valor) throws IOException{
        String path = "logpontoC.txt";
        BufferedWriter escritor = new BufferedWriter(new FileWriter(path));
        escritor.write(valor);
		escritor.close();
    }        

    public static void medicao() throws IOException {
        Random random = new Random();
        int sensor = random.nextInt(100);
        sensor+=1; //valores de 1 a 100;
        String valor = Integer.toString(sensor);
        registrarValor(valor);
        valor = ultimovalor();
        System.out.println("Vazão no ponto C: " +valor+"%");
    }

    public static void novamedicao() throws IOException{
        String ultimodado = "0";
        ultimodado = ultimovalor();

        Random random = new Random();
        int sensor = random.nextInt(7);
        int ultimonum = Integer.parseInt(ultimodado);

        sensor = sensor - 3 + ultimonum;
        String valor = Integer.toString(sensor);

        registrarValor(valor);

        valor = ultimovalor();
        System.out.println("Vazão no ponto C: " +valor+"%");
    }

}    