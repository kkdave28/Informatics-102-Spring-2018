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
void sort_map(std::map<std::string, void*> WordFrequencyObject, std::multimap<int, std::string, std::greater<int>>*final_map)
{
    (*reinterpret_cast<std::map<std::string, int>*>(WordFrequencyObject["freqs"])).erase("");
    (*reinterpret_cast<std::map<std::string, int>*>(WordFrequencyObject["freqs"])).erase("s");
    for(auto E:(*reinterpret_cast<std::map<std::string, int>*>(WordFrequencyObject["freqs"])))
    {
        (*final_map).insert(std::pair<int, std::string> (E.second, E.first));
    }

}
void increment_freq_count(std::string word,std::map<std::string, void*> WordFrequencyObject)
{
    (*reinterpret_cast<std::map<std::string, int>*>(WordFrequencyObject["freqs"]))[word]+=1;
}
extern "C" std::vector<std::pair<std::string, int>> top25_freqs(std::vector<std::string> data)
{
    std::map<std::string, int> temp_freq;
    std::multimap<int, std::string, std::greater<int>> final_map;
    std::map<std::string, void*> WordFrequencyObject;

    std::vector<std::pair<std::string, int>> ret_data;
    
    WordFrequencyObject["freqs"] = (void *)&temp_freq;
    WordFrequencyObject["increment_count"] = (void *)increment_freq_count;
    WordFrequencyObject["sorted"] = (void*) sort_map;

    for(auto E: data)
    {
        reinterpret_cast<void (*)(std::string, std::map<std::string, void*>)>(WordFrequencyObject["increment_count"]) (E, WordFrequencyObject);
    }
    reinterpret_cast<void(*)(std::map<std::string, void*>, std::multimap<int, std::string, std::greater<int>>*)>(WordFrequencyObject["sorted"])(WordFrequencyObject, &final_map);

    std::multimap<int, std::string>:: iterator itr;
    int i = 1;
    for(itr = final_map.begin(); itr != final_map.end(); itr++)
    {
        if(i >25)
            break;
        ret_data.push_back(std::pair<std::string,int>(itr->second,itr->first));
        i++;
    }
    if(debug)
    {
        std::cout<<"Printing from freq2"<<std::endl;
    }
    return ret_data;

}