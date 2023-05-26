#include <iostream>
#include "Utils/BlockManager.hpp"
#include "Map/Map.hpp"


using namespace std;






void testPrint(std::size_t start, std::filesystem::path path)
{
    std::ifstream f{path};
    f.seekg(start);

    std::string temp;
    std::getline(f,temp);
    f.close();

    std::cout<<temp<<std::endl;
}


struct TestMap
{
    TestMap()=default;
    ~ TestMap()=default;

    std::string&& execute(std::string&& str)
    {
        std::string* ret=new std::string{};
        *ret=(str+"test");
        return std::move(*ret);
    }


};

int main()
{
    constexpr std::size_t blockCount{5};

    std::filesystem::path fp{"emails.txt"};

    BlockManager bm{10, "emails.txt"};

    std::cout<<"BlockSize:"<<bm.getBlockSize()<<std::endl;
    for(auto i{0};i!=bm.getBlockCount();i++)
    {
        std::cout<<"Block:"<<bm.getBlockOffset(i).value_or(133)<<std::endl;
    }


    Map<TestMap> m;
    m.execute();

    m.join();


    std::cout<<"JOIN"<<std::endl;

    m.execute();
    m.join();

//    std::size_t n{0};
//    for(int i{0};i!=20;i++)
//    {
//        auto v=nextEndLine(n, fp);
//        if(v)
//        {
//            testPrint(n, fp);
//            n=nextEndLine(n, fp).value();
//        }


//    }










//    cout << 310 <<" : "<<310+temp.size()<<endl;
    return 0;
}
