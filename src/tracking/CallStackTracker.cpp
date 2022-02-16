#include "CallStackTracker.hpp"
#include "Logger.hpp"

CallStackTracker::CallStackTracker(const char* func_name) : func_name_(func_name) {
    Logger::GetInstance() << "Entering " << func_name_ << "\n";
    Logger::AddTab();
}

CallStackTracker::~CallStackTracker() {
    Logger::RemoveTab();
    Logger::GetInstance() << "Exiting " << func_name_ << "\n";

    // not deleting pointer func_name_
}