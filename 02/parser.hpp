#include <functional>
namespace libparser
{

using BeginEndCall = std::function<void()>;
using NumberCall = std::function<void(uint64_t)>;
using StringCall = std::function<void(const char*)>;

bool parse(const char* string);
void register_string_callback(StringCall func);
void register_number_callback(NumberCall func);
void register_begin(BeginEndCall func);
void register_end(BeginEndCall func);

}
