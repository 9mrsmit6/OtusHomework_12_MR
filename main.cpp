#include <iostream>
#include "Distributor.hpp"
#include "TestHandlers.hpp"


using namespace std;






int main()
{

    Distrib<TestMap, ReducerHandler> dist(4,8, "emails.txt");
    dist.execute();

    std::cout<<"SASDAD";

    return 0;
}
