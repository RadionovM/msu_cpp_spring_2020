#include <future>
#include <assert.h>
#include <algorithm>
#include <thread>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <atomic>



int64_t generate_file()
{
    std::ofstream myfile;
    myfile.open ("input.dat");
    std::srand(time(nullptr));
    size_t str_number = size_t(100*((double) std::rand() / (RAND_MAX))) + 100;
    int64_t sum =0;
    for(size_t i = 0; i < str_number; ++i)
    {
        size_t str_size = size_t(100*((double) std::rand() / (RAND_MAX))) + 100;
        for(size_t j = 0; j < str_size; ++j)
        {
            int64_t number =  std::rand() - RAND_MAX/2;
            myfile << number << " ";
            sum += number;
        }
        myfile << std::endl;
    }
    myfile.close();
    return sum;
}

class SafeBuffer
{
    std::deque<std::string> buf;
    std::mutex mutex;
    std::condition_variable new_line;
    std::condition_variable buffer_free;
    size_t max_buffer_size {100};
public:
    std::atomic<bool> final_state {false};
    bool get_line(std::string& str)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(!buf.size() && !final_state)
            new_line.wait(lock);
        if(!buf.size() && final_state)
            return false;
        str = buf.front();
        buf.pop_front();
        if(buf.size() < max_buffer_size)
            buffer_free.notify_all();
        return true;
    }

    void push_line(const std::string& str)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(buf.size() >= max_buffer_size)
            buffer_free.wait(lock);
        buf.push_back(str);
        new_line.notify_all();
    }
};

int64_t sum_lines(SafeBuffer& buffer)
{
    std::string str;
    int64_t sum {0};
    while(buffer.get_line(str))
    {
        std::istringstream is(str);
        int64_t n;
        while(is >> n)
            sum += n;
         // do something with n
    }
    return sum;
}
int main()
{
    for(size_t i = 0 ; i < 10; ++i)
    {
        int64_t sum = generate_file();
        SafeBuffer buffer;
        std::future<int64_t> f1 =
            std::async(std::launch::async, sum_lines, std::ref(buffer));
        std::future<int64_t> f2 =
            std::async(std::launch::async, sum_lines, std::ref(buffer));
        std::fstream newfile;
        newfile.open("input.dat",std::ios::in); //open a file to perform read operation using file object
        if (newfile.is_open())
        {
            std::string tp;
            while(getline(newfile, tp))
            {
                buffer.push_line(tp);
            }
            newfile.close();
        }
        buffer.final_state = true;
        int64_t result1 = f1.get();
        int64_t result2 = f2.get();
        assert(sum == result1 + result2);
    }
    fprintf(stderr,"ALL TESTS ARE PASSED\n");
}
