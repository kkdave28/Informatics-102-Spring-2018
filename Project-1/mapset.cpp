#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <utility>
#include <iterator>
#include <functional>
#include <algorithm>
using namespace std;

string convert_to_lower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string put_in_map(string s, set<string> wordset)
{
    if(wordset.find(s) == wordset.end())
        return s;
    else
        return "\0";
}

int main()
{
    set<string> word_exclusion;
    ifstream in("stopwords.txt");
    ifstream in2("sample_doc.txt");
	ofstream out("frequency.txt");
    for_each(istream_iterator<string>(in), istream_iterator<string>(), [&] (string s) {word_exclusion.insert(convert_to_lower(s));});
    map<string,int> map1;
    for_each(istream_iterator<string>(in2), istream_iterator<string>(), [&] (string s) {map1[put_in_map(convert_to_lower(s),word_exclusion)] +=1;});
    map1.erase(string("\t"));
    map1.erase(string("\r"));
    map1.erase(string(" "));
    map1.erase(string("\n"));
    map1.erase(string("\0"));
    for_each(map1.begin(), map1.end(), [&] (pair<string,int> pair1){out<<pair1.first<<"   :   "<<pair1.second<<endl;});
    return 0;
}
