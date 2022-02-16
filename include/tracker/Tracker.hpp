#include <string>

#include "GraphDumper.hpp"

#define GRAPH_DUMP_BINARY_OPERATOR(obj1, operator, obj2, result) \
    int interm_node = CreateIntermediateNode(#operator);         \
    SetEdge(obj1.index_, interm_node);                           \
    SetEdge(obj2.index_, interm_node);                           \
    SetEdge(interm_node, result.index_);

#define GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR(obj1, operator, obj2) \
    int interm_node = CreateIntermediateNode(#operator);              \
    SetEdge(obj1.index_, interm_node, "left operand");                \
    SetEdge(obj2.index_, interm_node, "right operand");               \
    int new_index = node_indexer_++;                                  \
    obj1.index_ = new_index;                                          \
    SetEdge(interm_node, obj1.index_);                                \
    LogName();

template<class T>
class Tracker {
  private:
    inline static int anon_var_cnt_ = 0;
    inline static int node_indexer_ = 0;
    int index_ = 0;
    T object_;
    std::string var_name_;
  public:
    Tracker<T>(const T& object) : object_(object) {
        index_ = node_indexer_++;
        SetAnonVarName();
        LogName();
    }
    Tracker<T>(T&& object)      : object_(object) {
        index_ = node_indexer_++;
        SetAnonVarName();
        LogName();
    }

    // Tracker<T>(T object, const std::string& var_name)        : object_(object), var_name_(var_name) {
    //     LogName();
    // }
    Tracker<T>(const T& object, const std::string& var_name) : object_(object), var_name_(var_name) {
        index_ = node_indexer_++;
        LogName();
    }
    Tracker<T>(T&& object, const std::string& var_name)      : object_(object), var_name_(var_name) {
        index_ = node_indexer_++;
        // std::string log_message = "created ";
        // log_message += var_name_;
        // log_message += " that is equals half-dead (real integer) ";
        // log_message += std::to_string(object);
        LogName();
    }

    Tracker<T>(const Tracker<T>& other, const std::string& var_name) : object_(other.object_), var_name_(var_name) {
        index_ = node_indexer_++;
        // std::string log_message = "created ";
        // log_message += var_name_;
        // log_message += " that is equals ";
        // log_message += other.var_name_;
        LogName();
        SetEdgeFrom(other, ":=");
    }

    Tracker<T>(Tracker<T>&& other, const std::string& var_name)      : object_(other.object_), var_name_(var_name) {
        index_ = node_indexer_++;
        // std::string log_message = "created ";
        // log_message += var_name_;
        // log_message += " that is equals half-dead ";
        // log_message += other.var_name_;
        LogName();
        SetEdgeFrom(other, "ctor rvalue ref");
    }

    Tracker<T>(const Tracker<T>& other) : object_(other.object_) {
        index_ = node_indexer_++;
        SetAnonVarName();
        SetEdgeFrom(other, ":=");
        LogName();
    }
    Tracker<T>(Tracker<T>&& other)      : object_(other.object_) {
        index_ = node_indexer_++;
        SetAnonVarName();
        SetEdgeFrom(other, "ctor rvalue ref");
        LogName();
    }

    Tracker<T>& operator=(const Tracker<T>& other) {
        object_ = other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), =, other)
        return *this;
    }

    Tracker<T>& operator=(Tracker<T>&& other) {
        std::swap(object_, other.object_);
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), =, other)
        return *this;
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
        Tracker<T> result(object_ + other.object_);
        GRAPH_DUMP_BINARY_OPERATOR((*this), +, other, result)
        return result;
    }

    Tracker<T> operator-(const Tracker<T>& other) {
        Tracker<T> result(object_ - other.object_);
        GRAPH_DUMP_BINARY_OPERATOR((*this), -, other, result)
        return result;
    }

    Tracker<T> operator*(const Tracker<T>& other) {
        Tracker<T> result(object_ * other.object_);
        GRAPH_DUMP_BINARY_OPERATOR((*this), *, other, result)
        return result;
    }

    Tracker<T> operator/(const Tracker<T>& other) {
        Tracker<T> result(object_ / other.object_);
        GRAPH_DUMP_BINARY_OPERATOR((*this), /, other, result)
        return result;
    }

    Tracker<T>& operator++() {
        ++object_;
        return *this;
    }

    Tracker<T>& operator++(int) {
        object_++;
        return *this;
    }

    Tracker<T>& operator--() {
        --object_;
        return *this;
    }

    Tracker<T>& operator--(int) {
        object_--;
        return *this;
    }

    Tracker<T>& operator+=(const Tracker<T>& other) {
        object_ += other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), +=, other)
        return *this;
    }

    Tracker<T>& operator-=(const Tracker<T>& other) {
        object_ -= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), -=, other)
        return *this;
    }

    Tracker<T>& operator*=(const Tracker<T>& other) {
        object_ *= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), *=, other)
        return *this;
    }

    Tracker<T>& operator/=(const Tracker<T>& other) {
        object_ /= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), /=, other)
        return *this;
    }

  private:
    
    void SetAnonVarName() {
        anon_var_cnt_++;
        var_name_ = "anonymous variable #" + std::to_string(anon_var_cnt_);
    }

    void LogName() {
        // file << var_name_ << std::endl;
        // el009BBBA8 [style=filled,fillcolor="#cfe8ff",label="OOP"];
        // el0        [shape=record,label="{{<f0> phys pos:\n 0} | { <f1>prev:\n 0 | value:\n 1.#QNAN0 | <f2> next:\n 0}}"style=filled,fillcolor="#ff8080"]
        // ellog1     [shape=record,label="{{logical_pos: 0 |<f0> phys pos:\n 1} | { <f1>prev:\n 0 | value:\n 0.100000 | <f2> next:\n 2}}"]
        std::string var_str = "\tel";
        var_str += std::to_string(index_);
        var_str += " [shape=record,label=\"{{<f0> ";
        var_str += var_name_;
        var_str += "} | { val: ";
        var_str += std::to_string(object_);
        // var_str += " | index: ";
        // var_str += std::to_string(index_);
        var_str += " }}\"];\n";
        GraphDumper::GetInstance() << var_str;
    }

    int CreateIntermediateNode(const std::string& label) {
        int node_index = node_indexer_++;
        std::string node_str = "\tel";
        node_str += std::to_string(node_index);
        node_str += " [style=filled,fillcolor=\"#cfe8ff\",label=\"";
        node_str += label;
        node_str += "\"];\n";
        GraphDumper::GetInstance() << node_str;
        return node_index;
    }

    void SetEdge(int from, int to, const std::string& label = "") {
        std::string message = "\tel";
        message += std::to_string(from);
        message += " -> el";
        message += std::to_string(to);
        message += " [label=\"";
        message += label;
        message += "\"];\n";
        GraphDumper::GetInstance() << message;
    }

    void SetEdge(const Tracker<T>& from, const Tracker<T>& to, const std::string& label = "") {
        SetEdge(from.index_, to.index_, label);
    }

    void SetEdgeFrom(const Tracker<T>& other, const std::string& label = "") {
        SetEdge(other, *this, label);
    }

    void SetEdgeTo(const Tracker<T>& other, const std::string& label = "") {
        SetEdge(*this, other, label);
    }
};

#define CREATEINT(var, value) Tracker<int> var(value, std::string(#var));