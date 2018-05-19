#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
bool debug = false;
extern "C" std::vector<std::pair<std::string, int>> top25_freqs(std::vector<std::string> data)
{
    std::map<std::string, int> temp_map;
    std::multimap<int, std::string, std::greater<int>> interm_map;
    std::vector<std::pair<std::string, int>> fin_map;

    for(auto E: data)
    {
        temp_map[E]+=1;
    }
    temp_map.erase("s");
    temp_map.erase("");
    for(auto E: temp_map)
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
    if(debug)
    {
        std::cout<<"Printing from freq1"<<std::endl;
    }
    return fin_map;

}