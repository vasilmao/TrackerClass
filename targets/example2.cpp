#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

#include "GraphDumper.hpp"
#include "Logger.hpp"
#include "SingletonHandler.hpp"
#include "CallStackTracker.hpp"

class B {
  public:
    int* big_array;

    B() {
        TRACK_CALL
        big_array = new int[100];
        std::cout << "created B with " << reinterpret_cast<uint64_t>(big_array) << std::endl;
    }

    B(const B&  other) {
        TRACK_CALL
        big_array = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array[i] = other.big_array[i];
        }
        std::cout << "LOOOONG copying......." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    B(B&& other) {
        TRACK_CALL
        big_array = other.big_array;
        other.big_array = nullptr;
        std::cout << "fast move!!!1" << std::endl;
    }

    B& operator=(const B&  other) {
        TRACK_CALL
        big_array = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array[i] = other.big_array[i];
        }
        std::cout << "LOOOONG copying......." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return *this;
    }

    B& operator=(B&& other) {
        TRACK_CALL
        std::swap(big_array, other.big_array);
        std::cout << "fast move!!!2" << std::endl;
        return *this;
    }

    ~B() {
        TRACK_CALL
        std::cout << "deleted B with " << reinterpret_cast<uint64_t>(big_array) << std::endl;
        if (big_array != nullptr) {
            delete[] big_array;
        }
    }
};

class A {
  public:
    int* big_array_too = nullptr;

    A() {
        big_array_too = nullptr;
    }

    A(const B& b) {
        TRACK_CALL
        big_array_too = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array_too[i] = b.big_array[i];
        }
        Logger::GetInstance() << "<font color=\"red\">LOOOONG copying.......</font>\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    A(B&& b) {
        TRACK_CALL
        big_array_too = b.big_array;
        b.big_array = nullptr;
        Logger::GetInstance() << "<font color=\"#7FFFD4\">fast move!!!</font>\n";
    }
    A(A&& other) {
        TRACK_CALL
        std::swap(big_array_too, other.big_array_too);
    }

    A(const A& other) {
        TRACK_CALL
        big_array_too = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array_too[i] = other.big_array_too[i];
        }
        Logger::GetInstance() << "<font color=\"red\">LOOOONG copying....... from another A</font>\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    A& operator=(const A& other) {
        if (big_array_too != nullptr) {
            delete big_array_too;
        }
        big_array_too = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array_too[i] = other.big_array_too[i];
        }
        Logger::GetInstance() << "<font color=\"red\">LOOOONG copying....... from another A</font>\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return *this;
    }
    A& operator=(A&& other) {
        std::swap(big_array_too, other.big_array_too);
        Logger::GetInstance() << "<font color=\"#7FFFD4\">fast move!!! from another A</font>\n";
        return *this;
    }
    ~A() {
        TRACK_CALL
        if (big_array_too != nullptr) {
            delete[] big_array_too;
        }
    }
};

namespace std {
    inline string to_string(A& a) {
        string answer = "A{ ptr = ";
        answer += to_string(reinterpret_cast<unsigned long long>(a.big_array_too));
        answer += "}";
        return answer;
    }

    inline string to_string(B& b) {
        string answer = "B{ ptr = ";
        answer += to_string(reinterpret_cast<unsigned long long>(b.big_array));
        answer += "}";
        return answer;
    }
}

#include "Tracker.hpp"
#include "mystd.hpp"

template<typename T>
A CreateA_move(T&& other) {
    TRACK_CALL
    return A{my_move(other)};
}

template<typename T>
A CreateA_forward(T&& other) {
    TRACK_CALL
    std::cout << "eee\n";
    return A{my_forward<T>(other)};
}

int main() {
    HANDLE_SINGLETONS
    TRACK_CALL


#define CREATE_B(var, value) Tracker<B> var(value, std::string(#var), "")
#define CREATE_A(var, value) Tracker<A> var(value, std::string(#var), "")

#ifdef MISTAKE
    CREATE_B(b, B{});
    b.GetObject().big_array[0] = 1;
    CREATE_A(a, CreateA_move(b.GetObject()));
    // we expect b is still valid
    std::cout << "trying to get the value\n";
    std::cout << b.GetObject().big_array[0] << std::endl; // segfault, because in constructor we put nullptr into  dying object
    std::cout << "move: created A.big_array_too = " << a.GetObject().big_array_too << std::endl;
    std::cout << "move: still existing B.big_array = " << b.GetObject().big_array << std::endl;
#endif
    // B b1;
    std::cout << "c...\n";
    CREATE_B(b1, B{});
    // A a1{CreateA_forward(b1)};
    std::cout << "how...\n";
    CREATE_A(a1, CreateA_forward(b1.GetObject()));
    std::cout << "forward: created A.big_array_too = " << reinterpret_cast<uint64_t>(a1.GetObject().big_array_too) << std::endl;
    std::cout << "forward: still existing B.big_array = " << reinterpret_cast<uint64_t>(b1.GetObject().big_array) << std::endl;

    CREATE_A(a2, CreateA_forward(B{}));
    std::cout << "forward: created A.big_array_too = " << a2.GetObject().big_array_too << std::endl;

    CREATE_A(a3, my_forward<B>(B{}));
    std::cout << "forward: created A.big_array_too = " << a3.GetObject().big_array_too << std::endl;
}