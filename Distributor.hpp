#ifndef DISTRIBUTOR_HPP
#define DISTRIBUTOR_HPP

#include "Utils/BlockManager.hpp"

#include "Map/Map.hpp"
#include "Shaffle/Shaffle.hpp"
#include "Reduce/Reduce.hpp"


template <class MObj, class ReObj>
struct Distrib
{
    using ShaffleFunc=ShaffleMap<std::string, Reducer<ReObj>>;
    using MapFunc= std::shared_ptr<Map<MObj,  ShaffleFunc>> ;
    using ReduceFunc=std::shared_ptr<Reducer<ReObj>>;


    Distrib(std::size_t M,std::size_t R, std::string path):
        shaffleObj{reduce}
    {
        for(std::size_t i{0};i!=R;i++)
        {
            reduce.push_back(std::make_shared<Reducer<ReObj>>());
        }

         BlockManager bm{M, path};


        for(std::size_t i{0};i!=M;i++)
        {
            auto offset=bm.getBlockOffset(i);
            if(offset)
            {
                map.push_back(std::make_shared<Map<MObj,  ShaffleFunc>>(offset.value(),bm.getBlockSize(), path, shaffleObj));
            }
        }

    }
    ~Distrib()=default;

    void execute()
    {
        step(map);
        step(reduce);
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



    std::vector<MapFunc> map;
    ShaffleFunc shaffleObj;
    std::vector<ReduceFunc> reduce;

};

#endif // DISTRIBUTOR_HPP
