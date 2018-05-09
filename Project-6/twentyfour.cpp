#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring> 
#include <sstream>
typedef std::function<std::vector<std::string>(std::vector<std::string>)> FunctionObject;
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
        std::multimap<int, std::string, std::greater<int>> f_map = std::multimap<int, std::string, std::greater<int>>(), std::string ps="", bool fnset = false)
        :filename(fn), data(dat), word_freqs(wf), final_map(f_map), print_string(ps), function_set(fnset)
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
        }
        custom_object& operator=(const custom_object & co)
        {
            data = co.data;
            word_freqs = co.word_freqs;
            final_map = co.final_map;
            print_string = co.print_string;
            filename = co.filename;
            return *this;
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
    private:
        std::vector<std::string> data;
        std::map<std::string, int> word_freqs;
        std::multimap<int, std::string, std::greater<int>> final_map;
        std::string print_string;
        std::string filename;
        FunctionObject quarantine_obj;
        bool function_set;
};
std::ostream & operator<<(std::ostream& out, custom_object& c)
{
    c.print(out);
    return out;
}
FunctionObject get_input(const char * arg)
{

    return [arg](std::vector<std::string>)
    {
        std::vector<std::string> x;
        x.push_back(arg);
        return x;
    };
}
int main(int argc, char const *argv[])
{

    FunctionObject x = get_input("lmao");
    std::vector<std::string> lma;
    std::cout<<x(lma)[0]<<std::endl;

    return 0;
}