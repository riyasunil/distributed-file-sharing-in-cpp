
// Online IDE - Code Editor, Compiler, Interpreter

#include<iostream>
#include <unordered_map>
#include <string>
#include "auth.h"
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::string;

namespace auth {
std::unordered_map<string, string> auth_keys = {
  {"UID9", "UT7OlpEtST"},
  {"UID7", "83WLKXx46q"},
  {"UID6", "3qciICMQOX"},
  {"UID8", "qmc0czdcre"},
  {"UID5", "vvSLyI7bJB"},
  {"UID1", "NDpZIUqDLu"},
  {"UID2", "KgXGskRomC"},
  {"UID0", "0Gp3iuAmyG"},
  {"UID3", "yXjxWP4kZB"},
  {"UID4", "wXWtWqf2g1"}
};


string trim(const string& str){
    size_t first = str.find_first_not_of(" \r\n\t");
    size_t last = str.find_last_not_of(" \r\n\t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}


bool validate_user(const string& userID,const string& password){
    string uid = trim(userID);
    string pass = trim(password);
    auto user_exists = auth_keys.find(uid);
    if (user_exists != auth_keys.end()) {
    std::cout << "User ID: " << user_exists->first << std::endl;
    std::cout << "Password: " << user_exists->second << std::endl;
} else {
    std::cout << "User not found." << std::endl;
}
        return(user_exists!= auth_keys.end() && user_exists->second == pass);
}

}
