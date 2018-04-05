/*
Name: Kush K Dave
Informatics 102 Spring 2018
Project-1
*/
#include <fstream> // file stream for reading/writing files
#include <iostream> // general purpose iostream for printing to terminal
#include <map> // storing the pairs of (word, frequency).
#include <set> // storing the stopwords.
#include <utility> // this package has general purpose algorithms for sorting STL containers
#include <iterator> // this package allows the user to iterate through STL containers
#include <functional> // this package allows the user to modify the STL containers
#include <algorithm> // this package allows the user to use lambda functions
#include <string> // this package allows user to use the multi-purpose and multifunctional c++ strings
#include <cstring> // this package allows the user to use lower level c-string functions like strcat, strcpy, strcmp...
#include <vector> // STL container for accumulating and manipulating data
#include <sstream> // string stream for efficient string processing

void split(const std::string& str, const std::string& delim, std::set<std::string> & tokens) // split functions allows the user to split the string w.r.t. the delimiter specified
{
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev); // find the position of delimiter and store it in 'pos'
        if (pos == std::string::npos) pos = str.length(); // delimiter not found in string at all or reached the end of the string
        std::string token = str.substr(prev, pos-prev); // split the string at the position of the delimiter
        if (!token.empty()) tokens.insert(token); // if the string is split and stored in the std::string token, add it to the std::set of strings.
        prev = pos + delim.length(); // start finding next delimiter starting from the last position where you found the delimiter.
    }
    while (pos < str.length() && prev < str.length()); // check the pos has not overshot the length of the string.
}

std::string put_in_map(std::string s, const std::set<std::string>& StopWords) // this function determines if the word should be put into the map or not.
{
    if(StopWords.find(s) == StopWords.end()) // if the std::string s passed is NOT in the set of StopWords, return that string.
        return s;
    else
        return "\0"; // the std::string s passed is IN the set of StopWords, return empty string.
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
int main()
{
    std::set<std::string> StopWords; // set of stopwords
    std::map<std::string, int> FrequencyMap; // primary map to store the word-frequency pairs
    std::vector<std::string> wordline; // vector of strings that contains all the individual words from a single line
    std::multimap<int, std::string, std::greater<int>> final_map; // secondary map to sort words according to the frequency
    std::ifstream StopWordsStream("stop_words.txt"); // input file stream generated from "stop_words.txt" file
    std::ifstream PandP("pride-and-prejudice.txt"); // input file stream generated from "pride-and-prejudice.txt" file
    std::ofstream OutputFile("kkdave_pride-and-prejudice-solution.txt"); // output filestream generated to redirect output to "kkdave-price-and-prejudice-solution.txt"
    const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0"; // string of all escape characters in the document specified
    std::string stop_words; // temporary string to store stopwords
    std::getline(StopWordsStream,stop_words);
    split(stop_words, ",",StopWords);
    std::string buf;
    
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
                
            std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(wordline)); // same as a for loop, but captures all the string and puts it into a vector of strings.
            while(std::find(wordline.begin(),wordline.end()," ") != wordline.end()) // find the extra space charaters in the vector 
            {
                std::vector<std::string>::iterator iter;
                iter = find(wordline.begin(),wordline.end()," ");
                wordline.erase(iter); // remove them from the vector
            }
                    for(auto E: wordline) // iterate over the vector and put them in the map
                    {
                        FrequencyMap[put_in_map( E, StopWords)] +=1; 
                    }                        
                }
                wordline.clear(); // remove all strings stored in vector for fresh processing of the new line
    }
    FrequencyMap.erase(""); // remove the entry pertaining to empty strings
    FrequencyMap.erase("s"); // for some reason 's' was redundant and would always appear as top 5 in the final map so have to remove explicitly
    for(auto E: FrequencyMap) // put the word-frequency pairs in the primary map as freequency-word pairs in the secondary map
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
        OutputFile<<itr->second<<"  -  "<<itr->first<<"\n"; // print the output to the file
        std::cout<<itr->second<<"  -  "<<itr->first<<std::endl; // print the output to the terminal
    }
    return 0;
}