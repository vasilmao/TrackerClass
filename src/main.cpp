#include "Tracker.hpp"
#include "CallStackTracker.hpp"
#include "GraphDumper.hpp"
#include "Logger.hpp"
#include "SingletonHandler.hpp"
#include <SFML/Graphics.hpp>

using MyInt = Tracker<int>;

MyInt Add(MyInt a, MyInt b) {
    TRACK_CALL
    return a + b;
}

MyInt Sub(MyInt a, MyInt b) {
    TRACK_CALL
    return a - b;
}

int main() {
    HANDLE_SINGLETONS
    TRACK_CALL

    CREATEINT(x1, 1);
    CREATEINT(x2, 11);
    CREATEINT(x3, 2);
    CREATEINT(x4, 22);

    CREATEINT(summ1, Add(x1, x2));
    CREATEINT(summ2, Add(x3, x4));

    CREATEINT(what, summ1 * summ2);

    what += 5; // implicit ctor

    x3 += 7;

    what /= x3;

}