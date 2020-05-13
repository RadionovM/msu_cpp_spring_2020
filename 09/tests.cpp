#include <future>
#include <assert.h>
#include <vector>
#include <ctime>
#include <exception>
#include <algorithm>
#include <thread>
#include <iterator>
#include <algorithm>
#include <deque>
#include "pool.hpp"

static ThreadPool pool(2);
static constexpr size_t chunk_size = 1024*100;
void make_file(const char* name, int quantity)
{
    FILE* file = fopen(name,"w");
    if(!file)
        throw std::runtime_error(std::string("cant open file: ") + name);
    uint64_t number;
    std::srand(time(nullptr));
    for(int i = 0; i < quantity; ++i)
    {
        number = std::rand();
        fwrite(&number, sizeof(uint64_t), 1, file);
    }
    fclose(file);
}

size_t check_sorted_file(const char* name)
{
    FILE* file = fopen(name,"r");
    size_t size = 0;
    if(!file)
        throw std::runtime_error(std::string("cant open file: ") + name);
    uint64_t number {0};
    uint64_t prev_number {0};
    while(fread(&number, sizeof(uint64_t), 1, file) == 1)
    {
        if(number < prev_number)
        {
            fclose(file);
            return size;
        }
        size++;
        prev_number = number;
    }
    if(!feof(file))
    {
        fclose(file);
        throw std::runtime_error(std::string("Error while reading file: ") + name);
    }
    fclose(file);
    return size;
}
void sort_vector(std::vector<uint64_t>& buffer)
{
    std::sort(buffer.begin(), buffer.end());
}

void merge_and_dump_vectors(const std::vector<uint64_t>& buffer1, const std::vector<uint64_t>& buffer2, const char* name)
{
    FILE* file = fopen(name,"w");
    if(!file)
        throw std::runtime_error(std::string("cant open file: ") + name);
    auto it1 = buffer1.begin();
    auto it2 = buffer2.begin();
    while(it1 != buffer1.end() && it2 != buffer2.end())
    {
        const uint64_t* number;
        if(*it1 >= *it2)
        {
            number = &(*it2);
            ++it2;
        }
        else
        {
            number = &(*it1);
            ++it1;
        }

        if(1 != fwrite(number, sizeof(uint64_t), 1, file))
        {
            fclose(file);
            throw std::runtime_error(std::string("cant write number to file: ") + name);
        }
    }
    auto end = buffer1.end();
    if(it2 != buffer2.end())
    {
        it1 = it2;
        end = buffer2.end();
    }
    for(;it1 != end;++it1)
    {
        if(1 != fwrite(&(*it1), sizeof(uint64_t), 1, file))
        {
            fclose(file);
            throw std::runtime_error(std::string("cant write number to file: ") + name);
        }
    }
    fclose(file);
}
void sort_and_merge(std::vector<uint64_t>& buffer1, std::vector<uint64_t>& buffer2, const char* name)
{
    auto th1 = pool.exec(sort_vector, std::ref(buffer1));
    auto th2 = pool.exec(sort_vector, std::ref(buffer2));
    th1.get();
    th2.get();
    merge_and_dump_vectors(buffer1, buffer2, name);
    buffer1.clear();
    buffer2.clear();
}
size_t file_to_sort_chunks(const char* name)
{
    FILE* file = fopen(name,"r");
    if(!file)
        throw std::runtime_error(std::string("cant open file: ") + name);
    std::vector<uint64_t> buffer1;
    std::vector<uint64_t> buffer2;
    buffer1.reserve(chunk_size/2);
    buffer2.reserve(chunk_size/2);
    uint64_t number;
    int chunks_number = 0;
    while(fread(&number, sizeof(uint64_t), 1, file) == 1)
    {
        if(buffer1.size() < chunk_size/2)
            buffer1.push_back(number);
        else if(buffer2.size() < chunk_size/2)
            buffer2.push_back(number);
        else
        {
            sort_and_merge(buffer1,buffer2, (".tmp" + std::to_string(chunks_number) + ".dat").c_str());
            buffer1.push_back(number);
            chunks_number++;
        }
    }
    if(buffer1.size() != 0 || buffer2.size() != 0)
    {
        sort_and_merge(buffer1,buffer2, (".tmp" + std::to_string(chunks_number) + ".dat").c_str());
        chunks_number++;
    }
    fclose(file);
    return chunks_number;
}

void merge_pair(const std::string& name1, const std::string& name2, const  std::string& name_out)
{
    uint64_t a;
    uint64_t b;
    FILE* file1  = fopen(name1.c_str(),"r");
    FILE* file2  = fopen(name2.c_str(),"r");
    FILE* file_out  = fopen(name_out.c_str(),"w");
    if(!file1)
        throw std::runtime_error(std::string("cant open file: ") + name1);
    if(!file2)
        throw std::runtime_error(std::string("cant open file: ") + name2);
    if(!file_out)
        throw std::runtime_error(std::string("cant open file: ") + name_out);

    if(fread(&a, sizeof(uint64_t), 1, file1) != 1)
        throw std::runtime_error(std::string("cant read file: ") + name1);
    if(fread(&b, sizeof(uint64_t), 1, file2) != 1)
        throw std::runtime_error(std::string("cant read file: ") + name2);
    while(!feof(file1) && !feof(file2))
    {
        if(a >= b)
        {
            if(fwrite(&b, sizeof(uint64_t), 1, file_out) != 1)
                throw std::runtime_error(std::string("cant write file: ") + name_out);

            if(fread(&b, sizeof(uint64_t), 1, file2) != 1)
            {
                if(!feof(file2))
                    throw std::runtime_error(std::string("cant read file: ") + name2);
            }
        }
        else
        {
            if(fwrite(&a, sizeof(uint64_t), 1, file_out) != 1)
                throw std::runtime_error(std::string("cant write file: ") + name_out);

            if(fread(&a, sizeof(uint64_t), 1, file1) != 1)
            {
                if(!feof(file1))
                    throw std::runtime_error(std::string("cant read file: ") + name1);
            }
        }
    }
    if(feof(file1))
    {
        while(!feof(file2))
        {
            if(fwrite(&b, sizeof(uint64_t), 1, file_out) != 1)
                throw std::runtime_error(std::string("cant write file: ") + name_out);
            if(fread(&b, sizeof(uint64_t), 1, file2) != 1)
            {
                if(!feof(file2))
                    throw std::runtime_error(std::string("cant read file: ") + name2);
            }
        }
    }
    else
    {
        while(!feof(file1))
        {
            if(fwrite(&a, sizeof(uint64_t), 1, file_out) != 1)
                throw std::runtime_error(std::string("cant write file: ") + name_out);
            if(fread(&a, sizeof(uint64_t), 1, file1) != 1)
            {
                if(!feof(file1))
                    throw std::runtime_error(std::string("cant read file: ") + name1);
            }
        }
    }
    fclose(file1);
    fclose(file2);
    fclose(file_out);
}

void worker(const char* infile_name)
{
    size_t chunks_number = file_to_sort_chunks(infile_name);
    switch (chunks_number)
    {
        case 0:
            throw std::runtime_error(std::string("cant devide file"));
            break;
        case 1:
            rename(".tmp0.dat", "output.dat");
            break;
        case 2:
            merge_pair(".tmp0.dat", ".tmp1.dat", "output.dat");
            break;
        default:
        {
            size_t i = 2;
            merge_pair(".tmp0.dat", ".tmp1.dat", ".tmp" + std::to_string(chunks_number) + ".dat");
            for(; i < chunks_number - 1; i++)
            {
                merge_pair(".tmp" + std::to_string(i) + ".dat",
                    ".tmp" + std::to_string(i + chunks_number - 2) + ".dat",
                    ".tmp"+ std::to_string(chunks_number - 1 + i) +".dat");

            }
            merge_pair(".tmp" + std::to_string(i) + ".dat",
                ".tmp" + std::to_string(i + chunks_number - 2) + ".dat",
                "output.dat");
        }
    }
}

int main()
{
    const char* infile_name = "input.dat";
    size_t infile_size = 1e3; //меньше чанка
    make_file(infile_name, infile_size);
    worker(infile_name);
    assert(infile_size==check_sorted_file("output.dat"));
    infile_size = 2e5; //порядка чанка
    make_file(infile_name, infile_size);
    worker(infile_name);
    assert(infile_size==check_sorted_file("output.dat"));
    infile_size = 2e6;//большой
    make_file(infile_name, infile_size);
    worker(infile_name);
    assert(infile_size==check_sorted_file("output.dat"));
}
