#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    std::ifstream file1(argv[1]);
    std::ifstream file2(argv[2]);

    std::string one,two;
    while(1)
    {
        if(file1.eof() && file2.eof())
        {
            break;
        }
        if(file1.eof() && !file2.eof() || !file1.eof() && file2.eof()){
            std::cout<<"Both files are NOT equal\n";
            return 0;
        }
        std::getline(file1, one);
        std::getline(file2,two);
        if(one != two)
        {
            std::cout<<"Both files are NOT equal\n";
            return 0;
        }

    }
    std::cout<<"Both files are equal\n";
    return 0;
}