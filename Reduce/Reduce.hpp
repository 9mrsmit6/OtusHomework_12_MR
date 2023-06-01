#ifndef REDUCE_HPP
#define REDUCE_HPP


#include <unordered_map>
#include <vector>
#include <string>
#include <thread>

template <class T>
concept ReduceHandler=
        requires(T m, std::vector<std::string>& s)
    {
        m.execute(s);
    };

//Это reducer который сам по себе очень простой. Он просто вызывает внешний обработчик. Который описан концептом выше
template <ReduceHandler ReHandler>
struct Reducer
{
    Reducer(std::unique_ptr<ReHandler>& handler_):
        handler{std::move(handler_)}
    {
    }

    ~ Reducer()=default;

    void addJob(std::string&& str)      //Этим методом смешивалка запихивает сюда работу
    {
        vect.push_back(str);
    }

    void execute()                      //Запускаем процесс и делегируем работу внешнему обработчику
    {
        auto job=[this]()
        {
            handler->execute(vect);
            vect.clear();
        };

        thread=std::thread(job);
    }


    void join()             //Ждем окончания
    {
        thread.join();
    }

    const std::unique_ptr<ReHandler>& getHandlerPtr(){return handler;};


private:
    std::vector<std::string> vect;
    std::unique_ptr<ReHandler> handler;
    std::thread thread;


};

#endif // REDUCE_HPP
