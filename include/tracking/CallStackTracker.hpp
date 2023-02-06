#ifndef CALL_STACK_TRACKER_HPP
#define CALL_STACK_TRACKER_HPP

#include <string>

class CallStackTracker {
  private:
    const char* func_name_ = nullptr;
    std::string PrettifyFuncnameForHTML(const char* str);
  public:
    CallStackTracker() = delete;
    CallStackTracker(const char* func_name);
    ~CallStackTracker();
};

#define TRACK_CALL CallStackTracker CALL_STACK_TRACKER{__PRETTY_FUNCTION__};

#endif