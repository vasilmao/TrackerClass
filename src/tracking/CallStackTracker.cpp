#include "CallStackTracker.hpp"
#include "Logger.hpp"
#include <cstring>


std::string CallStackTracker::PrettifyFuncnameForHTML(const char* str) {
    std::string ans = "";
    for (size_t i = 0; i < strlen(str); ++i) {
        switch (str[i])
        {
        case '<':
            ans += "&lt;";
            break;
        
        case '>':
            ans += "&gt;";
            break;
        
        default:
            ans += str[i];
            break;
        }
    } 
    return ans;
}

CallStackTracker::CallStackTracker(const char* func_name) : func_name_(func_name) {
    std::string to_print = "<font color=\"green\">Entering</font> ";

    to_print += PrettifyFuncnameForHTML(func_name_);
    to_print += '\n';
    Logger::GetInstance() << to_print;
    Logger::AddTab();
}

CallStackTracker::~CallStackTracker() {
    Logger::RemoveTab();
    std::string to_print = "<font color=\"red\">Exiting</font> ";
    to_print += PrettifyFuncnameForHTML(func_name_);
    to_print += '\n';
    Logger::GetInstance() << to_print;
    // not deleting pointer func_name_
}