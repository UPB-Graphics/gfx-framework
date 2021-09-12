#include "utils/text_utils.h"

#include <algorithm>


// -------------------------------------------------------------------------
std::string text_utils::Join(
    const std::vector<std::string> &elements,
    const std::string &separator)
{
    std::ostringstream os;
    auto b = elements.begin();
    auto e = elements.end();

    if (b != e) {
        std::copy(b, prev(e), std::ostream_iterator<std::string>(os, separator.c_str()));
        b = prev(e);
    }

    if (b != e) {
        os << *b;
    }

    return os.str();
}
