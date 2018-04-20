#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
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

void extract_words(std::string filename, std::map<std::string, void*> DataStorageObject)
{
    std::ifstream PandP(filename);
    std::string buf;
    const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
    
    reinterpret_cast<std::vector<std::string>*>(DataStorageObject["data"])->push_back("Hello World");

}
int main(int argc, char * argv[])
{   
    std::map<std::string, void*> DataStorageObject;
    std::vector<std::string> dsodata;
    DataStorageObject["init"] = (void*)extract_words;
    DataStorageObject["data"] = (void*)&dsodata;
    DataStorageObject["words"] = (void*)DataStorageObject["data"];

   reinterpret_cast<void (*) (std::string, std::map<std::string, void*>)>(DataStorageObject["init"]) (argv[1], DataStorageObject);
   for(int i=0; i< dsodata.size(); i++)
   {
       std::cout<<dsodata[i]<<std::endl;
   }
    return 0;
}