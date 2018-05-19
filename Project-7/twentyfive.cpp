#include <sqlite3.h>
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <sstream>
#define DEBUG 0
void split(const std::string& str, const std::string& delim, std::vector<std::string> & tokens) // split functions allows the user to split the string w.r.t. the delimiter specified
{
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev); // find the position of delimiter and store it in 'pos'
        if (pos == std::string::npos) pos = str.length(); // delimiter not found in string at all or reached the end of the string
        std::string token = str.substr(prev, pos-prev); // split the string at the position of the delimiter
        if (!token.empty()) tokens.push_back(token); // if the string is split and stored in the std::string token, add it to the std::set of strings.
        prev = pos + delim.length(); // start finding next delimiter starting from the last position where you found the delimiter.
    }
    while (pos < str.length() && prev < str.length()); // check the pos has not overshot the length of the string.
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
void is_stop_word(std::string & word, std::vector<std::string>& stopwords)
{
    for(auto E: stopwords)
    {
        if(word == E)
        {
            word = "";
            return;
        }
    }
}


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   return 0;
}
int safe_exec(sqlite3 * database, const char * arg)
{
    char *zErrMsg = 0;
    int rc;
    int * p;
    rc = sqlite3_exec(database, arg,callback, 0, &zErrMsg);    
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(EXIT_FAILURE);
    } 
    else 
    {
        #if DEBUG
        fprintf(stdout, "Command executed successfully\n");
        #endif
        return rc;
    }    
}
void create_db_scheme(sqlite3 * database)
{
    char *zErrMsg = 0;
    int rc;
    const char * docs =               "CREATE TABLE documents("\
                                      "ID INTEGER PRIMARY     KEY         AUTOINCREMENT,"\
                                      "NAME                   TEXT        NOT NULL);";
    
    const char * words =              "CREATE TABLE words("\
                                      "ID                     INTEGER         NOT NULL,"\
                                      "DOCID                  INTEGER         NOT NULL,"\
                                      "VALUE                  TEXT            NOT NULL);";
    
    const char * characters =        "CREATE TABLE characters("\
                                      "ID                     INTEGER         NOT NULL,"\
                                      "WORDID                 INTEGER         NOT NULL,"\
                                      "VALUE                  TEXT            NOT NULL);";
        
    safe_exec(database, docs);
    safe_exec(database, words);
    safe_exec(database, characters);

}
int read_database(sqlite3 * db, char * query)
{
    //char buf[512];
    sqlite3_stmt * statement;

    int row = 0;
    int bytes;
    int data;
    unsigned char * text;
    sqlite3_prepare(db,query, strlen(query), &statement,NULL);
    int done = 0;
    while(!done)
    {
        switch(sqlite3_step(statement))
        {
            case SQLITE_ROW:
                bytes = sqlite3_column_bytes(statement,0);
                data = sqlite3_column_int(statement,0);
                //printf("count :%d, data: %d, bytes:%d, text: %s\n", row, data,bytes, text);
                row++;
                break;
            case SQLITE_DONE:
                done = 1;
                break;
            default:
                printf("Failed\n");
                return -1;
        }
        return data;
    }

}
std::vector<std::pair<std::string,int>> get_freqs(sqlite3 * db, char * query)
{
    sqlite3_stmt * statement;
    int row = 0;
    unsigned char * text;
    int freqs;
    int done = 0;
    std::vector<std::pair<std::string,int>> ret;
    std::string push_pair_text;
    sqlite3_prepare(db,query, strlen(query), &statement,NULL);
    while(!done || row < 24)
    {
      switch(sqlite3_step(statement))
        {
            case SQLITE_ROW:
                push_pair_text = std::string((char *)sqlite3_column_text(statement,0));
                freqs = sqlite3_column_int(statement,1);
                ret.push_back(std::pair<std::string, int>(push_pair_text, freqs));
                //printf("count :%d, data: %d, bytes:%d, text: %s\n", row, data,bytes, text);
                row++;
                break;
            case SQLITE_DONE:
                done = 1;
                break;
            default:
                printf("Failed\n");
                exit(EXIT_FAILURE);
        }
    }
    return ret;
}
void load_file_into_database(sqlite3 * database, std::string path)
{
    std::function<std::vector<std::string>(std::string)> extract_words = [] (std::string filename) -> std::vector<std::string>
    {
            std::ifstream PandP(filename);
            std::ifstream StopWordsStream("../stop_words.txt");
            const char* EscapeChars = "!@#$%*()_-[]\"\';:\?/.,\n\0";
            std::string stop_words;
            std::vector<std::string> StopWords;
            std::getline(StopWordsStream,stop_words);
            split(stop_words, ",",StopWords);
            std::string buf;
            std::vector<std::string> data;
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
                    std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),std::back_inserter(data)); // same as a for loop, but captures all the string and puts it into a vector of strings.
                    while(std::find(data.begin(),data.end()," ") != data.end()) // find the extra space charaters in the vector 
                    {
                        std::vector<std::string>::iterator iter;
                        iter = find(data.begin(),data.end()," ");
                        data.erase(iter); // remove them from the vector
                    }
                }
            }

            std::vector<std::string> ret;
            std::string retbuf;
            for(auto E: data)
            {
                retbuf = E;
                is_stop_word(retbuf,StopWords);
                if(retbuf.length() >= 2)
                {
                    ret.push_back(retbuf);
                }
            }
            return ret;

    };
    std::vector<std::string> list_of_words = extract_words(path);

    char buf[512];
    sprintf(buf, "INSERT INTO documents (NAME) VALUES (\'%s\');", path.c_str());
    safe_exec(database, (const char *)buf);
    sprintf(buf, "SELECT ID FROM documents WHERE NAME=\'%s\';",path.c_str());
    int doc_id = read_database(database, buf);
    sprintf(buf, "SELECT MAX(id) FROM words;");
    int word_id = read_database(database,buf);
    
    for(auto E: list_of_words)
    {
        sprintf(buf, "INSERT INTO words (ID, DOCID, VALUE) VALUES (%d, %d, \'%s\');", word_id, doc_id, E.c_str());
        safe_exec(database, (const char *)buf);
        int char_id = 0;
        for(auto F: E)
        {
            sprintf(buf, "INSERT INTO characters (ID, WORDID, VALUE) VALUES (%d,%d, \'%c\');", char_id, word_id, F);
            safe_exec(database, (const char *)buf);
            char_id++;
        }
        word_id++;

    }
}
void safe_open(sqlite3 ** db)
{
    int rc = sqlite3_open("twenty-five-database.db", db);
   
   if( rc ) {
       #if DEBUG
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
      #endif
      exit(EXIT_FAILURE);
   } else {
       #if DEBUG
      fprintf(stdout, "Opened database successfully\n");
      #endif
   }
}
inline bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
int main(int argc, char const *argv[])
{
    sqlite3* db;
    if(!exists("twenty-five-database.db"))
    {
        safe_open(&db);
        create_db_scheme(db);   
        load_file_into_database(db,argv[1]);
    }
    else
    {
        safe_open(&db);
    }
    char buf[512];
    sprintf(buf, "SELECT VALUE, COUNT(*) as C FROM words GROUP BY VALUE ORDER BY C DESC;");
    std::vector<std::pair<std::string,int>> ret = get_freqs(db, buf);
    for(int i=0; i<25; i++)
    {
        std::cout<<ret[i].first<<"  -  "<<ret[i].second<<std::endl;
    }
    sqlite3_close(db);
    
    return 0;
}
