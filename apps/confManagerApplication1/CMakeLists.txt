add_executable(confManagerApplication1
	main.cpp
	TimeoutApp.cpp
	TimeoutAppDBusProxy.cpp
)
target_include_directories(confManagerApplication1
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/generated
)
target_link_libraries(confManagerApplication1
	PRIVATE SDBusCpp::sdbus-c++
		nlohmann_json::nlohmann_json
)
add_dependencies(confManagerApplication1 GenerateSDBusInterfaces)