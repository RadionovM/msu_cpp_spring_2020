#include <functional>
#include <assert.h>
#include "parser.hpp"
static std::string numbers;
static std::string strings;
int ii =10;
void number_call1(const char* str)
{
    printf("number: %s %d\n", str, ii);
}

void string_call1(const char* str)
{
    printf("string: %s\n", str);
}

void number_call2(const char* str)
{
    numbers += str;
}

void string_call2(const char* str)
{
    strings += str;
}

void begin_call()
{
    printf("Start parsing\n");
}

void end_call()
{
    printf("End of parsing\n");
}

int main()
{
    //tests
    //not all callbacks
    registre_string_callback(string_call1);
    registre_begin(begin_call);
    registre_end(end_call);
    parse("123 qwqwqwq\t 3221\n 1212 d");
    //
    registre_number_callback(number_call1);
    parse("123 qwqwqwq\t 3221\n 1212 d");
    //
    registre_number_callback(number_call2);
    registre_string_callback(string_call2);
    parse("123 qwqwqwq\t 3221\n 1212 d");
    printf("%s %s\n", numbers.c_str(), strings.c_str());
    return 0;
}
