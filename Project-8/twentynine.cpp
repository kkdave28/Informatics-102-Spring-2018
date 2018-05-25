#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#define DEBUG 0


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


bool is_stop_word(std::string & word, std::vector<std::string>& stopwords)
{
    for(auto E: stopwords)
    {
        if(word == E)
        {
            word = "";
            return true;
        }
    }
    return false;
}

std::vector<std::string> init_stopwords()
{
    std::vector<std::string> stop_words;
    std::ifstream StopWordsStream("../stop_words.txt");
    std::string stop_words_string;
    std::getline(StopWordsStream, stop_words_string);
    stop_words_string = convert_to_lower(stop_words_string);
    split(stop_words_string, ",", stop_words);
    return stop_words;
}
std::timed_mutex proc_lock;
std::queue<std::string> word_space;
std::queue<std::map<std::string,int>> freq_space;
std::vector<std::string> stop_words = init_stopwords();
void process_words()
{
    if(proc_lock.try_lock_for(std::chrono::seconds(1)))
    {
        std::map<std::string,int> word_freqs;
        while(true)
        {     
            if(word_space.empty())
            {
                break;
            }
            std::string word = word_space.front();
            word_space.pop();
            if(!is_stop_word(word, stop_words))
            {
                word_freqs[word]+=1;
            }
        }
        freq_space.push(word_freqs);
        
    }
    proc_lock.unlock();
    

}
std::function<std::vector<std::string>(std::string)> extract_words = [] (std::string filename) -> std::vector<std::string>
{
    std::ifstream PandP(filename);
    const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
    std::string buf;
    std::vector<std::string> data;
    while(PandP && std::getline(PandP, buf)) // processing the iput file
    {
        if(buf.length() < 2) // if the line read is an empty line
        {
            continue;
        }
        else
        {
            removeCharsFromString(buf, EscapeChars); //remove escape chars from the string
            buf = convert_to_lower(buf); // convert the entire string to lowercase
            std::istringstream iss(buf); // string as a stream of characters
            std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(data)); // same as a for loop, but captures all the string and puts it into a vector of strings.
            while(std::find(data.begin(),data.end()," ") != data.end()) // find the extra space charaters in the vector 
            {
                std::vector<std::string>::iterator iter;
                iter = find(data.begin(),data.end()," ");
                data.erase(iter); // remove them from the vector
            }
        }
    }
    return data;
};
std::vector<std::pair<std::string, int>> sort_map(std::map<std::string,int> word_freqs)
{
    word_freqs.erase("");
    word_freqs.erase("s");
    std::multimap<int, std::string, std::greater<int>> interm_map;
    std::vector<std::pair<std::string, int>> fin_map;
    for (auto E: word_freqs)
    {
        interm_map.insert(std::pair<int, std::string> (E.second, E.first));
    }
    std::multimap<int, std::string>:: iterator itr; // iterator for the multimap
    int i = 1; // counter to check the number of pairs printed
    for(itr = interm_map.begin(); itr != interm_map.end(); itr++)
    {
        if(i >25)
            break;
        i++;
        fin_map.push_back(std::pair<std::string,int>(itr->second, itr->first));
    }
    return fin_map;
}
int main(int argc, char const *argv[])
{
    for(auto E: extract_words(argv[1]))
    {
        word_space.push(E);
    }
    std::vector<std::thread> workers;
    for(int i=0; i< 5; i++)
    {
        workers.push_back(std::thread(process_words));
    }
    for(auto &th: workers)
    {
        th.join();
    }
    std::map<std::string, int> word_freqs;
    std::map<std::string,int> freqs;
    while(!freq_space.empty())
    {
        freqs = freq_space.front();
        freq_space.pop();

        for(auto E: freqs)
        {
            word_freqs[E.first]+=E.second;
        }
    }
    std::vector<std::pair<std::string,int>> fin_map;
    fin_map = sort_map(word_freqs);
    for(auto E: fin_map)
    {
        std::cout<<E.first<<"  -  "<<E.second<<std::endl;
    }
    return 0;
}
