#ifndef MAP_HPP
#define MAP_HPP

#include <thread>
#include <iostream>
#include <string>
#include <utility>
#include <concepts>
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
    void execute()
    {
        thread=std::thread(&Map::job, this);
    }

    void join()
    {
        thread.join();
    }

private:
    Handler handler{};
    std::thread thread;
    void job()
    {
        std::string myTestString{"PrivetMedved"};
        auto newStr=handler.execute(std::move(myTestString));
        std::cout<<newStr<<std::endl;
    }
};

#endif // MAP_HPP
