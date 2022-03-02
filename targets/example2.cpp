#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

class Empty {
    int a = 5;
  public:
    Empty(){
        std::cout << "Empty constructor" << std::endl;
    }
    Empty(const Empty& other) {
        std::cout << "Oh no!!! Copying :(" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    Empty(Empty&& other) {
        std::cout << "Move!! :)" << std::endl;
    }
    ~Empty() {
        std::cout << "Destructor" << std::endl;
    }
};

template<typename T>
using NOREF = typename std::remove_reference<T>::type;

template<typename T>
NOREF<T>* create_copy(T&& obj) {
    return new NOREF<T>(obj);
}

int main() {
    Empty a;
    Empty* p1 = create_copy(a);
    Empty* p2 = create_copy(Empty{});
}