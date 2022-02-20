cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(
  -O2
  -g

  #-fsanitize=address

  -Werror
  -Wall
  -Wextra
  -Wpedantic
  -DLOGGING
  -fno-elide-constructors
)

set(INCLUDE_DIR include
                include/tracking
                include/logging)

set(SRC_FILES src/main.cpp
              src/logging/Logger.cpp
              src/logging/GraphDumper.cpp
              src/tracking/CallStackTracker.cpp)

project(tracking)

add_executable(tracking ${SRC_FILES})

target_include_directories(tracking PUBLIC ${INCLUDE_DIR})