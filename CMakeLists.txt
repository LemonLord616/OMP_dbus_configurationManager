cmake_minimum_required(VERSION 3.10)
project(configurationManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(sdbus-c++ REQUIRED)

add_executable(configurationManager src/main.cpp)

target_link_libraries(configurationManager PRIVATE SDBusCpp::sdbus-c++)