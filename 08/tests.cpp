#include <iostream>
#include <sstream>
#include <future>
#include <thread>
#include <deque>
#include "pool.hpp" 

uint64_t long_f(int i)
{
    uint64_t res =0;
    for(int j = 0; j <= i*i; j++)
    {
        res++;
    }
    fprintf(stderr,"long_f terminate\n");
    return res;
}

void long_f_void(std::string i)
{
    uint64_t res =0;
    for(int j = 0; j <= i.size()*i.size()*i.size(); j++)
    {
        res++;
    }

    fprintf(stderr,"void long_f terminate\n");
}

void long_f_exception(std::string i)
{
    uint64_t res =0;
    for(int j = 0; j <= i.size()*i.size()*i.size(); j++)
    {
        res++;
    }
    throw std::string("void long_f exception");
}

int main()
{
    ThreadPool pool(10);
    auto task1 = pool.exec(long_f, 10);
    auto task2 = pool.exec(long_f_exception, "eeee");
    auto task3 = pool.exec(long_f_void, "eeee");
    try
    {
        task2.get();
    }
    catch(std::string& e)
    {
        fprintf(stderr,"Catch such exception: %s\n",e.c_str());
    }
    auto task4 = pool.exec(long_f_void, "iii");
    task3.get();
    auto i = task1.get();
    task4.get();
    fprintf(stderr,"main finished\n");
    return 0;
}
