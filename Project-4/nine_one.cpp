#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

class custom_object
{
    public:
        custom_object(){}
        custom_object(const custom_object& co)
        {
            data = co.data;
            word_freqs = co.word_freqs;
            final_map = co.final_map;
            print_string = co.print_string;
        }
        custom_object& operator=(custom_object & co)
        {
            data = co.data;
            word_freqs = co.word_freqs;
            final_map = co.final_map;
            print_string = co.print_string;
            return *this;
        }
        std::vector<std::string> & get_data()
        {
            return data;
        }
        std::map<std::string, int> & get_word_freq()
        {
            return word_freqs;
        }
        std::multimap<int, std::string, std::greater<int>> & get_final_map()
        {
            return final_map;
        }
        std::string & get_print_string()
        {
            return print_string;
        }
    private:
        std::vector<std::string> data;
        std::map<std::string, int> word_freqs;
        std::multimap<int, std::string, std::greater<int>> final_map;
        std::string print_string;
};
custom_object stuff(custom_object c)
{
    c.get_data().push_back("Hello word!");
    return c;
}
int main()
{
    custom_object a;
    custom_object c = stuff(a);
    //c.get_data().push_back("Hello Word");
    for(auto E: c.get_data())
    {
        std::cout<<E<<std::endl;
    }
    return 0;
}