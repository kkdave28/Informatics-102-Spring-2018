import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.lang.*;

class six
{
    static int counter = 1;
    public static void main(String args[]) throws IOException
    {
        String wordstr = new String(Files.readAllBytes(Paths.get(args[0]))).replaceAll("[^a-zA-Z0-9]"," ").toLowerCase();
        String swstr = new String(Files.readAllBytes(Paths.get("../stop_words.txt"))).replaceAll("[^a-zA-Z0-9]"," ").toLowerCase();
        List<String> all_words = new ArrayList<String>(Arrays.asList(wordstr.split(" ")));
        List<String> stopwords = new ArrayList<String>(Arrays.asList(swstr.split(" ")));
        HashMap<String,Integer> map = new HashMap<String, Integer>();
        for(String x: all_words){if(map.containsKey(x) && !stopwords.contains(x)){map.put(x,map.get(x)+1);}else{map.put(x,1);}}
        map.remove("s");map.remove("");        
        map.entrySet().stream().sorted((k1, k2) -> -k1.getValue().compareTo(k2.getValue())).limit(25).forEach(k ->{System.out.println(k.getKey() + "  -  " + k.getValue());});
    }
}
