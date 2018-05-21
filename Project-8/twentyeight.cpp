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
#include <deque>
#include <queue>
#include <thread>
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
struct communication_object;
class BaseClass: public std::thread
{
    public:
        BaseClass(){}
        virtual void run() = 0;
        virtual void dispatch(communication_object& c) = 0;
        virtual void join() = 0;
        virtual void put(std::pair<BaseClass*, std::vector<std::string>>) = 0;
        std::deque<communication_object> message_queue;


};

struct communication_object
{

    public:
        communication_object(BaseClass * newinstance = nullptr, BaseClass* newreciever = nullptr, BaseClass* newcinstance = nullptr,std::vector<std::string> msg = std::vector<std::string>())
        {
            instance = newinstance;
            reciever = newreciever;
            current_instance = newcinstance;
            messages = msg;
        }
        communication_object& operator=(const communication_object& another)
        {
            instance = another.instance;
            reciever = another.reciever;
            messages = another.messages;
            current_instance = another.current_instance;
            return *this;
        }
        BaseClass * instance;
        BaseClass * reciever;
        BaseClass* current_instance;
        std::vector<std::string> messages;
        
};
void send(BaseClass* instance, communication_object & c)
{
    c.current_instance = instance;
    instance->message_queue.push_back(c);
}
class ActiveWordFrequencyObject : public BaseClass
{
    public:
        ActiveWordFrequencyObject()
        {}
        ActiveWordFrequencyObject(int x)
        {
            class_name = __func__;
            stop = false;
            runtime_thread = new std::thread(&ActiveWordFrequencyObject::run, this);
        }
        void join()
        {
            if(runtime_thread->joinable())
            {
                runtime_thread->join();
            }
        }
        void put(communication_object & c)
        {
            message_queue.push_back(c);
        }
    std::deque<communication_object> message_queue;
    private:
        std::string class_name;
        
        bool stop;
        std::thread* runtime_thread;
        void run()
        {
            while(!stop)
            {
                communication_object msg = message_queue.front();
                message_queue.pop_front();
                msg.current_instance->dispatch(msg);
                if(msg.messages[0] == "die")
                {
                    stop = true;
                }       
            }
        }

};
class DataStorageManager: public ActiveWordFrequencyObject
{
    public:
        DataStorageManager(): ActiveWordFrequencyObject(){}
        

};
// class DataStorageManager: public ActiveWordFrequencyObject
// {
//     public:
//         DataStorageManager(): ActiveWordFrequencyObject(){}
//         void dispatch(std::pair<BaseClass*, std::vector<std::string>> message)
//         {
//             if(message.second[0] == "init")
//             {
//                 path_to_file = message.second[1];
//                 stop_words_manager = message.first;
//                 std::function<std::vector<std::string>(std::string)> extract_words = [] (std::string filename) -> std::vector<std::string>
//                 {
//                         std::ifstream PandP(filename);
//                         std::ifstream StopWordsStream("../stop_words.txt");
//                         const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
//                         std::string stop_words;
//                         std::vector<std::string> StopWords;
//                         std::getline(StopWordsStream,stop_words);
//                         split(stop_words, ",",StopWords);
//                         std::string buf;
//                         std::vector<std::string> data;
//                         while(PandP && std::getline(PandP, buf)) // processing the iput file
//                         {
//                             if(buf.length() == 0) // if the line read is an empty line
//                             {
//                                 continue;
//                             }
//                             else
//                             {
//                                 removeCharsFromString(buf, EscapeChars); //remove escape chars from the string
//                                 buf = convert_to_lower(buf); // convert the entire string to lowercase
//                                 std::istringstream iss(buf); // string as a stream of characters
//                                 std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(data)); // same as a for loop, but captures all the string and puts it into a vector of strings.
//                                 while(std::find(data.begin(),data.end()," ") != data.end()) // find the extra space charaters in the vector 
//                                 {
//                                     std::vector<std::string>::iterator iter;
//                                     iter = find(data.begin(),data.end()," ");
//                                     data.erase(iter); // remove them from the vector
//                                 }
//                             }
//                         }
//                         return data;
//                 };
//                 list_of_words = extract_words(path_to_file);

//             }

            
//         }

//     private:
//         std::string path_to_file;
//         BaseClass * stop_words_manager;
//         std::vector<std::string> list_of_words;

// };  
int main(int argc, char *argv[])
{
    return 0;
}
