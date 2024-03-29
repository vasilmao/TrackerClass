#ifndef TRACKER_T_HPP
#define TRACKER_T_HPP

#include <string>
#include "GraphDumper.hpp"
#include "CallStackTracker.hpp"
#include "Logger.hpp"

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
    GraphDumpObject();

#define GRAPH_DUMP_ASSIGNMENT_COPY_OPERATOR(obj1, operator, obj2)        \
    int interm_node = CreateIntermediateNode(#operator);                 \
    SetEdge(obj1.index_, interm_node, "left operand");                   \
    SetEdge(obj2.index_, interm_node, "right operand (COPY!!!)", "red"); \
    int new_index = node_indexer_++;                                     \
    obj1.index_ = new_index;                                             \
    SetEdge(interm_node, obj1.index_);                                   \
    GraphDumpObject();

#define GRAPH_DUMP_ASSIGNMENT_MOVE_OPERATOR(obj1, operator, obj2)        \
    int interm_node = CreateIntermediateNode(#operator);                 \
    SetEdge(obj1.index_, interm_node, "left operand");                   \
    SetEdge(obj2.index_, interm_node, "right operand (move)", "green");  \
    int new_index = node_indexer_++;                                     \
    obj1.index_ = new_index;                                             \
    SetEdge(interm_node, obj1.index_);                                   \
    GraphDumpObject();

const int colors_cnt = 9;
const std::string colors[colors_cnt] = {"red", "cyan", "blue", "darkblue", "lightblue", "purple", "lime", "magenta", "pink"};

template<class T>
class Tracker {
  private:
    inline static int anon_var_cnt_ = 0;
    inline static int node_indexer_ = 0;
    int index_ = 0;
    T object_;
    std::string var_name_;
    std::string history_ = "";
  public:

    Tracker<T>(const T& object, const std::string& parent_history="") : object_(object) {
        std::cout << "da anon lvalue\n";
        TRACK_CALL
        index_ = node_indexer_++;
        // index_ = TRACKER_COUNTER++;
        SetAnonVarName();
        SetHistory(parent_history);
        GraphDumpObject();
        LogDumpCtor();
    }
    Tracker<T>(T&& object, const std::string& parent_history="") {
        std::cout << "da anon rvalue\n";
        TRACK_CALL
        std::swap(object_, object);
        index_ = node_indexer_++;
        SetAnonVarName();
        SetHistory(parent_history);
        GraphDumpObject();
        LogDumpCtor();
    }

    Tracker<T>(const T& object, const std::string& var_name, const std::string& parent_history) : object_(object) {
        std::cout << "da lvalue from object\n";
        TRACK_CALL
        index_ = node_indexer_++;
        if (var_name.length() == 0) {
            SetAnonVarName();
        } else {
            var_name_ = var_name;
        }
        GraphDumpObject();
        SetHistory(parent_history);
        LogDumpCtor();
    }

    Tracker<T>(T&& object, const std::string& var_name, const std::string& parent_history) {
        std::cout << "da rvalue from object\n";
        TRACK_CALL
        std::swap(object_, object);
        index_ = node_indexer_++;
        // index_ = TRACKER_COUNTER++;
        if (var_name.length() == 0) {
            SetAnonVarName();
        } else {
            var_name_ = var_name;
        }
        GraphDumpObject();
        SetHistory(parent_history);
        LogDumpCtor();
    }


    Tracker<T>(const Tracker<T>& other, const std::string& var_name, const std::string&) : object_(other.object_), var_name_(var_name) {
        std::cout << "da lvalue from object, skipping? history\n";
        TRACK_CALL
        index_ = node_indexer_++;
        // index_ = TRACKER_COUNTER++;
        GraphDumpObject();
        SetEdgeFrom(other, "COPY", "red", "bold");
        SetHistory();
        LogDumpCtor();
    }

    Tracker<T>(Tracker<T>&& other, const std::string& var_name, const std::string&)      : var_name_(var_name) {
        std::cout << "da rvalue from object, skipping? history\n";
        TRACK_CALL
        std::swap(object_, other.object_);
        index_ = node_indexer_++;
        // index_ = TRACKER_COUNTER++;
        GraphDumpObject();
        SetEdgeFrom(other, "move", "green");
        SetRelativeHistory(other);
        LogDumpCtor();
    }

    Tracker<T>(const Tracker<T>& other) : object_(other.object_) {
        std::cout << "da lvalue from tracker without history\n";
        TRACK_CALL
        index_ = node_indexer_++;
        // index_ = TRACKER_COUNTER++;
        SetAnonVarName();
        SetRelativeHistory(other);
        SetEdgeFrom(other, "COPY", "red", "bold");
        GraphDumpObject();
        LogDumpCtor();
    }

    Tracker<T>(Tracker<T>&& other) {
        std::cout << "da rvalue from tracker without history\n";
        TRACK_CALL
        std::swap(object_, other.object_);
        index_ = node_indexer_++;
        SetAnonVarName();
        SetEdgeFrom(other, "move", "green");
        GraphDumpObject();
        SetRelativeHistory(other);
        LogDumpCtor();
    }

    Tracker<T>& operator=(const Tracker<T>& other) {
        std::cout << "da = lvalue\n";
        TRACK_CALL
        object_ = other.object_;
        GRAPH_DUMP_ASSIGNMENT_COPY_OPERATOR((*this), =, other)
        SetRelativeHistory(other);
        LogDumpHistory();
        return *this;
    }

    Tracker<T>& operator=(Tracker<T>&& other) {
        std::cout << "da = rvalue\n";
        TRACK_CALL
        std::swap(object_, other.object_);
        GRAPH_DUMP_ASSIGNMENT_MOVE_OPERATOR((*this), =, other)
        SetRelativeHistory(other);
        LogDumpHistory();
        return *this;
    }

    bool operator<(const Tracker<T>& other) {
        TRACK_CALL
        return object_ < other.object_;
    }

    bool operator>(const Tracker<T>& other) {
        TRACK_CALL
        return object_ > other.object_;
    }

    bool operator<=(const Tracker<T>& other) {
        TRACK_CALL
        return object_ <= other.object_;
    }

    bool operator>=(const Tracker<T>& other) {
        TRACK_CALL
        return object_ >= other.object_;
    }

    bool operator==(const Tracker<T>& other) {
        TRACK_CALL
        return object_ == other.object_;
    }

    Tracker<T> operator+(const Tracker<T>& other) {
        TRACK_CALL
        std::string parental_history = "(";
        parental_history += history_;
        parental_history += ") + (";
        parental_history += other.history_;
        parental_history += ")";
        Tracker<T> result(object_ + other.object_, parental_history);
        GRAPH_DUMP_BINARY_OPERATOR((*this), +, other, result)
        return result;
    }

    Tracker<T> operator-(const Tracker<T>& other) {
        TRACK_CALL
        std::string parental_history = "(";
        parental_history += history_;
        parental_history += ") + (";
        parental_history += other.history_;
        parental_history += ")";
        Tracker<T> result(object_ - other.object_, parental_history);
        GRAPH_DUMP_BINARY_OPERATOR((*this), -, other, result)
        return result;
    }

    Tracker<T> operator*(const Tracker<T>& other) {
        TRACK_CALL
        std::string parental_history = "(";
        parental_history += history_;
        parental_history += ") + (";
        parental_history += other.history_;
        parental_history += ")";
        Tracker<T> result(object_ * other.object_, parental_history);
        GRAPH_DUMP_BINARY_OPERATOR((*this), *, other, result)
        return result;
    }

    Tracker<T> operator/(const Tracker<T>& other) {
        TRACK_CALL
        std::string parental_history = "(";
        parental_history += history_;
        parental_history += ") + (";
        parental_history += other.history_;
        parental_history += ")";
        Tracker<T> result(object_ / other.object_, parental_history);
        GRAPH_DUMP_BINARY_OPERATOR((*this), /, other, result)
        return result;
    }

    Tracker<T>& operator++() {
        TRACK_CALL
        ++object_;
        return *this;
    }

    Tracker<T>& operator++(int) {
        TRACK_CALL
        object_++;
        return *this;
    }

    Tracker<T>& operator--() {
        TRACK_CALL
        --object_;
        return *this;
    }

    Tracker<T>& operator--(int) {
        TRACK_CALL
        object_--;
        return *this;
    }

    Tracker<T>& operator+=(const Tracker<T>& other) {
        TRACK_CALL
        object_ += other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), +=, other)
        return *this;
    }

    Tracker<T>& operator-=(const Tracker<T>& other) {
        TRACK_CALL
        object_ -= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), -=, other)
        return *this;
    }

    Tracker<T>& operator*=(const Tracker<T>& other) {
        TRACK_CALL
        object_ *= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), *=, other)
        return *this;
    }

    Tracker<T>& operator/=(const Tracker<T>& other) {
        TRACK_CALL
        object_ /= other.object_;
        GRAPH_DUMP_COMPOUND_ASSIGNMENT_OPERATOR((*this), /=, other)
        return *this;
    }

    ~Tracker() {
        TRACK_CALL
        std::cout << "Tracker deleted\n";
    }

  private:
    
    void SetAnonVarName() {
        anon_var_cnt_++;
        var_name_ = "ANON #" + std::to_string(anon_var_cnt_);
    }

    void SetHistory() {
        history_ = "<font color=\"";
        history_ += colors[(rand() % colors_cnt)];
        history_ += "\">";
        history_ += var_name_;
        history_ += " = ";
        history_ += std::to_string(object_);
        history_ += "</font>";
    }

    void SetHistory(const std::string& parent_history) {
        // srand (time(NULL));
        if (parent_history == "") {
            SetHistory();
            return;
        }
        history_ = "<font color=\"";
        history_ += colors[(rand() % colors_cnt)];
        history_ += "\">";
        history_ += var_name_;
        history_ += "[";
        history_ += std::to_string(object_);
        history_ += "] = (";
        history_ += parent_history;
        history_ += ")</font> ";
    }

    void SetRelativeHistory(const Tracker<T>& other) {
        // srand (time(NULL));
        history_ = "<font color=\"";
        history_ += colors[(rand() % colors_cnt)];
        history_ += "\">";
        history_ += var_name_;
        history_ += " [";
        history_ += std::to_string(object_);
        history_ += "] = (";
        history_ += other.history_;
        history_ += ")</font>";
    }

    void GraphDumpObject() {
        // el009BBBA8 [style=filled,fillcolor="#cfe8ff",label="OOP"];
        // el0        [shape=record,label="{{<f0> phys pos:\n 0} | { <f1>prev:\n 0 | value:\n 1.#QNAN0 | <f2> next:\n 0}}"style=filled,fillcolor="#ff8080"]
        // ellog1     [shape=record,label="{{logical_pos: 0 |<f0> phys pos:\n 1} | { <f1>prev:\n 0 | value:\n 0.100000 | <f2> next:\n 2}}"]
        std::string var_str = "\tel";
        var_str += std::to_string(index_);
        var_str += " [shape=record,label=\"{{<f0> ";
        var_str += var_name_;
        var_str += "} | { val: ";
        var_str += std::to_string(object_);
        // var_str += " }}\"];\n";
        var_str += " }}\"";
        if (var_name_.compare(0, 6, "ANON #") == 0) {
            var_str += ",style=filled,fillcolor=\"red\"";
        }
        var_str += "];\n";
        GraphDumper::GetInstance() << var_str;
    }

    int CreateIntermediateNode(const std::string& label) {
        int node_index = node_indexer_++;
        // int node_index = TRACKER_COUNTER++;
        std::string node_str = "\tel";
        node_str += std::to_string(node_index);
        node_str += " [style=filled,fillcolor=\"#cfe8ff\",label=\"";
        node_str += label;
        node_str += "\"];\n";
        GraphDumper::GetInstance() << node_str;
        return node_index;
    }

    void SetEdge(int from, int to, const std::string& label = "", const std::string& color = "black", const std::string& style = "") {
        std::string message = "\tel";
        message += std::to_string(from);
        message += " -> el";
        message += std::to_string(to);
        message += " [label=\"";
        message += label;
        message += "\",color=\"";
        message += color;
        message += "\",style=\"";
        message += style;
        message += "\"];\n";
        GraphDumper::GetInstance() << message;
    }

    void SetEdge(const Tracker<T>& from, const Tracker<T>& to, const std::string& label = "", const std::string& color = "black", const std::string& style = "") {
        SetEdge(from.index_, to.index_, label, color, style);
    }

    void SetEdgeFrom(const Tracker<T>& other, const std::string& label = "", const std::string& color = "black", const std::string& style = "") {
        SetEdge(other, *this, label, color, style);
    }

    void SetEdgeTo(const Tracker<T>& other, const std::string& label = "", const std::string& color = "black", const std::string& style = "") {
        SetEdge(*this, other, label, color, style);
    }

    void LogDumpCtor() {
        Logger::GetInstance() << history_ + "\n";
    }

    void LogDumpHistory() {
        Logger::GetInstance() << history_ + "\n";
    }

  public:
    
    T& GetObject() & {
        return object_;
    }

    T&& GetObject() && {
        return object_;
    }
};

#define CREATEINT(var, value) Tracker<int> var(value, std::string(#var), "");

#endif