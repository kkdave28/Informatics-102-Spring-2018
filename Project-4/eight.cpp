#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

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

void no_func()
{
    return;
}
void print_text(std::multimap<int, std::string, std::greater<int>> final_map, void * next_func)
{
    std::multimap<int, std::string>:: iterator itr; // iterator for the multimap
    int i = 1; // counter to check the number of pairs printed
    for(itr = final_map.begin(); itr != final_map.end(); itr++)
    {
        if(i >25)
            break;
        i++;
        std::cout<<itr->second<<"  -  "<<itr->first<<std::endl; // print the output to the terminal
    }
    reinterpret_cast<void(*)()>(next_func)();
}
void sort(std::map<std::string, int> word_freqs, void * next_func)
{
    std::multimap<int, std::string, std::greater<int>> final_map;
    word_freqs.erase("");
    word_freqs.erase("s");
    for(auto E: word_freqs)
    {
        final_map.insert(std::pair<int, std::string> (E.second, E.first));
    }
    reinterpret_cast<void(*)(std::multimap<int, std::string, std::greater<int>>, void*)>(next_func)(final_map, (void*)no_func);
}
void frequencies(std::vector<std::string> word_list, void* next_func)
{
    std::map<std::string, int> word_freqs;
    for(auto E: word_list)
    {
        word_freqs[E]+=1;
    }
    reinterpret_cast<void(*)(std::map<std::string, int>, void*)>(next_func)(word_freqs, (void*)print_text);
}
void remove_stopwords(std::vector<std::string> data, void* next_func)
{
    std::vector<std::string> stop_word;
    std::ifstream StopWordsStream("../stop_words.txt");
    std::string stop_words_string;
    std::getline(StopWordsStream, stop_words_string);
    stop_words_string = convert_to_lower(stop_words_string);
    split(stop_words_string, ",", stop_word);

    std::vector<std::string> word_list;

    for(auto E: data)
    {
        std::string buf = E;
        is_stop_word(buf, stop_word);
        word_list.push_back(buf);
    }

    reinterpret_cast<void(*)(std::vector<std::string>, void*)>(next_func)(word_list, (void*)sort);

}
void scan(std::vector<std::string> data, void * next_func)
{
    std::vector<std::string> word_list;
    for(auto E: data)
    {
        std::istringstream iss(E);
        std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(word_list));
        while(std::find(word_list.begin(),word_list.end()," ") != word_list.end()) // find the extra space charaters in the vector 
            {
                std::vector<std::string>::iterator iter;
                iter = find(word_list.begin(),word_list.end()," ");
                word_list.erase(iter); // remove them from the vector
            }
    }
    reinterpret_cast<void(*)(std::vector<std::string>, void*)>(next_func)(word_list, (void*)frequencies);
}
void normalize(std::vector<std::string> data, void * next_func)
{
    for(int i=0; i<data.size(); i++)
    {
        data[i] = convert_to_lower(data[i]);
    }
    reinterpret_cast<void(*)(std::vector<std::string>, void*)>(next_func)(data, (void*)remove_stopwords);
}

void filter_chars(std::vector<std::string> data, void * next_func)
{
    const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
    for(int i=0; i<data.size(); i++)
    {
        removeCharsFromString(data[i],EscapeChars);
    }
    // while(std::find(data.begin(),data.end()," ") != data.end())
    // {
    //     std::vector<std::string>::iterator iter;
    //     iter = find(data.begin(),data.end()," ");
    //     data.erase(iter); // remove them from the vector
    // }
    reinterpret_cast<void(*)(std::vector<std::string>, void*)>(next_func)(data, (void*)scan);
}
void read_file(std::string filename, void * next_func)
{
    std::vector<std::string> data;
    std::ifstream PandP(filename);
    std::string buf;
    
    while(PandP && std::getline(PandP, buf)) // processing the iput file
    {
        if(buf.length() == 0) // if the line read is an empty line
        {
            continue;
        }
        else
        {
            data.push_back(buf);
        }
    }
    reinterpret_cast<void(*) (std::vector<std::string>, void*)> (next_func)(data, (void*)normalize);

}
int main(int argc, char * argv[])
{
    read_file(argv[1], (void*)filter_chars);
    return 0;
}