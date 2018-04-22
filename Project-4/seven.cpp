#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#define RECUSION_LIMIT 2000
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
bool is_stop_word(std::string &word, std::vector<std::string> *stop_words)
{
    for(auto E: (*stop_words))
    {
        if(word == E)
        {
            return true;
        }
    }
    return false;
}

void sort(std::map<std::string, int> *word_freq, std::multimap<int, std::string, std::greater<int>> *final_map)
{
    word_freq->erase("");
    word_freq->erase("s");
    for(auto E: *(word_freq))
    {
        final_map->insert(std::pair<int, std::string> (E.second, E.first));
    }

}
void print_freqs(std::multimap<int, std::string, std::greater<int>> * final_map, int & count)
{
    if(count > 24)
    {
        return;
    }
    else
    {
        std::multimap<int, std::string>:: iterator itr; // iterator for the multimap
        int i = 1; // counter to check the number of pairs printed
        itr = std::next(final_map->begin(), count);
    
        std::cout<<itr->second<<"  -  "<<itr->first<<std::endl; // print the output to the terminal
        count +=1;
        print_freqs(final_map, count);
    }
}
void count(std::vector<std::string> word_list, std::vector<std::string> stop_words, std::map<std::string, int>* word_freqs)
{
    if(word_list.empty())
    {
        return;
    }
    else
    {
        if(!is_stop_word(word_list[0], &stop_words))
        {
            (*word_freqs)[word_list[0]]+=1;
        }
        count(std::vector<std::string>(word_list.begin()+1, word_list.end()), stop_words, word_freqs);
    }
}
int main(int argc, char * argv[])
{
    std::vector<std::string> data;
    std::vector<std::string> stop_word;

    std::map<std::string, int> word_freqs;
    std::multimap<int, std::string, std::greater<int>> final_map;

    std::ifstream StopWordsStream("../stop_words.txt");
    std::string stop_words_string;
    std::getline(StopWordsStream, stop_words_string);
    stop_words_string = convert_to_lower(stop_words_string);
    split(stop_words_string, ",", stop_word);


    std::ifstream PandP(argv[1]);
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
    size_t limit = 0;
    for(size_t i=0; i< data.size(); i+= RECUSION_LIMIT)
    {
        limit = std::min(i+RECUSION_LIMIT, data.size());
        count(std::vector<std::string>(data.begin()+i, data.begin()+i+RECUSION_LIMIT), stop_word, &word_freqs);
    
    }
    sort(&word_freqs, &final_map);
    int count =0;
    print_freqs(&final_map,count);

    return 0;
}