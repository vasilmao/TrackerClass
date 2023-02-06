#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

class StrangeClass;

namespace std {
    string to_string(StrangeClass& sc);
}

class StrangeClass {
  private:
    std::vector<int> a;

  public:
    StrangeClass() : a(1000, 0) {
        for (int  i = 0; i < 1000; ++i)  {
            a[i] = rand();
        }
    }
    StrangeClass(const StrangeClass& other) {
        a = other.a;
        std::cout << "Oh no!!! It is copying :(" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    StrangeClass(StrangeClass&& other) {
        std::cout << "Yes!! Move!! exactly what is needed" << std::endl;
        std::swap(a, other.a);
    }
    StrangeClass& operator=(const StrangeClass& other) {
        std::cout << "Assignment.. Oh no!!! It is copying :(" << std::endl;
        a = other.a;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return *this;
    }
    StrangeClass& operator=(StrangeClass&& other) {
        std::cout << "Assignment... Move!! exactly what is needed" << std::endl;
        std::swap(a, other.a);
        return *this;
    }
    friend std::string std::to_string(StrangeClass& sc);
};

namespace std {
    inline string to_string(StrangeClass& sc) {
        string ans = "";
        for (int i = 0; i < 3; ++i) {
            ans += to_string(sc.a[i]);
            ans += ",";
        }
        ans += "...";
        return ans;
    }
}

#include "Tracker.hpp"
#include "CallStackTracker.hpp"
#include "GraphDumper.hpp"
#include "Logger.hpp"
#include "SingletonHandler.hpp"



template<class T>
void slow_swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template<class T>
void fast_swap(T& a, T& b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

int main() {
    // srand (time(NULL));
    HANDLE_SINGLETONS
    TRACK_CALL
    Tracker<StrangeClass> a{StrangeClass{}, "a", ""};
    Tracker<StrangeClass> b{StrangeClass{}, "b", ""};
    // slow_swap(a, b);
    fast_swap(a, b);
}