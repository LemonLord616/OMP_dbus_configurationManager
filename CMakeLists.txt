cmake_minimum_required(VERSION 3.10)
project(configurationManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(GenerateSDBusInterfaces)

find_package(sdbus-c++ 1.3.0 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(src)
add_subdirectory(apps/confManagerApplication1)

add_custom_target(service DEPENDS configurationManager)
add_custom_target(client DEPENDS confManagerApplication1)