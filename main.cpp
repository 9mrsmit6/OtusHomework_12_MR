#include <iostream>
#include "Distributor.hpp"
#include "TestHandlers.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    //Передача параметров в приложение
    std::size_t M{2};
    std::size_t R{4};
    std::string P;

    auto setBaseArgs=[&]()
    {
        M=2;
        R=4;
        P="emails.txt";

        std::cout<<"Set M=2; R=4; P=emails.txt"<<std::endl;
    };

    if(argc<3)
    {
        std::cout<<"incorrect number of arguments"<<std::endl;
        setBaseArgs();
    }
    else
    {
        try
        {
            M= std::stoi(argv[1]);
            R= std::stoi(argv[2]);
            P= argv[3];
        }

        catch(...)
        {
            std::cout<<"wrong arguments"<<std::endl;
            setBaseArgs();
        }
    }

// Тут вся логика приложения
    Distrib<TestMap, ReducerHandler> dist(M,R,P);
    dist.execute();
    return 0;
}



