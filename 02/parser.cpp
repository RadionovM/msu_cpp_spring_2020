#include "parser.hpp"
#include <cstdlib>

static std::function<void()> begin_call;
static std::function<void()> end_call;
static std::function<void(uint64_t)> number_call;
static std::function<void(const char*)> string_call;

bool parse(const char* text)
{
    if(!begin_call || !end_call || !string_call || !number_call)
        return false;
    begin_call();
    std::string token;
    char cur;
    do
    {
        cur = *text++;
        if(cur == ' ' or cur == '\t' or cur == '\n' or cur =='\0')
        {
            if(!token.empty())
            {
                if(token[0] >= '0' && token[0] <= '9')
                    number_call(std::stoull(token.c_str()));
                else
                    string_call(token.c_str());
                token.erase();
            }
        }
        else
            token += cur;
    }
    while(cur);
    end_call();
    return true;
}

void register_string_callback(std::function<void(const char*)> func)
{
   string_call = func;
}

void register_number_callback(std::function<void(uint64_t)> func)
{
    number_call = func;
}

void register_begin(std::function<void()> func)
{
    begin_call = func;
}

void register_end(std::function<void()> func)
{
    end_call = func;
}
