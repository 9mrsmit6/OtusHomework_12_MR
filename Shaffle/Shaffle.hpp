#ifndef SHAFFLE_HPP
#define SHAFFLE_HPP

#include <mutex>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <cassert>
#include <memory>




struct TestReducer
{
    TestReducer()=default;
    ~ TestReducer()=default;

    void addJob(std::string&& str)
    {
        vect.push_back(str);
    }

    std::vector<std::string> vect;
};





template <class T, class Job>
concept ReducerForShuffle=
        requires(T m, Job& s)
    {
           m.addJob(std::move(s))     ;
    };

template <std::equality_comparable Key, ReducerForShuffle<Key> Val>
struct ShaffleMap
{
    ShaffleMap(std::vector<std::shared_ptr<Val>>& reducers_):
        reducers(reducers_)
    {
        assert(reducers.size()>0 && "Reducers poll is empty");
    }

    ~ShaffleMap()=default;

    void shaffleJob(Key& job)
    {
        std::lock_guard guard(mutex);

        if(map.contains(job))
        {
            map[job]->addJob(std::move(job));
        }
        else
        {
            auto& reducer=getNextReducer();
            map[job]=reducer;
            reducer->addJob(std::move(job));
        }
    }

    private:
        std::mutex mutex;
        std::unordered_map<Key,std::shared_ptr<Val>>  map;

        std::size_t reducerCounter{0};
        std::vector<std::shared_ptr<Val>>& reducers;

        std::shared_ptr<Val>& getNextReducer()
        {
            if(reducerCounter>=reducers.size())
            {
                reducerCounter=0;
            }
            auto& ret=reducers[reducerCounter];
            reducerCounter++;
            return ret;
        }

};

#endif // SHAFFLE_HPP
