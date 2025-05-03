cmake_minimum_required(VERSION 3.10)
project(configurationManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(GenerateSDBusInterfaces)

find_package(sdbus-c++ 1.3.0 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(configurationManager 
	src/main.cpp
	src/ConfigurationManagerService.cpp
	src/ApplicationDBusAdapter.cpp
)

target_link_libraries(configurationManager
	PRIVATE 
		SDBusCpp::sdbus-c++
		nlohmann_json::nlohmann_json
)

target_include_directories(configurationManager
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
		${GENERATED_PATH}
)