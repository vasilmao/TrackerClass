#ifndef SINGLETON_HANDLER
#define SINGLETON_HANDLER

#include "GraphDumper.hpp"
#include "Logger.hpp"

class SingletonDestroyer {
  public:
    ~SingletonDestroyer() {
        GraphDumper::Release();
        Logger::Release();
    }
};

#define HANDLE_SINGLETONS SingletonDestroyer SINGLETONDESTROYER{};

#endif