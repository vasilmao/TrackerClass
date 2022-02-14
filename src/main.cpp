#include "Tracker.hpp"
#include <SFML/Graphics.hpp>

using MyInt = Tracker<int>;

MyInt Add(MyInt a, MyInt b) {
    return a + b;
}

MyInt Sub(MyInt a, MyInt b) {
    return a - b;
}

int main() {
    CREATEINT(x1, 1);
    CREATEINT(x2, 1);
    CREATEINT(x3, 1);
    CREATEINT(x4, 1);

    MyInt summ = Add(x1 + x2, x3 + x4);
    MyInt diff = Sub(x1 + x2, x3 + x4);

    CREATEINT(what, summ * diff);
    what++;
    what += diff;
}