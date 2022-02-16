#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

class Logger { // singleton
  private:
    static Logger* instance_;
    std::ofstream file_;
    static int tabs_cnt_;

  private:
    Logger();
    ~Logger();

  public:
    static Logger& GetInstance();
    static void Release();
    static void AddTab();
    static void RemoveTab();

    Logger& operator<<(const std::string& str) {
        file_ << std::string(tabs_cnt_ * 4, ' ') << str;
        return *this;
    }
};

#endif