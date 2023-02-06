all: Makefile a.out example1.out example2.out 

a.out: Makefile obj/main.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o 
	g++ obj/main.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o  -o a.out 

example1.out: Makefile obj/example1.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o 
	g++ obj/example1.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o  -o example1.out 

example2.out: Makefile obj/example2.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o 
	g++ obj/example2.o obj/Logger.o obj/GraphDumper.o obj/CallStackTracker.o  -o example2.out 

obj/Logger.o: src/logging/Logger.cpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c ./src/logging/Logger.cpp -o obj/Logger.o

obj/GraphDumper.o: src/logging/GraphDumper.cpp \
 include/logging/GraphDumper.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c ./src/logging/GraphDumper.cpp -o obj/GraphDumper.o

obj/CallStackTracker.o: src/tracking/CallStackTracker.cpp \
 include/tracking/CallStackTracker.hpp include/logging/Logger.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c ./src/tracking/CallStackTracker.cpp -o obj/CallStackTracker.o

obj/main.o: targets/main.cpp include/tracking/Tracker.hpp \
 include/logging/GraphDumper.hpp include/tracking/CallStackTracker.hpp \
 include/logging/Logger.hpp include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c targets/main.cpp -o obj/main.o

obj/example1.o: targets/example1.cpp include/tracking/Tracker.hpp \
 include/logging/GraphDumper.hpp include/tracking/CallStackTracker.hpp \
 include/logging/Logger.hpp include/tracking/CallStackTracker.hpp \
 include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c targets/example1.cpp -o obj/example1.o

obj/example2.o: targets/example2.cpp include/logging/GraphDumper.hpp \
 include/logging/Logger.hpp include/SingletonHandler.hpp \
 include/tracking/CallStackTracker.hpp include/tracking/Tracker.hpp \
 include/tracking/CallStackTracker.hpp include/mystd.hpp \
 include/SingletonHandler.hpp Makefile
	g++ -I . -I ./include -I ./include/logging -I ./include/tracking -I ./targets -I ./src -I ./src/logging -I ./src/tracking -I ./obj -Wall -Werror -c targets/example2.cpp -o obj/example2.o

