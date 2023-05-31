#ifndef SHAFFLE_HPP
#define SHAFFLE_HPP

#include <mutex>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <cassert>
#include <memory>



template <class T, class Job>
concept ReducerForShuffle=
        requires(T m, Job s)
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

    void shaffleJob(std::vector<Key>& job)
    {
        if(job.empty()){return ;}

        std::lock_guard guard(mutex);

        if(map.contains(job[0]))
        {
            auto& reducer=map[job[0]];
            for(auto& v:job)
            {
                reducer->addJob(std::move(v));
            }
        }
        else
        {
            auto& reducer=getNextReducer();
            map[job[0]]=reducer;
            for(auto& v:job)
            {
                reducer->addJob(std::move(v));
            }
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
