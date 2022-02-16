#ifndef GRAPH_DUMPER_HPP
#define GRAPH_DUMPER_HPP

#include <fstream>
#include <iostream>
#include <string>

class GraphDumper { // singleton
  private:
    static GraphDumper* instance_;
    std::ofstream file_;

  private:
    GraphDumper();
    ~GraphDumper();

  public:
    static GraphDumper& GetInstance();
    static void Release();

    GraphDumper& operator<<(const std::string& str) {
        file_ << str;
        return *this;
    }
};

#endif