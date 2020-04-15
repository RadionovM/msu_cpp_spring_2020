#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>
#include <ctype.h>
#include <stdexcept>

inline std::string format(const std::string& str)
{
    return str;
}


inline bool my_is_digit(char c)
{
    if(c >= '0' and c <= '9')
        return true;
    return false;
}


template <class T>
std::string to_string (T&& arg)
{
    std::stringstream stream;
    stream << arg;
    return stream.str();
}

template <class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args_a)
{
    std::vector<std::string> args {to_string(std::forward<ArgsT>(args_a))...};
    std::string out;
    bool copy {false};
    int arg_n {0};
    for(auto it = str.begin(); it != str.end(); ++it)
    {
        if(*it == '}')
        {
            if(!copy)
                throw std::runtime_error("unexpected }");
            copy = false;
            if(arg_n >= args.size())
                throw std::runtime_error("argumet index out of range");
            out += args[arg_n];
            arg_n = 0;
            continue;
        }
        if(*it == '{')
        {
            if(copy)
                throw std::runtime_error("twice {");
            copy = true;
            continue;
        }
        if(copy)
        {
            if(!my_is_digit(*it))
                throw std::runtime_error("non digit ");
            arg_n = arg_n * 10 + *it - '0';
            continue;
        }
        out += *it;

    }
    if(copy)
        throw std::runtime_error("expected }");
    return out;
}
