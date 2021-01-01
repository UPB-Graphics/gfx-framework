#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iterator>


#if defined(_WIN32)
#   define PATH_SEPARATOR   '\\'
#else
#   define PATH_SEPARATOR   '/'
#endif


// -------------------------------------------------------------------------
namespace TextUtils
{
    std::string Join(
        const std::vector<std::string> &elements,
        const std::string &separator);

#define PATH_JOIN(...) TextUtils::Join(std::vector<std::string>{__VA_ARGS__}, std::string(1, PATH_SEPARATOR))
}
