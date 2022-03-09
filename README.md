# TrackerClass
So, target is to track all constructors of target class T, and get detailed understanding where copy/move constructors called and how much copies created.
Using flag -fno-elide-constructors, we can see additional copies.

## main.cpp
main.cpp: just example of tracker usage, see the log.html. To see graph use graphviz:
```
dot -Tsvg graph.txt > graph
```

## example1.cpp
This just shows how move can be used to speed up the programm. On the graph you can see red arrows of copying and green arrows of moving.

## example2.cpp
This is the example of wrong move usage and how forward fixes it

### my own remove reference, move, forward
mystd.hpp have my own versions remove_reference, move and forward written for educational purposes
