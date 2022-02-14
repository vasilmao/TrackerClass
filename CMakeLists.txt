cmake_minimum_required(VERSION 3.16.3)

project(TrackerClass)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(kek src/main.cpp)

target_include_directories(kek PUBLIC include/kek
                                      include/tracker)


# target_link_libraries(my_sfml PUBLIC sfml-graphics)

target_link_libraries(kek PUBLIC sfml-graphics)

# add_library(liba STATIC liba/zalupa.cpp)

# target_link_libraries(kek PUBLIC liba)