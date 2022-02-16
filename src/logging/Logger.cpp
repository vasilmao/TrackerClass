#include "Logger.hpp"

#include <iomanip>
#include <ctime>

Logger* Logger::instance_ = nullptr;
int Logger::tabs_cnt_ = 0;

const std::string log_file_name = "log.html";

Logger::Logger() {
    file_.open(log_file_name, std::ofstream::out);
}

Logger::~Logger() {
    file_.close();
}

Logger& Logger::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new Logger{};
        (*instance_) << "<!DOCTYPE html>\n<html>\n<body>\n<pre>\n";
        (*instance_) << "<font color=\"blue\">log started ";

        time_t rawtime;
        struct tm * timeinfo;
        char buffer[100] = {};

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S</font>\n",timeinfo);
        std::string time_str(buffer);

        (*instance_) << time_str;
    }
    return *instance_;
}

void Logger::Release() {
    if (instance_ != nullptr) {
        (*instance_) << "<font color=\"blue\">log finished ";

        time_t rawtime;
        struct tm * timeinfo;
        char buffer[100] = {};

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S</font>\n",timeinfo);
        std::string time_str(buffer);

        (*instance_) << time_str;

        (*instance_) << "</pre>\n</body>\n</html>\n";

        delete instance_;
    }
}

void Logger::AddTab() {
    tabs_cnt_++;
}

void Logger::RemoveTab() {
    tabs_cnt_--;
}