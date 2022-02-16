#ifndef SINGLETON_HANDLER
#define SINGLETON_HANDLER

#include "GraphDumper.hpp"

class SingletonDestroyer {
  public:
    ~SingletonDestroyer() {
        GraphDumper::Release();
    }
};

#define HANDLE_SINGLETONS SingletonDestroyer SINGLETONDESTROYER{};

#endif