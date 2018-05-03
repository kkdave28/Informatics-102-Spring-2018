#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

void * term_words = nullptr;
void * term_freq = nullptr;
//Get pull request before submitting for boost libraries.
void load_plugins()
{
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);
    
    void * word_handle = dlopen(pt.get<std::string>("Plugins.words").c_str(), RTLD_LAZY);
    void * freq_handle = dlopen(pt.get<std::string>("Plugins.frequencies").c_str(), RTLD_LAZY);
    term_words = dlsym(word_handle, "extract_words");
    term_freq = dlsym(freq_handle, "top25_freqs");
}

int main(int argc, char *argv[])
{

    load_plugins();

    std::vector<std::pair<std::string,int>> fin_map;
    fin_map = reinterpret_cast<std::vector<std::pair<std::string,int>> (*) (std::vector<std::string>)>(term_freq)(reinterpret_cast<std::vector<std::string> (*) (std::string)> (term_words)(argv[1]));
    for(auto E: fin_map)
    {
        std::cout<<E.first<<"  -  "<<E.second<<std::endl;
    }
    return 0;
}