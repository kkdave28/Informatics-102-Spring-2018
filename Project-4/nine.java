import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.Map.Entry;

import java.lang.*;

class nine
{
    private static Object read_file(Object filename)
    {
        try{
        return new String(Files.readAllBytes(Paths.get((String)filename)));
        }
        catch (IOException e)
        {
            
            e.printStackTrace();
            return null;
        }
    }
    private static Object filter_char(Object data)
    {
        return ((String)data).replaceAll("[^a-zA-Z ]+", " ");
    }
    private static Object normalize(Object data)
    {
        return ((String)data).toLowerCase();
    }
    private static Object scan(Object data)
    {
        return ((String)data).split(" ");
    }
    private static Object remove_stop_words(Object data)
    {
        try{
        String swstr = new String(Files.readAllBytes(Paths.get("../stop_words.txt"))).replaceAll("[^a-zA-Z0-9]"," ").toLowerCase();
        List<String> stopwords = new ArrayList<String>(Arrays.asList(swstr.split(" ")));
        List<String> ret = new ArrayList<String>();
        String [] wordlist = (String[])data;
        for(int i=0; i<wordlist.length; i++)
        {
            if(!stopwords.contains(wordlist[i]))
            {
                ret.add(wordlist[i]);
            }
        }
        return ret;
    }
    catch (IOException e)
    {
        e.printStackTrace();
        return null;
    }
    }
    private static Object freqeuncies(Object data)
    {
        Map<String, Integer> ret = new HashMap<>();
        for(int i=0; i<((List<String>)data).size(); i++)
        {
            if(ret.containsKey(((List<String>)data).get(i)))
            {
                ret.put(((List<String>)data).get(i), ret.get(((List<String>)data).get(i))+1);
            }
            else
            {
                ret.put(((List<String>)data).get(i), 1);
            }
        }
        ret.remove("");
        ret.remove("s");
        return ret;

        //return data;
    }
    private static Object sort(Object word_freqs_map)
    {		
        return ((Map<String, Integer>) word_freqs_map).entrySet().stream().sorted((o1, o2) -> {int v1 = o1.getValue();int v2 = o2.getValue();return (v1 > v2) ? -1 : ((v1 == v2) ? 0 : 1);}).collect(Collectors.toList());
    }
    private static Object top25_freqs(Object word_freqs_list)
    {
        List<Entry<String, Integer>> word_freqs = (List<Entry<String, Integer>>) word_freqs_list;
		StringBuilder buf = new StringBuilder();
		
        for (int i = 0; i < 25; i++) 
        {
            if(i> 0)
            {
                buf.append("\n");
            }
			Entry<String, Integer> tf = word_freqs.get(i);
			buf.append(tf.getKey() + " - " + tf.getValue());
		}
		
        return buf.toString();
    }
    public static void main(String args[])throws IOException
    {
        new TermFrequencyTheOne(args[0]).bind(nine::read_file).bind(nine::filter_char).bind(nine::normalize).bind(nine::scan).bind(nine::remove_stop_words).bind(nine::freqeuncies).bind(nine::sort).bind(nine::top25_freqs).printme();
    }
}

class TermFrequencyTheOne
{
    private Object val;
    TermFrequencyTheOne(Object v)
    {
        val = v;
    }
    public TermFrequencyTheOne bind(Function<Object, Object> func)
    {
        this.val = func.apply(this.val);
        return this;
    }
    public void printme()
    {
        System.out.println(this.val);
    }
}