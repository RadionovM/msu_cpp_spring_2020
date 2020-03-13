#include <functional>
#include <assert.h>
#include "parser.hpp"

static std::string numbers;
static std::string strings;

void number_call(const char* str)
{
    numbers += str;
}

void string_call(const char* str)
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
    registre_string_callback(string_call);
    registre_begin(begin_call);
    registre_end(end_call);
    assert(!parse("123 qwqwqwq\t 3221\n 1212 d"));
    numbers.erase();
    strings.erase();
    //Check out strings
    registre_number_callback(number_call);
    registre_string_callback(string_call);
    parse("123 qwqwqwq\t 3221\n 1212 d");
    assert(numbers == "12332211212" && strings == "qwqwqwqd");
    numbers.erase();
    strings.erase();
    //all spaces
    parse("   \t \n\n   ");
    assert(numbers == "" && strings == "");
    numbers.erase();
    strings.erase();
    //start spaces
    parse("   2\t \na\n   bb");
    assert(numbers == "2" && strings == "abb");
    numbers.erase();
    strings.erase();
    //empty
    parse("");
    assert(numbers == "" && strings == "");
    numbers.erase();
    strings.erase();
    //one token
    parse("121");
    assert(numbers == "121" && strings == "");
    numbers.erase();
    strings.erase();

    printf("All tests are passed\n");
    return 0;
}
