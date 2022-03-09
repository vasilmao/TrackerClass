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

the slow version:
![slow_swap](https://user-images.githubusercontent.com/34096563/157378743-78fd316d-2243-4d7d-9b53-a086d1b46de9.svg)

the "fast" version:
![fast_swap](https://user-images.githubusercontent.com/34096563/157378883-8a33cbd3-9935-4e49-8a52-4a4d8ab0f50a.svg)


## example2.cpp
This is the example of wrong move usage and how forward fixes it
![graph](https://user-images.githubusercontent.com/34096563/157386767-9a5142eb-b3ce-4732-a679-906bca2dd02b.svg)
The illustration (It is not fair because you can't spawn graph when your program gets segfault, this graph was created when ptr was not zeroed in move and default contructor. The b variable has zero at after move. Other chains of variable are correct.)

### my own remove reference, move, forward
mystd.hpp have my own versions remove_reference, move and forward written for educational purposes
