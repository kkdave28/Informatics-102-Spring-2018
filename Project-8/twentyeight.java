import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;

import javafx.beans.binding.ObjectExpression;

import java.util.Map.Entry;
import java.lang.*;

class ActiveWordFreqObj implements Runnable
{
    public Queue<Object> message_queue = new LinkedList<>();
    public boolean stop;
    private Thread t;
    ActiveWordFreqObj()
    {
        stop = false;
        if(t == null)
        {
            t = new Thread(this, this.getClass().getSimpleName());
            t.start();
        }


    }
    public void dispatch(){}
    public void run()
    {
        while(!stop)
        {
            Object a = message_queue.poll();
            this.dispatch(a);
            if (String.valueOf(a) == "die")
            {
                stop = true;
            }

        }
    }
}
class DataStorageObject extends ActiveWordFreqObj
{
    private String data;
    private Object swm;
    DataStorageObject()
    {
        super();
    }
    public void dispatch(ArrayList<Object> message)
    {
        initialize(new ArrayList<>(message.subList(1, message.size())));
    }
    public void initialize(ArrayList<Object> message) throws IOException
    {
        data = new String(Files.readAllBytes(Paths.get((String)message.get(0)))).replaceAll("[^a-zA-Z0-9]"," ").toLowerCase();
        swm = message.get(1);
    }
    public void printdaata()
    {
        System.out.print(data);
    }
}

class twentyeight
{
    public static void main(String args[]) 
    {
        System.out.println(args[0]);
        ActiveWordFreqObj y = new ActiveWordFreqObj();
        DataStorageObject d =new DataStorageObject();
        ArrayList<Object> m= new ArrayList<>();
        m.add("init");
        m.add(args[0]);
        m.add(y);
        send(d, m);
        d.printdaata();
    }
    public static void send(Object reciever, ArrayList<Object> message)
    {
        reciever.dispatch(message);
    }
}