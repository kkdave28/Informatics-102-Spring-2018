import java.io.*;
import java.nio.file.*;
import java.util.*;

class fiveone
{
    private static String read_file(String path_to_file)throws IOException
    {
        try{
        String content = new String(Files.readAllBytes(Paths.get(path_to_file)));
        return content;
        }
        catch (IOException e)
        {
            String x = "";
            e.printStackTrace();
            return x;
        }
    }
    private static List<String> scan(String data)
    {
        List<String> myList = new ArrayList<String>(Arrays.asList(data.split(" ")));
        return myList;
    }
    private static String filter_chars_and_normalize(String data)
    {
        String ret = data.replaceAll("[^a-zA-Z0-9]"," ");
        return ret.toLowerCase();
    }
    private static List<String> remove_stop_words(List<String> word_list)throws IOException
    {
        List<String> stop_words = scan(filter_chars_and_normalize(read_file("../stop_words.txt")));
        List<String> ret = new ArrayList<String>();
        for(int i=0; i<word_list.size(); i++)
        {
            if(!stop_words.contains(word_list.get(i)))
            {
                ret.add(word_list.get(i));
            }
        }
        return ret;
    }
    private static HashMap<String,Integer> sort(HashMap<String, Integer> map) { 
        List list = new LinkedList(map.entrySet());
        Collections.sort(list, new Comparator() {
             public int compare(Object o1, Object o2) {
                return ((Comparable) ((Map.Entry) (o2)).getValue())
                   .compareTo(((Map.Entry) (o1)).getValue());
             }
        });
 
        HashMap sortedHashMap = new LinkedHashMap();
        for (Iterator it = list.iterator(); it.hasNext();) {
               Map.Entry entry = (Map.Entry) it.next();
               sortedHashMap.put(entry.getKey(), entry.getValue());
        } 
        return sortedHashMap;
   }
    private static HashMap<String, Integer> frequencies(List<String> word_list)
    {
        HashMap<String, Integer> ret = new HashMap<String, Integer>();
        for(int i=0; i<word_list.size(); i++)
        {
            if(ret.containsKey(word_list.get(i)))
            {
                ret.put(word_list.get(i), ret.get(word_list.get(i))+1);
            }
            else
            {
                ret.put(word_list.get(i), 1);
            }
        }
        ret.remove("");
        ret.remove("s");
        return ret;
    }
    private static void print_all(HashMap<String, Integer> word_dict)
    {
        int i = 1;
        for(Map.Entry<String, Integer> entry : word_dict.entrySet())
        {
            if(i>25)
            {break;}
            System.out.println(entry.getKey() + "  -  "+ entry.getValue());
            i++;
        }
    }
    public static void main(String args[])
    {
        try
        {
            print_all(sort(frequencies(remove_stop_words(scan(filter_chars_and_normalize(read_file(args[0])))))));
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
}