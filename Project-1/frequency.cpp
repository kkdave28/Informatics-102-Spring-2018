/*
Design ideas:
    1.  Use map to store <word,frequency> pairs sorted w.r.t the decreasing 
        order of frequency.
*/
#include <fstream>
#include <iostream>
#include <map> // storing the pairs of (word, frequency).
#include <set> // storing the stopwords.
#include <utility>
#include <iterator>
#include <functional>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector> // string processing purposes
#include <sstream>

void split(const std::string& str, const std::string& delim, std::set<std::string> & tokens)
{
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.insert(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
}
void split2(const std::string &s, const char* delim, std::set<std::string> & v){
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, delim);
    while(token != NULL){
        v.insert(std::string(token));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token = strtok(NULL, delim);
    }
    free(dup);
}
std::string put_in_map(std::string s, const std::set<std::string>& StopWords)
{
    if(StopWords.find(s) == StopWords.end())
        return s;
    else
        return "\0";
}
void removeCharsFromString( std::string &str, const char* charsToRemove ) {
   for ( unsigned int i = 0; i < std::strlen(charsToRemove); ++i ) {
    while(
        str.find(std::string(1,charsToRemove[i])) != std::string::npos
        )
    {
        size_t f = str.find(std::string(1,charsToRemove[i]));
        str.replace(f, 1, " ");
      //str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
   }
}
std::string convert_to_lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
int main()
{
    std::set<std::string> StopWords;
    std::map<std::string, int> FrequencyMap;
    std::vector<std::string> wordline;
    std::ifstream StopWordsStream("stop_words.txt");
    std::ifstream PandP("pride-and-prejudice.txt");
    std::ofstream OutputFile("frequency.txt");
    std::ofstream op("such.txt");
    std::ofstream op2("such2.txt");
    const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
    std::string stop_words;
    std::getline(StopWordsStream,stop_words);
    split(stop_words, ",",StopWords);
    std::string buf;
    
    while(PandP && std::getline(PandP, buf))
    {
        if(buf.length() == 0)
        {
            continue;
        }
        else
        {
            removeCharsFromString(buf, EscapeChars);
            buf = convert_to_lower(buf);
            std::istringstream iss(buf);
                
            std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(wordline));
                    //remove_extra_whitespaces(buf2, buf);
            while(std::find(wordline.begin(),wordline.end()," ") != wordline.end())
            {
                std::vector<std::string>::iterator iter;
                iter = find(wordline.begin(),wordline.end()," ");
                wordline.erase(iter);
            }
                    for(auto E: wordline)
                    {
                        FrequencyMap[put_in_map( E, StopWords)] +=1;
                    }                        
                }
                wordline.clear();
    }
    // for_each(std::istream_iterator<std::string>(PandP), std::istream_iterator<std::string>(), [&] (std::string s) 
    // {

    //     FrequencyMap[put_in_map(s,StopWords)] +=1;
    // });

    for(auto E: FrequencyMap)
    {
        OutputFile<<E.first <<" -> "<<E.second<<"\n";
    }
    // FrequencyMap.erase(std::string("\t"));
    // FrequencyMap.erase(std::string("\r"));
    // FrequencyMap.erase(std::string(" "));
    // FrequencyMap.erase(std::string("\n"));
    // FrequencyMap.erase(std::string("\0"));

/* Debug Purposes

    for(auto E: StopWords)
    {
        std::cout<<E<<std::endl;
    }
*/

    return 0;
}