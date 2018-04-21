#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
static std::string null_string = "";
void no_func(std::pair<std::string, std::string> event = std::pair<std::string, std::string>("","")){}
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
class EventManager
{
    public:
        EventManager(){}
        void subscribe(std::string event_type, std::function<void(std::pair<std::string, std::string>, std::string&)> handler)
        {
            subs[event_type].push_back(handler);
        }
        void publish(std::pair<std::string, std::string> event, std::string& buf=null_string)
        {
            for(auto E: subs[event.first])
            {
                E(event,buf);
            }
        }
    private:
        std::map<std::string, std::vector<std::function<void(std::pair<std::string, std::string>, std::string&)>>> subs;
};

class DataStorage
{
    public: 
        DataStorage(EventManager * event_manager)
        {
            using namespace std::placeholders;
            em = event_manager;
            em->subscribe("load", std::bind(&DataStorage::load, this, _1, _2));
            em->subscribe("start", std::bind(&DataStorage::produce_words, this, _1, _2));
        }
        void load(std::pair<std::string, std::string> event, std::string& temp=null_string)
        {
            std::ifstream PandP(event.second);
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
        void produce_words(std::pair<std::string, std::string> event, std::string& buf=null_string)
        {
            for(auto E: data)
            {
                em->publish(std::pair<std::string, std::string>("word", E));
            }
            em->publish(std::pair<std::string, std::string>("eof", ""));
        }
    private:
        EventManager * em;
        std::vector<std::string> data;
};
class StopWordsFilter
{
    public:
        StopWordsFilter(EventManager * event_manager)
        {
            using namespace std::placeholders;
            em = event_manager;
            em->subscribe("load", std::bind(&StopWordsFilter::load, this, _1, _2));
            em->subscribe("word", std::bind(&StopWordsFilter::is_stop_word, this, _1, _2));

        }
        void load(std::pair<std::string, std::string> event = std::pair<std::string, std::string>("",""), std::string& buf=null_string)
        {
            std::ifstream StopWordsStream("../stop_words.txt");
            std::string stop_words_string;
            std::getline(StopWordsStream, stop_words_string);
            stop_words_string = convert_to_lower(stop_words_string);
            split(stop_words_string, ",", stop_words);
        }
        void is_stop_word(std::pair<std::string, std::string> event, std::string &buf)
        {
            buf = event.second;            
            for(auto E: stop_words)
            {
                if(E == event.second)
                {
                    buf = "";
                    return;
                }
            }
            em->publish(std::pair<std::string, std::string>("valid_word", event.second));
        }
    private:
        std::vector<std::string> stop_words;
        EventManager * em;

};
class WordFrequencyCounter
{
    public:
        WordFrequencyCounter(EventManager* event_manager)
        {
            using namespace std::placeholders;
            em = event_manager;
            em->subscribe("valid_word", std::bind(&WordFrequencyCounter::increment_count, this, _1, _2));
            em->subscribe("print", std::bind(&WordFrequencyCounter::print_freqs, this, _1, _2));

        }
        void increment_count(std::pair<std::string, std::string> event, std::string& buf=null_string)
        {
            word_freqs[event.second]+=1;
        }
        void print_freqs(std::pair<std::string, std::string> event = std::pair<std::string, std::string>("",""), std::string& buf=null_string)
        {
            word_freqs.erase("");
            word_freqs.erase("s");
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
        EventManager* em;

};
class WordFrequencyApp
{
    public:
        WordFrequencyApp(EventManager* event_manager)
        {
            using namespace std::placeholders;
            em = event_manager;
            em->subscribe("run", std::bind(&WordFrequencyApp::run, this, _1, _2));
            em->subscribe("eof", std::bind(&WordFrequencyApp::stop, this, _1, _2));
        }
        void run(std::pair<std::string, std::string> event, std::string& buf=null_string)
        {
            em->publish(std::pair<std::string, std::string>("load", event.second));
            em->publish(std::pair<std::string, std::string>("start", ""));

        }
        void stop(std::pair<std::string, std::string> event, std::string& buf=null_string)
        {
            em->publish(std::pair<std::string, std::string>("print", ""));
        }
    private:
        EventManager * em;
};
int main(int argc, char * argv[])
{
    EventManager em;
    DataStorage dso(&em);
    StopWordsFilter swf(&em);
    WordFrequencyCounter wfc(&em);
    WordFrequencyApp wfapp(&em);
    em.publish(std::pair<std::string, std::string>("run", argv[1]));
    return 0;
}