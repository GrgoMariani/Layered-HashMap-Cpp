#include <random>
#include <string>
#include <algorithm>

 /* StackOverflow is always great to get these kinds of functions in no time                      *
  * https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c */
std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
