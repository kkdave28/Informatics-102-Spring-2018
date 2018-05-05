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
#include <stdexcept>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

std::map<std::string, std::string> options; // parses input from ini file and stores the data.
void * term_words = nullptr; // function pointer
void * term_freq = nullptr; // function pointer
//Get pull request before submitting for boost libraries.
void parse(std::istream & cfgfile)
{
    for (std::string line; std::getline(cfgfile, line); )
    {
        std::istringstream iss(line);
        std::string id, eq, val;

        bool error = false;

        if (!(iss >> id))
        {
            error = true;
        }
        else if (id[0] == '#')
        {
            continue;
        }
        else if (!(iss >> eq >> val >> std::ws) || eq != "=" || iss.get() != EOF)
        {
            error = true;
        }

        if (error)
        {
            continue;
            // do something appropriate: throw, skip, warn, etc.
        }
        else
        {
            options[id] = val;
        }
    }
}
void load_plugins()
{
    void * word_handle = dlopen(options["words"].c_str(), RTLD_LAZY);
    void * freq_handle = dlopen(options["frequencies"].c_str(), RTLD_LAZY);
    term_words = dlsym(word_handle, "extract_words");
    term_freq = dlsym(freq_handle, "top25_freqs");
}

int main(int argc, char *argv[])
{

    std::ifstream config_file("config.ini");
    parse(config_file);
    load_plugins();
    // for(auto E: options)
    // {
    //     std::cout<<E.first <<" = "<<E.second<<std::endl;
    // }

    std::vector<std::pair<std::string,int>> fin_map;
    fin_map = reinterpret_cast<std::vector<std::pair<std::string,int>> (*) (std::vector<std::string>)>(term_freq)(reinterpret_cast<std::vector<std::string> (*) (std::string)> (term_words)(argv[1]));
    for(auto E: fin_map)
    {
        std::cout<<E.first<<"  -  "<<E.second<<std::endl;
    }
    return 0;
}