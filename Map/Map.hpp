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


template <class T>
concept MapHandler=
        std::default_initializable<T> && requires(T m, std::string s)
    {
        {   m.execute(std::move(s))     }   ->  std::same_as<std::string&&>;
    };

template <class T>
concept MapShaflle=
        requires( T m, std::vector<std::string>& s )
    {
           m.shaffleJob(s) ;
    };

template <MapHandler Handler, MapShaflle Shaffle>
struct Map
{
    Map(const std::size_t start_, const std::size_t stop_, const std::filesystem::path path_, Shaffle& shaffler_):
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

    Shaffle& shaffler;

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
                std::sort(out.begin(),out.end());
                trueExecution=true;

                auto start=out.begin();
                while(true)
                {
                    auto rez=std::find_if(start, out.end(),[start](std::string& s) -> bool {    return s!=(*start);     }       );
                    auto vect=std::vector(start,rez);
                    shaffler.shaffleJob(vect);

                    if(rez==out.end()){break;}
                    start=rez;
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
