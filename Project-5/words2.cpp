#ifndef WORDS2_CPP
#define WORDS2_CPP
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
bool debug = false;
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
void obtain_words(std::string filename, std::map<std::string, void*> DataStorageObject)
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
            std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(*reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"]))); // same as a for loop, but captures all the string and puts it into a vector of strings.
            while(std::find(reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->begin(),reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->end()," ") != reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->end()) // find the extra space charaters in the vector 
            {
                std::vector<std::string>::iterator iter;
                iter = find(reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->begin(),reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->end()," ");
                reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->erase(iter); // remove them from the vector
            }
        }
    }
    // reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->push_back("Hello World ");
    // reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->push_back("This is great!!!");
    // reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->push_back("I totally love c++");

}
void load_stop_words(std::map<std::string, void*> StopWordsObject)
{
    std::ifstream StopWordsStream("../stop_words.txt");
    std::string stop_words;
    std::getline(StopWordsStream, stop_words);
    stop_words = convert_to_lower(stop_words);
    split(stop_words, ",", (*reinterpret_cast<std::vector<std::string>*>(StopWordsObject["stop_words"])));
}
void check_stop_word(std::string word, std::map<std::string, void*> StopWordsObject, std::string& buf)
{
    for(int i=0; i< (*reinterpret_cast<std::vector<std::string>*>(StopWordsObject["stop_words"])).size(); i++)
    {
       if ( word == (*reinterpret_cast<std::vector<std::string>*>(StopWordsObject["stop_words"]))[i])
       {
           buf = "";
           return;
       }
    }
   buf = word;
}
extern "C" std::vector<std::string> extract_words(std::string filename)
{
    std::vector<std::string> dsodata;
    std::vector<std::string> stop_words;
    std::vector<std::string> data;

    
    std::map<std::string, void*> DataStorageObject;
    std::map<std::string, void*> StopWordsObject;


    DataStorageObject["init"] = (void*)obtain_words;
    DataStorageObject["data"] = (void*)&dsodata;
    DataStorageObject["words"] = (void*)DataStorageObject["data"];

    StopWordsObject["stop_words"] = (void*)&stop_words;
    StopWordsObject["init"] = (void*)load_stop_words;
    StopWordsObject["is_stop_word"] = (void*)check_stop_word;

    reinterpret_cast<void (*) (std::string, std::map<std::string, void*>)>(DataStorageObject["init"]) (filename, DataStorageObject);
    reinterpret_cast<void(*) (std::map<std::string, void*>)>(StopWordsObject["init"])(StopWordsObject);

    for(auto E: *reinterpret_cast<std::vector<std::string>*>(DataStorageObject["words"]))
    {
       std::string buf = "";
       reinterpret_cast<void(*)(std::string, std::map<std::string, void*>, std::string&)>(StopWordsObject["is_stop_word"]) (E, StopWordsObject, buf);
       data.push_back(buf);
    }
    
    if(debug)
    {
        std::cout<<"Printing from words2"<<std::endl;
    }
    return data;


}
#endif