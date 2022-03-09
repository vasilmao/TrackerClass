#include "CallStackTracker.hpp"
#include "Logger.hpp"

CallStackTracker::CallStackTracker(const char* func_name) : func_name_(func_name) {
    std::string to_print = "<font color=\"green\">Entering</font> ";
    to_print += func_name_;
    to_print += "\n";
    Logger::GetInstance() << to_print;
    Logger::AddTab();
}

CallStackTracker::~CallStackTracker() {
    Logger::RemoveTab();
    std::string to_print = "<font color=\"red\">Exiting</font> ";
    to_print += func_name_;
    to_print += "\n";
    Logger::GetInstance() << to_print;
    // not deleting pointer func_name_
}