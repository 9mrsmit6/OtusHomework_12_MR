#ifndef TESTHANDLERS_HPP
#define TESTHANDLERS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>

//Это map и reduce для поиска минимального префикса

struct TestMap
{
    TestMap()=default;
    ~ TestMap()=default;

    std::string&& execute(std::string&& str)
    {
        std::string* ret=new std::string{};
        *ret=str.substr(0,prefixLen);
        return std::move(*ret);
    }

    void setPrefixLen(std::size_t p)
    {
        if(p<1){return;}
        prefixLen=p;
    }


private:
    std::size_t prefixLen{1};


};



struct ReducerHandler
{
    ReducerHandler(const std::size_t id_):
        id{id_}
    {

    };
    ~ ReducerHandler()=default;

    void execute(std::vector<std::string>& vect)
    {
        for(auto& s:vect)
        {
            if(rezultMap.contains(s))
            {
                rezultMap[s]++;
                hasUniqOnly=false;
            }
            else
            {
                rezultMap[s]=1;
            }
        }

        saveRezult();
    }


    bool uniqOnly(){return hasUniqOnly;}

    void reset()
    {
        hasUniqOnly=true;
        rezultMap.clear();
    }

private:
    std::unordered_map<std::string, std::size_t> rezultMap;
    bool hasUniqOnly{true};

    void saveRezult()
    {
        std::stringstream fileName;
        fileName<<"Reducer "<<id<<".log";

        std::ofstream file(fileName.str(), std::ios::out);
        if(!file.is_open()){return;}

        file<<"Uniq:"<<std::boolalpha<<hasUniqOnly<<std::noboolalpha<<std::endl;
        for(auto& s:rezultMap)
        {
            file<<s.first<<":"<<s.second<<std::endl;
        }
    }

    const std::size_t id;
};

#endif // TESTHANDLERS_HPP
