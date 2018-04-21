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
class WordFrequencyFramework
{
    public:
        void register_load_handler(std::function<void(std::string)> func)
        {
            load_handlers.push_back(func);
        }
        void register_work_handler(std::function<void()> func)
        {
            work_handlers.push_back(func);
        }
        void register_end_handler(std::function<void()> func)
        {
            end_handlers.push_back(func);
        }
        void run(std::string filename)
        {
            for(auto E: load_handlers)
            {
                E(filename);
            }
            for(auto E: work_handlers)
            {
                E();
            }
            for(auto E: end_handlers)
            {
                E();
            }
        }
    private:
        std::vector<std::function<void(std::string)>> load_handlers;
        std::vector<std::function<void()>> work_handlers;
        std::vector<std::function<void()>> end_handlers;
};
class StopWordFilter
{
    public:
        StopWordFilter(){}
        StopWordFilter(WordFrequencyFramework &wfapp)
        {
            using namespace std::placeholders;
            wfapp.register_load_handler(std::bind(&StopWordFilter::load, this, _1));
        }
        void load(std::string fname ="")
        {
            std::ifstream StopWordsStream("../stop_words.txt");
            std::string stop_words_string;
            std::getline(StopWordsStream, stop_words_string);
            stop_words_string = convert_to_lower(stop_words_string);
            split(stop_words_string, ",", stop_words);
        }
        void is_stop_word(std::string word, std::string & buf)
        {
            buf = word;
            if(std::find(stop_words.begin(), stop_words.end(), word) != stop_words.end())
            {
                buf = "";
            }
        }
    private:
        std::vector<std::string> stop_words;
};

class DataStorage
{
    public:
        DataStorage(WordFrequencyFramework &wfapp, StopWordFilter& stop_word_filter)
        {
            using namespace std::placeholders;
            stop_words_filter = stop_word_filter;
            wfapp.register_load_handler(std::bind(&DataStorage::load, this, _1));
            wfapp.register_work_handler(std::bind(&DataStorage::produce, this));
            //wfapp.register_for_load_event((void*)&DataStorage::load);
            //wfapp.register_for_dowork_event((void*)&DataStorage::produce);
        }
        void load(std::string filename)
        {
            std::ifstream PandP(filename);
            std::string buf;
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
                    std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(data)); // same as a for loop, but captures all the string and puts it into a vector of strings.
                    while(std::find(data.begin(),data.end()," ") != data.end()) // find the extra space charaters in the vector 
                    {
                        std::vector<std::string>::iterator iter;
                        iter = find(data.begin(),data.end()," ");
                        data.erase(iter); // remove them from the vector
                    }
                }
            }
        }
        void produce()
        {
            for(auto E: data)
            {
                std::string buf = "";
                stop_words_filter.is_stop_word(E,buf);
                for (auto F: word_event_handlers)
                {
                    F(buf);
                }
            }
        }
        void register_for_word_event(std::function<void(std::string)> handler)
        {
            word_event_handlers.push_back(handler);
        }

    private:
        std::vector<std::string> data;
        StopWordFilter stop_words_filter;
        std::vector<std::function<void(std::string)>> word_event_handlers;
};

class WordFrequencyCounter
{
    public:
        WordFrequencyCounter(WordFrequencyFramework &wfapp, DataStorage& dstor)
        {
            using namespace std::placeholders;
            dstor.register_for_word_event(std::bind(&WordFrequencyCounter::increment_count, this, _1));
            wfapp.register_end_handler(std::bind(&WordFrequencyCounter::print_freqs, this));
            //wfapp.register_for_end_event((void*)&WordFrequencyCounter::print_freqs);
        }
        void increment_count(std::string word)
        {
            word_freqs[word]+=1;
        }
        void print_freqs()
        {
            std::multimap<int, std::string, std::greater<int>> final_map;
            for(auto E: word_freqs)
            {
                final_map.insert(std::pair<int, std::string> (E.second, E.first));
            }
            std::multimap<int, std::string>:: iterator itr; // iterator for the multimap
            int i = 1; // counter to check the number of pairs printed
            for(itr = final_map.begin(); itr != final_map.end(); itr++)
            {
                if(i >25)
                    break;
                i++;
                std::cout<<itr->second<<"  -  "<<itr->first<<std::endl; // print the output to the terminal
            }
        }
    private:
        std::map<std::string, int> word_freqs;
};
int main(int argc, char * argv[])
{

    WordFrequencyFramework wfapp;
    StopWordFilter stop_word_filter(wfapp);
    DataStorage data_storage(wfapp, stop_word_filter);
    WordFrequencyCounter word_freq_counter(wfapp, data_storage);
    wfapp.run(argv[1]);
    return 0;
}