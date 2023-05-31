#ifndef TESTHANDLERS_HPP
#define TESTHANDLERS_HPP

#include <string>
#include <vector>
#include <unordered_map>

struct TestMap
{
    TestMap()=default;
    ~ TestMap()=default;

    std::string&& execute(std::string&& str)
    {

        std::string* ret=new std::string{};
        *ret=str.substr(0,2);
        return std::move(*ret);
    }


};



struct ReducerHandler
{
    ReducerHandler()=default;
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
    }


    bool uniqOnly(){return hasUniqOnly;}

private:
    std::unordered_map<std::string, std::size_t> rezultMap;
    bool hasUniqOnly{true};
};

#endif // TESTHANDLERS_HPP
