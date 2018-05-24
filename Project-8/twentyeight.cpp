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

void run(){}
class example: public std::thread
{
    public:
        example()
        {
            std::thread();
        }
        void operations()
        {
            std::cout<<"Hello"<<std::endl;
            std::cout<<"world"<<std::endl;
            std::cout<<"this"<<std::endl;
            std::cout<<"works"<<std::endl;
        }
        void join_thread()
        {
            if(joinable())
                join();
        }
};
struct communication_object
{
    public:
        communication_object(void * r, void * s, void * w, std::vector<std::string> m)
        {
            reciepient = r;
            stopwordsobj = s;
            wordfreqobj = w;
            messages = m;
        }
        communication_object & operator =(const communication_object& another)
        {
            reciepient = another.reciepient;
            stopwordsobj = another.stopwordsobj;
            wordfreqobj = another.wordfreqobj;
            messages = another.messages;
        }
        void * reciepient = nullptr;
        void * stopwordsobj = nullptr;
        void * wordfreqobj = nullptr;
        std::vector<std::string> messages;
};

class ActiveWordFrequencyObject 
{
    public:
        ActiveWordFrequencyObject(){
            
        }
        virtual void run() = 0;
        virtual void dispatch(communication_object & c) = 0;
        virtual void join_thread() = 0;
        virtual void print_data() = 0;
        virtual void put_in_queue(communication_object & c) = 0;

};
void send(ActiveWordFrequencyObject * reciever, communication_object &message)
{
    reciever->put_in_queue(message);
}
class DataStorageManager : public ActiveWordFrequencyObject
{
    public:
        DataStorageManager(): ActiveWordFrequencyObject(){
            stop = false;
            runtime_thread = new std::thread(&DataStorageManager::run, this); // creating a thread here which executes the run method.
        }
        void run() 
        {
            while(!stop)
            {
                if(message_queue.empty()) // if no message is present for processing, simply dont execute the statements below (wait for a message to pop up)
                    continue;
                proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                communication_object c = message_queue.front(); // get the first arrived message
                message_queue.pop(); // remove it from processing it.
                proc_lock.unlock(); // unlock for others to mutate the queue.
                dispatch(c); // pass the message forward.
                if(c.messages[0] == "die")
                    stop = true;
                
            }
        }
        void dispatch (communication_object&c) 
        {
            if(c.messages[0] == "init")
            {
                init(c);
            }
        }
        void print_data()
        {
            for(auto E: data)
            {
                std::cout<<E<<std::endl;
            }
        }
        void put_in_queue(communication_object &c)
        {
            std::cout<<"message recieved"<<std::endl;
            proc_lock.lock();
            message_queue.push(c);
            proc_lock.unlock();
        }
        void init(communication_object &c)
        {
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
            std::cout<<"CALLED"<<std::endl;
            data = extract_words(c.messages[1]);

        }
        
        void join_thread()
        {
            if(runtime_thread->joinable())
                runtime_thread->join();
        }
    
    private:
        std::vector<std::string> data;
        std::queue<communication_object> message_queue; // queue of messages.
        std::mutex proc_lock; // process lock for implementing thread safe queues.
        std::thread* runtime_thread; // main thread that runs the entire processes.
        bool stop;

};
int main(int argc, char const *argv[])
{

    ActiveWordFrequencyObject * x = new DataStorageManager();
    std::vector<std::string> message_string;
    message_string.push_back("init");
    message_string.push_back(argv[1]);
    
    communication_object c(nullptr, nullptr, nullptr, message_string);
    send(x, c);
    message_string.clear();
    message_string.push_back("die");
    communication_object d(nullptr, nullptr, nullptr, message_string);
    send(x, d);
       
    x->join_thread();
    x->print_data(); 
    return 0;
}
