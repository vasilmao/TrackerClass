kek.out: Makefile main.cpp.o CallStackTracker.cpp.o Logger.cpp.o GraphDumper.cpp.o 
	g++  -o bin/kek.out obj/main.cpp.o obj/CallStackTracker.cpp.o obj/Logger.cpp.o obj/GraphDumper.cpp.o 

main.cpp.o: src/main.cpp include/tracking/Tracker.hpp \
 include/logging/GraphDumper.hpp include/tracking/CallStackTracker.hpp \
 include/logging/Logger.hpp include/tracking/CallStackTracker.hpp \
 include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -fno-elide-constructors -c ./src/main.cpp -o obj/main.cpp.o

CallStackTracker.cpp.o: src/tracking/CallStackTracker.cpp \
 include/tracking/CallStackTracker.hpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -fno-elide-constructors -c ./src/tracking/CallStackTracker.cpp -o obj/CallStackTracker.cpp.o

Logger.cpp.o: src/logging/Logger.cpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -fno-elide-constructors -c ./src/logging/Logger.cpp -o obj/Logger.cpp.o

GraphDumper.cpp.o: src/logging/GraphDumper.cpp \
 include/logging/GraphDumper.hpp Makefile
	g++ -I . -I ./include -I ./include/tracking -I ./include/logging -I ./src -I ./src/tracking -I ./src/logging -fno-elide-constructors -c ./src/logging/GraphDumper.cpp -o obj/GraphDumper.cpp.o

