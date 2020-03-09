#include <stdio.h>
#include <assert.h>

#include "allocator.hpp"

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

    free_memory();
    assert(!c);
    printf("All tests are passed\n");
    return 0;
}
