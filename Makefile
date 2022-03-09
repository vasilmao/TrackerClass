all: bin/main.out bin/example2.out bin/example1.out 

bin/main.out: Makefile obj/main.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 
	g++ -o bin/main.out obj/main.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 

bin/example2.out: Makefile obj/example2.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 
	g++ -o bin/example2.out obj/example2.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 

bin/example1.out: Makefile obj/example1.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 
	g++ -o bin/example1.out obj/example1.o obj/CallStackTracker.o obj/Logger.o obj/GraphDumper.o 

obj/CallStackTracker.o: src/tracking/CallStackTracker.cpp \
 include/tracking/CallStackTracker.hpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./src/tracking/CallStackTracker.cpp -o obj/CallStackTracker.o

obj/Logger.o: src/logging/Logger.cpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./src/logging/Logger.cpp -o obj/Logger.o

obj/GraphDumper.o: src/logging/GraphDumper.cpp \
 include/logging/GraphDumper.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./src/logging/GraphDumper.cpp -o obj/GraphDumper.o

obj/main.o: targets/main.cpp include/tracking/Tracker.hpp \
 include/logging/GraphDumper.hpp include/tracking/CallStackTracker.hpp \
 include/logging/Logger.hpp include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./targets/main.cpp -o obj/main.o

obj/example2.o: targets/example2.cpp include/logging/GraphDumper.hpp \
 include/logging/Logger.hpp include/SingletonHandler.hpp \
 include/tracking/CallStackTracker.hpp include/tracking/Tracker.hpp \
 include/tracking/CallStackTracker.hpp include/mystd.hpp \
 include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./targets/example2.cpp -o obj/example2.o

obj/example1.o: targets/example1.cpp include/tracking/Tracker.hpp \
 include/logging/GraphDumper.hpp include/tracking/CallStackTracker.hpp \
 include/logging/Logger.hpp include/tracking/CallStackTracker.hpp \
 include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -std=c++17 -c ./targets/example1.cpp -o obj/example1.o

