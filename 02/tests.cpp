#include <functional>
#include <assert.h>
#include "parser.hpp"

static std::string numbers;
static std::string strings;
static bool begin = false;
static bool end = false;

void deinit()
{
    numbers.erase();
    strings.erase();
    begin = false;
    end = false;
}

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
    begin = true;
}

void end_call()
{
    printf("End of parsing\n");
    end = true;
}

int main()
{
    //tests
    //not all callbacks
    register_string_callback(string_call);
    register_begin(begin_call);
    register_end(end_call);
    assert(!parse("123 qwqwqwq\t 3221\n 1212 d") && !begin && !end);
    deinit();
    //Check out strings
    register_number_callback(number_call);
    assert(parse("123 qwqwqwq\t 3221\n 1212 d") && begin && end && numbers == "12332211212" && strings == "qwqwqwqd");
    deinit();
    //all spaces
    assert(parse("   \t \n\n   ") && begin && end && numbers == "" && strings == "");
    deinit();
    //start spaces
    assert(parse("   2\t \na\n   bb") && begin && end && numbers == "2" && strings == "abb");
    deinit();
    //empty
    assert(parse("") && begin && end && numbers == "" && strings == "");
    deinit();
    //one token
    assert(parse("121") && begin && end && numbers == "121" && strings == "");
    deinit();

    printf("All tests are passed\n");
    return 0;
}
