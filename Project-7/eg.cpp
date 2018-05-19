#include <iostream>
#include <string>
int main()
{
    std::string x = "HELLO WORLD";
    for(auto E: x)
    {
        std::cout<<E<<std::endl;
    }
    return 0;
}