#ifndef DISTRIBUTOR_HPP
#define DISTRIBUTOR_HPP

#include "Utils/BlockManager.hpp"

#include "Map/Map.hpp"
#include "Shaffle/Shaffle.hpp"
#include "Reduce/Reduce.hpp"

//Это часть, отвечающая непосредственно за решение задачи поиска минимального префикса
//В шаблоне передается объект для map и reduce
template <class MObj, class ReObj>
struct Distrib
{
    using ShaffleFunc   =   ShaffleMap<std::string, Reducer<ReObj>>;
    using MapFunc       =   std::shared_ptr<Map<MObj,  ShaffleFunc>> ;
    using ReduceFunc    =   std::shared_ptr<Reducer<ReObj>>;


    Distrib(std::size_t M,std::size_t R, std::string path):
        shaffleObj{reduce}
    {
        //Собираем всю схему
        //Cначала reduce
        for(std::size_t i{0};i!=R;i++)
        {
            auto obj=std::make_unique<ReObj>(i);
            reduce.push_back(std::make_shared<Reducer<ReObj>>(obj));
        }

        //Этот обьект говорит смещения с учетом выравнивания по строкам
        BlockManager bm{M, path};

        //Теперь map
        std::size_t start{0};
        for(std::size_t i{0};i!=M;i++)
        {
            auto stop=bm.getBlockEnd(start);
            if(stop)
            {
                auto obj=std::make_unique<MObj>();
                map.push_back(std::make_shared<Map<MObj,  ShaffleFunc>>(start,stop.value(), path, shaffleObj, obj));
                start=stop.value();
            }
        }

    }
    ~Distrib()=default;

    //Итерации проверки префикса. Сначала 1 2 ..... 100
    //Ограничил 100
    void execute()
    {
        for(std::size_t it{1};it!=100;it++)
        {
            if(executeStep(it))
            {
                std::cout<<"MinPrefix: "<<it<<std::endl;
                return;
            }
        }
        std::cout<<"MinPrefix to long "<<std::endl;
    }


private:

    template <class T>
    void step(T& h)
    {
        for(auto& s:h)
        {
            s->execute();
        }
        for(auto& s:h)
        {
            s->join();
        }
    }

    void setStepPrefix(std::size_t prefix)
    {
        for(auto& s:map)
        {
            s->getHandlerPtr()->setPrefixLen(prefix);
        }
    }

    bool isUniqPrefix()
    {
        bool ret{true};
        for(auto& s:reduce)
        {
            ret=ret&(s->getHandlerPtr()->uniqOnly());
            s->getHandlerPtr()->reset();
        }
        return ret;
    }

    // Шаг вычислений
    bool executeStep(std::size_t iter)
    {
        setStepPrefix(iter);       //Установить длинну префиксов для всех map

        step(map);                  //Запускаем потоки map + shaffle
        step(reduce);               //Запускаем reduce

        shaffleObj.reset();         //Чистим перемешивалку после итерации

        return isUniqPrefix();      //Смотрим у всех ли reduce только уникальные префиксы
    }



    std::vector<MapFunc> map;   //map-ы
    ShaffleFunc shaffleObj;     //обьект перемешивания
    std::vector<ReduceFunc> reduce; //reducers

};

#endif // DISTRIBUTOR_HPP
