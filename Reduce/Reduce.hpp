#ifndef REDUCE_HPP
#define REDUCE_HPP


#include <unordered_map>
#include <vector>
#include <string>
#include <thread>

template <class T>
concept ReduceHandler=
        std::default_initializable<T> && requires(T m, std::vector<std::string>& s)
    {
        m.execute(s);
    };


template <ReduceHandler ReHandler>
struct Reducer
{
    Reducer()=default;
    ~ Reducer()=default;

    void addJob(std::string&& str)
    {
        vect.push_back(str);
    }

    void execute()
    {
        auto job=[this]()
        {
            handler.execute(vect);
        };

        thread=std::thread(job);
    }


    void join()
    {
        thread.join();
    }


private:
    std::vector<std::string> vect;
    ReHandler handler;
    std::thread thread;
};

#endif // REDUCE_HPP
