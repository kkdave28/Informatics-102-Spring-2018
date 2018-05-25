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
#define DEBUG 0
class WordFreqException{
    public:
        WordFreqException(std::string error_message)
        {
            std::cerr<<"Word Freq Exception: "<<error_message<<std::endl;
        }
};
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
typedef std::vector<void *> communication_object;
// works with std::vector of void pointers.

class ActiveWordFrequencyObject 
{
    public:
        ActiveWordFrequencyObject(){
            
        }
        virtual void run() = 0;
        virtual void dispatch(communication_object  c) = 0;
        virtual void join_thread() = 0;
        virtual void print_data() = 0;
        virtual void put_in_queue(communication_object  c) = 0;

};
void send(ActiveWordFrequencyObject * reciever, communication_object message)
{
    reciever->put_in_queue(message);
}
class DataStorageManager : public ActiveWordFrequencyObject
{
    public:
        DataStorageManager(): ActiveWordFrequencyObject(){
            stop = false;
            runtime_thread = new std::thread(&DataStorageManager::run, this); // creating a thread here which executes the run method.
            stopwords = nullptr;
        }
        void run() 
        {
            while(!stop)
            {
                proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                if(message_queue.empty()) // if no message is present for processing, simply dont execute the statements below (wait for a message to pop up)
                {
                    proc_lock.unlock();    
                    continue;
                }
                //proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                communication_object c = message_queue.front(); // get the first arrived message
                message_queue.pop(); // remove it from processing it.
                proc_lock.unlock(); // unlock for others to mutate the queue.
                dispatch(c); // pass the message forward.
                if(*static_cast<std::string*>(c[0]) == "die")
                    stop = true;
                
            }
        }
        void dispatch (communication_object c) 
        {
            if(*static_cast<std::string*>(c[0]) == "init")
            {
                init(std::vector<void*>(c.begin()+1, c.end()));
            }
            else if(*static_cast<std::string*>(c[0]) == "send_word_freqs")
            {
                process_words(std::vector<void*>(c.begin()+1, c.end()));
            }
            else
            {
                send(stopwords, c);
            }

        }
        void process_words(communication_object c)
        {   
            ActiveWordFrequencyObject * recipient = reinterpret_cast<ActiveWordFrequencyObject*>(c[0]);
            for(auto words: data)
            {
                send(stopwords, std::vector<void*>{new std::string("filter"), new std::string(words)});
            }
            send(stopwords, std::vector<void*>{new std::string("top25"), recipient});
        }
        void print_data()
        {
            for(auto E: data)
            {
                std::cout<<E<<std::endl;
            }
        }
        void put_in_queue(communication_object c)
        {
            #if DEBUG
            std::cout<<"message recieved"<<std::endl;
            #endif
            proc_lock.lock();
            message_queue.push(c);
            proc_lock.unlock();
        }
        void init(communication_object c)
        {
            stopwords = static_cast<ActiveWordFrequencyObject*>(c[1]);
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
            #if DEBUG
            std::cout<<"CALLED"<<std::endl;
            #endif
            data = extract_words(*static_cast<std::string*>(c[0]));

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
        ActiveWordFrequencyObject * stopwords;

};

class StopWordsManager : public ActiveWordFrequencyObject
{
    public:
        StopWordsManager() : ActiveWordFrequencyObject() 
        {
            stop = false;
            runtime_thread = new std::thread(&StopWordsManager::run, this);
        }
        void run() 
        {
            while(!stop)
            {
                proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                if(message_queue.empty()) // if no message is present for processing, simply dont execute the statements below (wait for a message to pop up)
                {
                    proc_lock.unlock();    
                    continue;
                }
                //proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                communication_object c = message_queue.front(); // get the first arrived message
                message_queue.pop(); // remove it from processing it.
                proc_lock.unlock(); // unlock for others to mutate the queue.
                dispatch(c); // pass the message forward.
                if(*static_cast<std::string*>(c[0]) == "die")
                    stop = true;

            }
        }
        void dispatch(communication_object c)
        {
            if(*static_cast<std::string*>(c[0]) == "init")
            {
                init(std::vector<void*>(c.begin()+1, c.end()));
            }
            else if(*static_cast<std::string*>(c[0]) == "filter")
            {
                filter(std::vector<void*>(c.begin()+1, c.end()));
            }
            else // implement this after having created word freq manager.
            {
                 send(word_freq_manager, c);
            }
        }
        void init(communication_object c)
        {
            word_freq_manager = static_cast<ActiveWordFrequencyObject*>(c[0]);
            std::ifstream StopWordsStream("../stop_words.txt");
            std::string stop_words_string;
            std::getline(StopWordsStream, stop_words_string);
            stop_words_string = convert_to_lower(stop_words_string);
            split(stop_words_string, ",", stop_words);
            
        }
        void join_thread()
        {
            if(runtime_thread->joinable())
                runtime_thread->join();
        }
        void print_data()
        {
            for(auto E: filtered_words)
            {
                std::cout<<E<<std::endl;
            }
        }
        void put_in_queue(communication_object c)
        {
            proc_lock.lock();
            message_queue.push(c);
            proc_lock.unlock();
        }
        void filter(communication_object c)
        {
            if(!is_stop_word(*static_cast<std::string*>(c[0]), stop_words))
            {
                filtered_words.push_back(*static_cast<std::string*>(c[0]));
                send(word_freq_manager, std::vector<void*>{new std::string("word"), new std::string(*static_cast<std::string*>(c[0]))});
            }

        }
    private:
        std::vector<std::string> stop_words;
        std::queue<communication_object> message_queue; // queue of messages.
        std::mutex proc_lock; // process lock for implementing thread safe queues.
        std::thread* runtime_thread; // main thread that runs the entire processes.
        bool stop;
        ActiveWordFrequencyObject * word_freq_manager;
        std::vector<std::string> filtered_words;
};
class WordFrequencyManager : public ActiveWordFrequencyObject
{
    public:
        WordFrequencyManager() : ActiveWordFrequencyObject()
        {
            stop = false;
            runtime_thread = new std::thread(&WordFrequencyManager::run, this);
        }
        void dispatch(communication_object c)
        {
            if(*static_cast<std::string*>(c[0]) == "word")
            {
                increment(std::vector<void*>(c.begin()+1, c.end()));
            }
            else if(*static_cast<std::string*>(c[0]) == "top25")
            {
                top25(std::vector<void*>(c.begin()+1, c.end()));
            }
        }
        void top25(communication_object c)
        {
            ActiveWordFrequencyObject * recipient = static_cast<ActiveWordFrequencyObject*>(c[0]);
            send(recipient, std::vector<void*> {new std::string("top25"),new std::vector<std::pair<std::string, int>>(sort_map())});
        }
        std::vector<std::pair<std::string, int>> sort_map()
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
        void increment(communication_object c)
        {
            word_freqs[*static_cast<std::string*>(c[0])] +=1;
        }
        void put_in_queue(communication_object c)
        {
            proc_lock.lock();
            message_queue.push(c);
            proc_lock.unlock();
        }
        void join_thread()
        {
            if(runtime_thread->joinable())
                runtime_thread->join();
        }
        void run()
        {
            while(!stop)
            {
                proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                if(message_queue.empty()) // if no message is present for processing, simply dont execute the statements below (wait for a message to pop up)
                {
                    proc_lock.unlock();    
                    continue;
                }
                //proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                communication_object c = message_queue.front(); // get the first arrived message
                message_queue.pop(); // remove it from processing it.
                proc_lock.unlock(); // unlock for others to mutate the queue.
                dispatch(c); // pass the message forward.
                if(*static_cast<std::string*>(c[0]) == "die")
                    stop = true;

            }
        }
        void print_data(){}
    private:
        std::map<std::string, int> word_freqs;
        std::queue<communication_object> message_queue;
        std::mutex proc_lock;
        std::thread * runtime_thread;
        bool stop;
};
class WordFrequencyController : public ActiveWordFrequencyObject
{
    public:
        WordFrequencyController() : ActiveWordFrequencyObject()
        {
            stop = false;
            runtime_thread = new std::thread(&WordFrequencyController::run, this);
        }
        void run()
        {
            while(!stop)
            {
                proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                if(message_queue.empty()) // if no message is present for processing, simply dont execute the statements below (wait for a message to pop up)
                {
                    proc_lock.unlock();    
                    continue;
                }
                //proc_lock.lock(); // Thread safe queues implemetation by using mutex which ensures that no simultaneous push/pop takes place.
                communication_object c = message_queue.front(); // get the first arrived message
                message_queue.pop(); // remove it from processing it.
                proc_lock.unlock(); // unlock for others to mutate the queue.
                dispatch(c); // pass the message forward.
                if(*static_cast<std::string*>(c[0]) == "die")
                    stop = true;

            }
        }
        void dispatch(communication_object c)
        {
            if(*static_cast<std::string*>(c[0]) == "run")
            {
                begin_processing(std::vector<void*>(c.begin()+1, c.end()));
            }
            else if(*static_cast<std::string*>(c[0]) == "top25")
            {
                fin_map = *static_cast<std::vector<std::pair<std::string,int>>*>(c[1]);
                print_data();
            }
            else
            {
                throw(WordFreqException("Message not understood"));
            }
        }
        void begin_processing(communication_object c) 
        {
            storage_manager = static_cast<ActiveWordFrequencyObject*>(c[0]);
            send(storage_manager, std::vector<void *>{new std::string("send_word_freqs"), this});
        }
        void join_thread()
        {
            if(runtime_thread->joinable())
                runtime_thread->join();
        }
        void print_data() 
        {
            for(auto E: fin_map)
            {
                std::cout<<E.first<<"  -  "<<E.second<<std::endl;
            }
            send(storage_manager, std::vector<void*>{new std::string("die")});
            stop = true;
        }
        void put_in_queue(communication_object c)
        {
            proc_lock.lock();
            message_queue.push(c);
            proc_lock.unlock();
        }
    private:
        std::queue<communication_object> message_queue;
        std::mutex proc_lock;
        std::thread * runtime_thread;
        std::vector<std::pair<std::string,int>> fin_map;
        ActiveWordFrequencyObject * storage_manager;
        bool stop;      
};
int main(int argc, char const *argv[])
{

    ActiveWordFrequencyObject * word_freq_manager = new WordFrequencyManager();
    ActiveWordFrequencyObject * stop_words_manager = new StopWordsManager();
    send(stop_words_manager, std::vector<void*>{new std::string("init"), word_freq_manager});

    ActiveWordFrequencyObject * storage_manager = new DataStorageManager();
    send(storage_manager, std::vector<void*>{new std::string("init"), new std::string(argv[1]), stop_words_manager});
    
    ActiveWordFrequencyObject * word_freq_controller = new WordFrequencyController();
    send(word_freq_controller, std::vector<void*>{new std::string("run"), storage_manager});

    word_freq_controller->join_thread();
    stop_words_manager->join_thread();
    storage_manager->join_thread();
    word_freq_controller->join_thread();
    return 0;
}
