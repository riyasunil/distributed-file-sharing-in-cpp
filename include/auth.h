#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, std::string> auth_keys;
namespace auth {
bool validate_user(const std::string& userID, const std::string& password);

}
#endif
