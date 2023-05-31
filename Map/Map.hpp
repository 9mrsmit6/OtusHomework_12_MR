#ifndef MAP_HPP
#define MAP_HPP

#include <thread>
#include <iostream>
#include <string>
#include <utility>
#include <concepts>

#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../Shaffle/Shaffle.hpp"


//std::thread(&CommandProcessor::loggerTask, this)


template <class T>
concept MapHandler=
        std::default_initializable<T> && requires(T m, std::string s)
    {
        {   m.execute(std::move(s))     }   ->  std::same_as<std::string&&>;
    };


template <MapHandler Handler>
struct Map
{
    Map(const std::size_t start_, const std::size_t stop_, const std::filesystem::path path_, ShaffleMap<std::string, TestReducer>& shaffler_):
        start{start_},
        stop{stop_},
        path{path_},
        shaffler(shaffler_)
    { }

    void execute()
    {
        thread=std::thread(&Map::job, this);
    }

    void join()
    {
        thread.join();
    }

    const bool executionSucceed() const
    {
        return trueExecution;
    }

private:
    const std::size_t start;
    const std::size_t stop;
    const std::filesystem::path path;
    std::vector<std::string> out;

    ShaffleMap<std::string, TestReducer>& shaffler;

    bool trueExecution{false};

    Handler handler{};
    std::thread thread;
    void job()
    {
        out.clear();
        trueExecution=false;
        if(fileProcessing())
        {
            try
            {
                std::sort(out.begin(),out.end());   trueExecution=true;

                for(auto& v:out)
                {
                    shaffler.shaffleJob(v);
                }
            }
            catch(...)
            {
                trueExecution=false;
            }
        }

    }

    bool fileProcessing()
    {
        std::ifstream f;
        try
        {
            f.open(path);
            if(f.is_open())
            {
                f.seekg(start);

                while(f.tellg()<stop)
                {
                    std::string tmp;
                    std::getline(f,tmp);
                    auto newStr=handler.execute(std::move(tmp));
                    out.push_back(newStr);
                }
            }
            f.close();
            return true;
        }
        catch(...)
        {
            f.close();
            return false;
        }
    }
};

#endif // MAP_HPP
