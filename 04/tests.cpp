#include <assert.h>
#include <stdio.h>
#include <stdexcept>
#include "serializer.hpp"
#include <sstream>
#include <ostream>
#include <iostream>


struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)// в десериализаторе тоже используется этот выхов, зачем дублировать код
    {
        return serializer(a, b, c);
    }
};



int main()
{
    Data x {10, false,20};

    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(x);

    Data y { 0, false, 0};

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    assert(err == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    return 0;
}
