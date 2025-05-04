add_executable(confManagerApplication1
	main.cpp
	TimeoutApp.cpp
)
target_include_directories(confManagerApplication1
	PRIVATE ${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/generated
)
target_link_libraries(confManagerApplication1
	PRIVATE SDBusCpp::sdbus-c++
		nlohmann_json::nlohmann_json
)
add_dependencies(confManagerApplication1 GenerateSDBusInterfaces)