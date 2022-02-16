#include "GraphDumper.hpp"

GraphDumper* GraphDumper::instance_ = nullptr;

const std::string log_file_name = "graph.txt";

GraphDumper::GraphDumper() {
    file_.open(log_file_name, std::ofstream::out);
}

GraphDumper::~GraphDumper() {
    file_.close();
}

GraphDumper& GraphDumper::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new GraphDumper{};
        (*instance_) << "digraph structs {\n\trankdir=HR; outputOrder=nodesfirst;\n";
    }
    return *instance_;
}

void GraphDumper::Release() {
    if (instance_ != nullptr) {
        (*instance_) << "}\n";
        delete instance_;
    }
}