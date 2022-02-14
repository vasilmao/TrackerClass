#include <string>
#include <fstream>
#include <iostream>

template<class T>
class Tracker {
  private:
    inline static int anon_var_cnt_ = 0;
    T object_;
    std::string var_name_;
  public:
    // Tracker<T>(T object)        : object_(object) {
    //     SetAnonVarName();
    //     LogMessage(var_name_);
    // }
    Tracker<T>(const T& object) : object_(object) {
        SetAnonVarName();
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals (real integer) ";
        log_message += std::to_string(object);
        LogMessage(log_message);
    }
    Tracker<T>(T&& object)      : object_(object) {
        SetAnonVarName();
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals half-dead (real integer) ";
        log_message += std::to_string(object);
        LogMessage(log_message);
    }

    // Tracker<T>(T object, const std::string& var_name)        : object_(object), var_name_(var_name) {
    //     LogMessage(var_name_);
    // }
    Tracker<T>(const T& object, const std::string& var_name) : object_(object), var_name_(var_name) {
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals (real integer) ";
        log_message += std::to_string(object);
        LogMessage(log_message);
    }
    Tracker<T>(T&& object, const std::string& var_name)      : object_(object), var_name_(var_name) {
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals half-dead (real integer) ";
        log_message += std::to_string(object);
        LogMessage(log_message);
    }

    Tracker<T>(const Tracker<T>& other, const std::string& var_name) : object_(other.object_), var_name_(var_name) {
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals ";
        log_message += other.var_name_;
        LogMessage(log_message);
    }

    Tracker<T>(Tracker<T>&& other, const std::string& var_name)      : object_(other.object_), var_name_(var_name) {
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals half-dead ";
        log_message += other.var_name_;
        LogMessage(log_message);
    }

    Tracker<T>(const Tracker<T>& other) : object_(other.object_) {
        SetAnonVarName();
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals ";
        log_message += other.var_name_;
        LogMessage(log_message);
    }
    Tracker<T>(Tracker<T>&& other)      : object_(other.object_) {
        SetAnonVarName();
        std::string log_message = "created ";
        log_message += var_name_;
        log_message += " that is equals half-dead ";
        log_message += other.var_name_;
        LogMessage(log_message);
    }

    Tracker<T>& operator=(const Tracker<T>& other) {
        std::string log_str = var_name_;
        log_str += " is now equals ";
        log_str += other;
        LogMessage(log_str);
        object_ = other.object_;
    }

    Tracker<T>& operator=(Tracker<T>&& other) {
        std::string log_str = var_name_;
        log_str += " is now equals half-dead ";
        log_str += other;
        LogMessage(log_str);
        std::swap(object_, other.object_);
    }

    bool operator<(const Tracker<T>& other) {
        return object_ < other.object_;
    }

    bool operator>(const Tracker<T>& other) {
        return object_ > other.object_;
    }

    bool operator<=(const Tracker<T>& other) {
        return object_ <= other.object_;
    }

    bool operator>=(const Tracker<T>& other) {
        return object_ >= other.object_;
    }

    bool operator==(const Tracker<T>& other) {
        return object_ == other.object_;
    }

    Tracker<T> operator+(const Tracker<T>& other) {
        anon_var_cnt_++;
        std::cout << anon_var_cnt_ << std::endl;
        std::string new_name = "anonymous var #";
        new_name += std::to_string(anon_var_cnt_);
        new_name += ", result of (";
        new_name += var_name_;
        new_name += ") + (";
        new_name += other.var_name_;
        new_name += ")";
        return Tracker<T>(object_ + other.object_, new_name);
    }

    Tracker<T> operator-(const Tracker<T>& other) {
        anon_var_cnt_++;
        std::string new_name = "anonymous var #";
        new_name += std::to_string(anon_var_cnt_);
        new_name += ", result of (";
        new_name += var_name_;
        new_name += ") - (";
        new_name += other.var_name_;
        new_name += ")";
        return Tracker<T>(object_ - other.object_, new_name);
    }

    Tracker<T> operator*(const Tracker<T>& other) {
        anon_var_cnt_++;
        std::string new_name = "anonymous var #";
        new_name += std::to_string(anon_var_cnt_);
        new_name += ", result of (";
        new_name += var_name_;
        new_name += ") * (";
        new_name += other.var_name_;
        new_name += ")";
        return Tracker<T>(object_ * other.object_, new_name);
    }

    Tracker<T> operator/(const Tracker<T>& other) {
        anon_var_cnt_++;
        std::string new_name = "anonymous var #";
        new_name += std::to_string(anon_var_cnt_);
        new_name += ", result of (";
        new_name += var_name_;
        new_name += ") / (";
        new_name += other.var_name_;
        new_name += ")";
        return Tracker<T>(object_ / other.object_, new_name);
    }

    Tracker<T>& operator++() {
        std::string log_message = var_name_;
        log_message += " left-incremented";
        LogMessage(log_message);

        ++object_;
        return *this;
    }

    Tracker<T>& operator++(int) {
        std::string log_message = var_name_;
        log_message += " right-incremented";
        LogMessage(log_message);

        object_++;
        return *this;
    }

    Tracker<T>& operator--() {
        std::string log_message = var_name_;
        log_message += " left-decremented";
        LogMessage(log_message);

        --object_;
        return *this;
    }

    Tracker<T>& operator--(int) {
        std::string log_message = var_name_;
        log_message += " right-decremented";
        LogMessage(log_message);

        object_--;
        return *this;
    }

    Tracker<T>& operator+=(const Tracker<T>& other) {
        std::string log_message = var_name_;
        log_message += " increased(+=) by value of ";
        log_message += other.var_name_;
        LogMessage(log_message);
        
        object_ += other.object_;
        return *this;
    }

    Tracker<T>& operator-=(const Tracker<T>& other) {
        std::string log_message = var_name_;
        log_message += " decreased(-=) by value of ";
        log_message += other.var_name_;
        LogMessage(log_message);

        object_ -= other.object_;
        return *this;
    }

    Tracker<T>& operator*=(const Tracker<T>& other) {
        std::string log_message = var_name_;
        log_message += " mulled(*=) by value of ";
        log_message += other.var_name_;
        LogMessage(log_message);

        object_ *= other.object_;
        return *this;
    }

    Tracker<T>& operator/=(const Tracker<T>& other) {
        std::string log_message = var_name_;
        log_message += " divided(/=) by value of ";
        log_message += other.var_name_;
        LogMessage(log_message);

        object_ /= other.object_;
        return *this;
    }

  private:
    
    void SetAnonVarName() {
        anon_var_cnt_++;
        var_name_ = "anonymous variable #" + std::to_string(anon_var_cnt_);
    }

    void SetCopyVarName(const std::string& copy_name) {
        anon_var_cnt_++;
        var_name_ = "anonymous variable #";
        var_name_ += std::to_string(anon_var_cnt_);
        var_name_ += ", copy of (";
        var_name_ += copy_name;
        var_name_ += ")";
    }

    void LogName() {
        std::ofstream file;
        file.open("log.txt", std::ofstream::out | std::ofstream::app);
        file << var_name_ << std::endl;
        file.close();
    }

    void LogMessage(const std::string& message) {
        std::ofstream file;
        file.open("log.txt", std::ofstream::out | std::ofstream::app);
        file << message << std::endl;
        file.close();
    }

};

#define CREATEINT(var, value) Tracker<int> var(value, std::string(#var));