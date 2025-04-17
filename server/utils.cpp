#include "utils.h"
#include <string>
#include <algorithm>

using std::string;

namespace utils{
std::string trim(const std::string& str){
    size_t first = str.find_first_not_of(" \r\n\t");
    size_t last = str.find_last_not_of(" \r\n\t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}
};