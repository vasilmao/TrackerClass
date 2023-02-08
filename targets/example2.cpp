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
    int* big_array = nullptr;

    B() {
        big_array = nullptr;
    }

    B(int how_much) {
        TRACK_CALL
        std::cout << "ouf " << big_array << "\n";
        big_array = new int[how_much];
        std::cout << "ugh " << big_array << "\n";
        std::cout << "created B with " << reinterpret_cast<uint64_t>(big_array) << std::endl;
    }

    B(const B&  other) {
        TRACK_CALL
        std::cout << "b, copy\n";
        std::cout << other.big_array << "\n";
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
        std::cout << "LOOOONG copying......." << std::endl;
        big_array = new int[100];
        for (int i = 0; i < 100; ++i) {
            big_array[i] = other.big_array[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return *this;
    }

    B& operator=(B&& other) {
        TRACK_CALL
        std::cout << "fast move!!!2" << std::endl;
        std::swap(big_array, other.big_array);
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

namespace std {

    inline string to_string(B& b) {
        string answer = "ptr = ";
        answer += to_string(reinterpret_cast<unsigned long long>(b.big_array));
        // answer += "}";
        return answer;
    }
}

#include "Tracker.hpp"
#include "mystd.hpp"

template<typename T>
Tracker<B> CreateB_move(T&& other) {
    TRACK_CALL
    return Tracker<B>{my_move(other)};
}

template<typename T>
Tracker<B> CreateB_forward(T&& other) {
    TRACK_CALL
    std::cout << "eee\n";
    return Tracker<B>{my_forward<T>(other)};
}

int main() {
    HANDLE_SINGLETONS
    TRACK_CALL


#define CREATE_B(var, value) Tracker<B> var(value, std::string(#var), "")
// #define CREATE_A(var, value) Tracker<A> var(value, std::string(#var), "")

#ifdef MISTAKE
    CREATE_B(b, B{100});
    b.GetObject().big_array[0] = 1;
    CREATE_B(a, CreateB_move(b));
    // we expect b is still valid
    std::cout << "trying to get the value\n";
    std::cout << b.GetObject().big_array[0] << std::endl; // segfault, because in constructor we put nullptr into  dying object
    std::cout << "move: created A.big_array_too = " << a.GetObject().big_array << std::endl;
    std::cout << "move: still existing B.big_array = " << b.GetObject().big_array << std::endl;
#endif
    // B b1;
    std::cout << "c...\n";
    CREATE_B(b1, B{100});
    // A a1{CreateB_forward(b1)};
    std::cout << "how...\n";
    std::cout << b1.GetObject().big_array << "\n";
    CREATE_B(b2, CreateB_forward(b1));
    std::cout << "forward: created A.big_array_too = " << reinterpret_cast<uint64_t>(b2.GetObject().big_array) << std::endl;
    std::cout << "forward: still existing B.big_array = " << reinterpret_cast<uint64_t>(b1.GetObject().big_array) << std::endl;

    CREATE_B(b3, CreateB_forward(B{100}));
    std::cout << "forward: created A.big_array_too = " << b3.GetObject().big_array << std::endl;

    CREATE_B(b4, my_forward<B>(B{100}));
    std::cout << "forward: created A.big_array_too = " << b4.GetObject().big_array << std::endl;
}