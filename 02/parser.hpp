#include <functional>

bool parse(const char* string);
void registre_string_callback(std::function<void(const char*)> func);
void registre_number_callback(std::function<void(const char*)> func);
void registre_begin(std::function<void()> func);
void registre_end(std::function<void()> func);

