#include <stdio.h>
#include <stdint.h>
#include <new>

#include "allocator.hpp"

static uint8_t* data = nullptr;
static size_t maxSize = 0;
static size_t pos = 0;

void makeAllocator(size_t maxSize)
{
    if(data)
        delete[] data;
    data = nullptr;
    data = new(std::nothrow) uint8_t[maxSize];
    if(!data)
    {
        fprintf(stderr,"Cant alloc memory\n");
        return;
    }
    ::maxSize = maxSize;
}

char* alloc(size_t size)
{
    if(!data)
        return nullptr;
    if(pos + size > maxSize)
        return nullptr;
    pos += size;
    return (char*)(data + pos - size);
}

void reset()
{
    pos = 0;
}

void free_memory()
{
    if(data)
        delete[] data;
    data = nullptr;
}
