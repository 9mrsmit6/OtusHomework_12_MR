#ifndef SHAFFLE_HPP
#define SHAFFLE_HPP

#include <mutex>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <cassert>
#include <memory>

//Смешиватель

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
        assert(reducers.size()>0 && "Reducers poll is empty");  //Нужно чтобы редюсеры были!
    }

    ~ShaffleMap()=default;

    void shaffleJob(std::vector<Key>& job)
    {
        if(job.empty()){return ;}

        std::lock_guard guard(mutex);  //Блокируемся т.к. мы общий обьект

        if(map.contains(job[0]))        //Ищем есть редюсер для такого блока данных
        {
            auto& reducer=map[job[0]];  //Если есть то берем его чтобы не искать его по таблице каждый раз
            for(auto& v:job)
            {
                reducer->addJob(std::move(v));  //Запихиваем обьекты в reducers
            }
        }
        else        //Если нет то все тоже самое. тока добавляем его в таблицу
        {
            auto& reducer=getNextReducer();
            map[job[0]]=reducer;
            for(auto& v:job)
            {
                reducer->addJob(std::move(v));
            }
        }
    }

    void reset()    //Сброс
    {
       std::lock_guard guard(mutex);
       map.clear();
       reducerCounter=0;
    }

    private:
        std::mutex mutex;
        std::unordered_map<Key,std::shared_ptr<Val>>  map;

        std::size_t reducerCounter{0};
        std::vector<std::shared_ptr<Val>>& reducers;

        std::shared_ptr<Val>& getNextReducer()  //Получение редьюсера из пула
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
