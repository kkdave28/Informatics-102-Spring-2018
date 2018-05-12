#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
static std::string filename = "";
void removeCharsFromString( std::string &str, const char* charsToRemove ) // This function explicitly removes all the special characters from the string.
{
   for ( unsigned int i = 0; i < std::strlen(charsToRemove); ++i ) // each character in the string of special characters
   {
    
        while(str.find(std::string(1,charsToRemove[i])) != std::string::npos) // checking whether any instance of the specified character is in the string or not.
        {
            size_t f = str.find(std::string(1,charsToRemove[i])); // get the position of the special character.
            str.replace(f, 1, " "); // replace it with a space.
        }
   }
}   
std::string convert_to_lower(std::string s) // converts the entire string that can be capitalized at various positions to all lower-case characters
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
void split(const std::string& str, const std::string& delim, std::vector<std::string> & tokens) // split functions allows the user to split the string w.r.t. the delimiter specified
{
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev); // find the position of delimiter and store it in 'pos'
        if (pos == std::string::npos) pos = str.length(); // delimiter not found in string at all or reached the end of the string
        std::string token = str.substr(prev, pos-prev); // split the string at the position of the delimiter
        if (!token.empty()) tokens.push_back(token); // if the string is split and stored in the std::string token, add it to the std::set of strings.
        prev = pos + delim.length(); // start finding next delimiter starting from the last position where you found the delimiter.
    }
    while (pos < str.length() && prev < str.length()); // check the pos has not overshot the length of the string.
}

void is_stop_word(std::string & word, const std::vector<std::string> sw)
{
    for(auto E: sw)
    {
        if(word == E)
        {
            word="";
        }
    }
}

class custom_object
{
    public:
        custom_object(std::string fn ="",std::vector<std::string>dat = std::vector<std::string>(),std::map<std::string, int> wf =std::map<std::string, int>(),
        std::multimap<int, std::string, std::greater<int>> f_map = std::multimap<int, std::string, std::greater<int>>(), std::string ps="", bool setf=false, std::function<custom_object&(custom_object&)> _fcn_obj = nullptr)
        :filename(fn), data(dat), word_freqs(wf), final_map(f_map), print_string(ps), is_fnc_set(setf), fcn_obj(_fcn_obj)
        {
            //Empty, all initialization has been done in the initializer list.
        }
        
        custom_object(const custom_object& co)
        {
            data = co.data;
            word_freqs = co.word_freqs;
            final_map = co.final_map;
            print_string = co.print_string;
            filename = co.filename;
            fcn_obj = co.fcn_obj;
            is_fnc_set = co.is_fnc_set;
        }
        custom_object& operator=(const custom_object & co)
        {
            data = co.data;
            word_freqs = co.word_freqs;
            final_map = co.final_map;
            print_string = co.print_string;
            filename = co.filename;
            fcn_obj = co.fcn_obj;
            is_fnc_set = co.is_fnc_set;
            return *this;
        }
        custom_object& operator()()
        {
            if(fcn_set())
            {
                fcn_set() = false;
                return fcn_obj(*this);
            }
            else
            {
                return *this;
            }
        }
        std::vector<std::string> & get_data()
        {
            return data;
        }
        std::map<std::string, int> & get_word_freq()
        {
            return word_freqs;
        }
        std::multimap<int, std::string, std::greater<int>> & get_final_map()
        {
            return final_map;
        }
        std::string & get_print_string()
        {
            return print_string;
        }
        std::string& get_filename()
        {
            return filename;
        }
        void print(std::ostream& out) const
        {
            out<<print_string;
        }
        std::function<custom_object&(custom_object&)> & get_fcn_obj()
        {
            return fcn_obj;
        }
        bool &fcn_set()
        {
            return is_fnc_set;
        }
    private:
        std::vector<std::string> data;
        std::map<std::string, int> word_freqs;
        std::multimap<int, std::string, std::greater<int>> final_map;
        std::string print_string;
        std::string filename;
        bool is_fnc_set;
        std::function<custom_object&(custom_object&)> fcn_obj;
};
typedef std::function<custom_object&(custom_object&)> FuncObj;
std::ostream & operator<<(std::ostream& out, custom_object& c)
{
    c.print(out);
    return out;
}
class TFQuarantine
{
    public:
        TFQuarantine(FuncObj func)
        {
            all_funcs.push_back(func);
        }
        TFQuarantine bind(FuncObj func)
        {
            all_funcs.push_back(func);
            return *this;
        }
        void execute()
        {
            FuncObj guard_callable = [](custom_object&v)-> custom_object&{
                return v();
            };
            for(auto func: all_funcs)
            {
                value = func(guard_callable(value));
            }
            std::cout<<value;
        }
    private:
        std::vector<FuncObj> all_funcs;
        custom_object value;
};
custom_object& get_file_name(custom_object& arg)
{
    arg.get_fcn_obj() = [] (custom_object& another) -> custom_object&{
        another.get_filename() = filename;
        return another;
    };
    arg.fcn_set() = true;
    return arg;
}
custom_object& extract_words(custom_object& path_to_file)
{
    path_to_file.get_fcn_obj() = [](custom_object& another) -> custom_object&
    {
        std::ifstream PandP(another.get_filename());
        std::string buf;
        std::vector<std::string> wordline;
        const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
        while(PandP && std::getline(PandP, buf)) // processing the iput file
        {
            if(buf.length() == 0) // if the line read is an empty line
            {
                continue;
            }
            else
            {
                removeCharsFromString(buf, EscapeChars); //remove escape chars from the string
                buf = convert_to_lower(buf); // convert the entire string to lowercase
                std::istringstream iss(buf); // string as a stream of characters
                
                std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(wordline)); // same as a for loop, but captures all the string and puts it into a vector of strings.
                while(std::find(wordline.begin(),wordline.end()," ") != wordline.end()) // find the extra space charaters in the vector 
                {
                    std::vector<std::string>::iterator iter;
                    iter = find(wordline.begin(),wordline.end()," ");
                    wordline.erase(iter); // remove them from the vector
                }
                for(auto E: wordline) // iterate over the vector and put them in the map
                {
                  another.get_data().push_back(E);       
                }                        
            }
                wordline.clear(); // remove all strings stored in vector for fresh processing of the new line
                //
            }
        return another;
          
    };
    path_to_file.fcn_set() = true;
    return path_to_file;
}
custom_object& remove_stop_words(custom_object & data)
{
    data.get_fcn_obj() = [](custom_object& another) -> custom_object&{
        std::vector<std::string> stop_word;
        std::ifstream StopWordsStream("../stop_words.txt");
        std::string stop_words_string;
        std::getline(StopWordsStream, stop_words_string);
        stop_words_string = convert_to_lower(stop_words_string);
        split(stop_words_string, ",", stop_word);

        std::vector<std::string> word_list;

        for(auto E: another.get_data())
        {
            std::string buf = E;
            is_stop_word(buf, stop_word);
            word_list.push_back(buf);
        }
        another.get_data() = word_list;
        return another;
    };
    data.fcn_set() = true;
    return data;
}

// custom_object& read_file(custom_object& path_to_file)
// {
//     std::ifstream PandP(path_to_file.get_filename());
//     std::string buf;
//     while(PandP && std::getline(PandP, buf)) // processing the iput file
//     {
//         if(buf.length() == 0) // if the line read is an empty line
//         {
//             continue;
//         }
//         else
//         {
//             path_to_file.get_data().push_back(buf);
//         }
//     }
//     return path_to_file;
    
// }
// custom_object& filter_chars(custom_object& str_data)
// {
//     const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
//     for(int i=0; i< str_data.get_data().size(); i++)
//     {
//         removeCharsFromString(str_data.get_data()[i], EscapeChars);
//     }
//     return str_data;

// }
// custom_object& normalize(custom_object& str_data)
// {
//     for(int i=0; i< str_data.get_data().size(); i++)
//     {
//         str_data.get_data()[i] = convert_to_lower(str_data.get_data()[i]);
//     }
//     return str_data;
// }
// custom_object& scan(custom_object& str_data)
// {
//     std::vector<std::string> word_list;
//     for(auto E: str_data.get_data())
//     {
//         std::istringstream iss(E);
//         std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(word_list));
//         while(std::find(word_list.begin(),word_list.end()," ") != word_list.end()) // find the extra space charaters in the vector 
//         {
//             std::vector<std::string>::iterator iter;
//             iter = find(word_list.begin(),word_list.end()," ");
//             word_list.erase(iter); // remove them from the vector
//         }
//     }
//     str_data.get_data() = word_list;
//     return str_data;
// }
// custom_object& remove_stop_words(custom_object& data)
// {
//     std::vector<std::string> stop_word;
//     std::ifstream StopWordsStream("../stop_words.txt");
//     std::string stop_words_string;
//     std::getline(StopWordsStream, stop_words_string);
//     stop_words_string = convert_to_lower(stop_words_string);
//     split(stop_words_string, ",", stop_word);

//     std::vector<std::string> word_list;

//     for(auto E: data.get_data())
//     {
//         std::string buf = E;
//         is_stop_word(buf, stop_word);
//         word_list.push_back(buf);
//     }
//     data.get_data() = word_list;
//     return data;
// }

custom_object& frequencies(custom_object& word_list)
{
    for(auto E: word_list.get_data())
    {
        word_list.get_word_freq()[E]+=1;
    }
    return word_list;
}

custom_object& sort(custom_object& word_freqs)
{
    word_freqs.get_word_freq().erase("");
    word_freqs.get_word_freq().erase("s");
    for(auto E: word_freqs.get_word_freq())
    {
        word_freqs.get_final_map().insert(std::pair<int, std::string>(E.second, E.first));
    }
    return word_freqs;
}
custom_object& top25_freqs(custom_object& final_map)
{
    std::multimap<int, std::string>:: iterator itr;
    int i=1;
    std::stringstream buf;
    for(itr = final_map.get_final_map().begin(); itr != final_map.get_final_map().end(); itr++)
    {
        // if(i!= 1)
        // {
        //     buf<<"\n";
        // }
        if(i>25){break;}
        i++;
        buf<<itr->second<<"  -  "<<itr->first<<std::endl;
    }

    final_map.get_print_string() = buf.str();
    return final_map;
}
int main(int argc, char*argv[])
{
    filename = argv[1];
    TFQuarantine(get_file_name).bind(extract_words).bind(remove_stop_words).bind(frequencies).bind(sort).bind(top25_freqs).execute();
    //TFTheOne(argv[1]).bind(read_file).bind(filter_chars).bind(normalize).bind(scan).bind(remove_stop_words).bind(frequencies).bind(sort).bind(top25_freqs).printme();
    return 0;
}