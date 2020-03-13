#include <functional>

bool parse(const char* string);
void register_string_callback(std::function<void(const char*)> func);
void register_number_callback(std::function<void(const char*)> func);
void register_begin(std::function<void()> func);
void register_end(std::function<void()> func);

