#include <stdio.h>
#include <stdint.h>
#include <assert.h>

uint8_t* data = nullptr;
size_t maxSize = 0;
size_t pos = 0;

void makeAllocator(size_t maxSize)
{
    if(data)
        delete[] data;
    data = new uint8_t[maxSize];
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

int main()
{
//tests
    makeAllocator(100);
    char* c = nullptr;

//try to write
    c = alloc(99);
    *(c+10) = 'c';
    c = nullptr;

//alloc summury > maxSize
    c = alloc(2);
    assert(!c);         //must be nullptr
    c = nullptr;

//repeat after reset
    reset();
    c = alloc(2);
    assert(c);         //must be not nullptr
    c = nullptr;

//after resume alloc > maxSize
    reset();
    c = alloc(101);
    assert(!c);         //must be nullptr
    c = nullptr;

    if(data)
        delete[] data;
    printf("All tests are passed\n");
    return 0;
}
