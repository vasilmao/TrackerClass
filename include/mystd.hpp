#include "Tracker.hpp"
#include "GraphDumper.hpp"
#include "Logger.hpp"
#include "SingletonHandler.hpp"

template<typename T>
struct my_remove_reference {
    typedef T type;
};

template<typename T>
struct my_remove_reference<T&> {
    typedef T type;
};

template<typename T>
struct my_remove_reference<T&&> {
    typedef T type;
};

template<typename T>
typename my_remove_reference<T>::type&& my_move(T&& obj) {
    TRACK_CALL
    Logger::GetInstance() << "MOVE\n";
    return static_cast<typename my_remove_reference<T>::type&&>(obj);
}

#include <iostream>

template<typename T>
T&& my_forward(typename my_remove_reference<T>::type& obj) {
    TRACK_CALL
    std::cout << "yeee\n";
    Logger::GetInstance() << "FORWARD: lvalue\n";
    return static_cast<T&&>(obj);
}

template<typename T>
T&& my_forward(typename my_remove_reference<T>::type&& obj) {
    TRACK_CALL
    std::cout << "yeee1\n";
    Logger::GetInstance() << "FORWARD: rvalue\n";
    return static_cast<T&&>(obj);
}
