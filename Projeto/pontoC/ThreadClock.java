import java.text.SimpleDateFormat;
import java.util.Date;
import java.lang.Thread;
import java.util.Random;

public class ThreadClock extends Thread
{
    Random random = new Random();

    public int atualizacaoClock()
    {
        int aux = random.nextInt(5);
        aux = aux-2;
        return aux;
    }
    public int atualizacaoCiclo()
    {
        int aux = random.nextInt(4);
        aux = aux+7;
        aux = aux*1000;
        return aux;
    }

    public void run()
    {
        try
        {
            int ciclo;
        
            Date date = new Date(System.currentTimeMillis());

            SimpleDateFormat fhora = new SimpleDateFormat("HH");
            SimpleDateFormat fmin = new SimpleDateFormat("mm");
            SimpleDateFormat fseg = new SimpleDateFormat("ss");

            String h = fhora.format(date);
            String m = fmin.format(date);
            String s = fseg.format(date);

            Global.hora = h;
            Global.minuto = m;
            Global.segundo = s;

            int ss = Integer.parseInt(s);
            int mm = Integer.parseInt(m);
            int hh =Integer.parseInt(h);

            while (true){
                ciclo = atualizacaoCiclo();
                Thread.sleep(ciclo);

                ss=ss+atualizacaoClock();

                if(ss>=60)
                {
                    ss = ss-60;
                    mm=mm+1;
                    if(mm==60)
                    {
                        mm=0;
                        hh=hh+1;
                        if(hh==24)
                        {
                            hh=0;
                        }
                    }
                }
                else if(ss<0)
                {
                    ss=60+ss;
                    mm=mm-1;
                    if(mm<0)
                    {
                        mm=60+mm;
                        hh=hh-1;
                        if(hh<0)
                        {
                            hh=23;
                        }
                    }    
                }

                s=Integer.toString(ss);
                h=Integer.toString(hh);
                m=Integer.toString(mm);

                Global.hora = h;
                Global.minuto = m;
                Global.segundo = s;

                Global.enviar = true;
                
            } 
        }

        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
}
