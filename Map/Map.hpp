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

//map

//концепты для проверки пользовательской части MAP на наличие необходимых методов
//Делал больше для изучения и тренировки

template <class T>
concept MapHandler=
        requires(T m, std::string s)
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
    Map(const std::size_t start_, const std::size_t stop_, const std::filesystem::path path_, Shaffle& shuffler_, std::unique_ptr<Handler>& handler_):
        start{start_},
        stop{stop_},
        path{path_},
        shuffler(shuffler_),
        handler{std::move(handler_)}
    { }

    void execute()          //Запуск потока
    {
        thread=std::thread(&Map::job, this);
    }

    void join()             //Ожидание завершения потока
    {
        thread.join();
    }

    const bool executionSucceed() const
    {
        return trueExecution;
    }

    const std::unique_ptr<Handler>& getHandlerPtr(){return handler;};


private:
    const std::size_t start;            //Границы блоков
    const std::size_t stop;
    const std::filesystem::path path;   //путь к файлу
    std::vector<std::string> out;       //Вектор обработанных строк
    std::unique_ptr<Handler> handler;   //Указатель на пользовательский(внешний) обработчик

    Shaffle& shuffler;                  //Ссылка на обьект для перемешивния

    bool trueExecution{false};

    std::thread thread;
    void job()                          //Непосредственно логика
    {
        out.clear();
        trueExecution=false;
        if(fileProcessing())        //читаем файл
        {
            try
            {
                std::sort(out.begin(),out.end());  //Сортируем

                auto start=out.begin();         //Формируем подмножество для передачи shuffle
                while(true)
                {
                    auto rez=std::find_if(start, out.end(),[start](std::string& s) -> bool {    return s!=(*start);     }       );
                    auto vect=std::vector(start,rez);
                    shuffler.shaffleJob(vect);  //Передаю на смешивание. В задание вроде бы количество потоков смешивания и map одинаковое. Поэтому сделал так

                    if(rez==out.end()){break;}
                    start=rez;                  //По итогу в reducer будут лежать их данные для обработки
                }

                trueExecution=true;

            }
            catch(...)
            {
                trueExecution=false;
            }
        }

    }

    bool fileProcessing() //Чтение файла реализация
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
                    auto newStr=handler->execute(std::move(tmp));
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
