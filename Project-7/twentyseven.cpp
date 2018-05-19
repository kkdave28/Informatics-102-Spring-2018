#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <sstream>

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
            return true;
        }
    }
    return false;
}
std::vector<std::pair<std::string, int>> sort_map(std::map<std::string,int> word_freqs)
{
    std::multimap<int, std::string, std::greater<int>> interm_map;
    std::vector<std::pair<std::string, int>> fin_map;
    for(auto E: word_freqs)
    {
        interm_map.insert(std::pair<int, std::string> (E.second, E.first));
    }
    std::multimap<int, std::string>:: iterator itr; // iterator for the multimap
    for(itr = interm_map.begin(); itr != interm_map.end(); itr++)
    {
        fin_map.push_back(std::pair<std::string,int>(itr->second, itr->first));
    }
    return fin_map;
}
class CharacterGenerator
{
    
    public:
        CharacterGenerator(){}
        char operator()()
        {     
            char c;
            if(file.get(c))
            {
                return c;
            }            
            return 0;
        }
        bool end()
        {
            return file.eof();
        }
        void open(std::string filename)
        {
            file.open(filename, std::ifstream::in);
        }
    private:
        std::ifstream file;
};

class WordGenerator
{
    public:
        WordGenerator()
        {}
        void create_instance(std::string filename)
        {
            cgen.open(filename);
            start_char = true;
        }
        std::string operator()()
        {
            start_char = true;
            char c;
            while(!end())
            {
                c = cgen();
                if (start_char)
                {
                    word = "";
                    if (isalnum(c))
                    {
                        word = std::string(1,c);
                        word = convert_to_lower(word);
                        start_char = false;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    if(isalnum(c))
                    {
                        word += convert_to_lower(std::string(1,c));
                    }
                    else
                    {
                        start_char = true;
                        return word;
                    }
                }

            }
            return "";

        }
        bool end()
        {
            return cgen.end();
        }
    private:
        CharacterGenerator cgen;
        bool start_char;
        std::string word;
};
class NonStopWordsGenerator
{
    public:
        NonStopWordsGenerator(){}
        std::string operator()()
        {
            while(!end())
            {
                std::string test = wgen();
                if(!is_stop_word(test, StopWords) && test.length() >= 2)
                {
                    return test;
                }
            }
            return "";
        }
        bool end()
        {
            return wgen.end();
        }
        void create_instance(std::string filename)
        {
            wgen.create_instance(filename);
            std::ifstream StopWordsStream("../stop_words.txt");
            std::string stop_words;
            std::getline(StopWordsStream,stop_words);
            split(stop_words, ",",StopWords);
        }

    private:
        WordGenerator wgen;
        std::vector<std::string> StopWords;
};
class FreqGenerator
{
    public:
        FreqGenerator(std::string filename)
        {
            nonswgen.create_instance(filename);
            counter = 1;
        }
        std::vector<std::pair<std::string, int>> operator()()
        {
            while (!nonswgen.end())
            {
                freq_map[nonswgen()] +=1;
                if(counter % 5000 == 0)
                {
                    return sort_map(freq_map);
                }
                counter++;
            }
            return sort_map(freq_map);
        }
        bool end()
        {
            return nonswgen.end();
        }

    private:
        std::map<std::string, int> freq_map;
        NonStopWordsGenerator nonswgen;
        int counter;
};
int main(int argc, char *argv[])
{
    FreqGenerator freqg(argv[1]);
    while(!freqg.end())
    {
        auto list_of_freqs = freqg();
        std::cout<<"------------------------------"<<std::endl;
        for(int i=0; i< 25; i++)
        {
            std::cout<<list_of_freqs[i].first<<"  -  "<<list_of_freqs[i].second<<std::endl;
        }
    }
    return 0;
}
