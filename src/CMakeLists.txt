add_executable(configurationManager 
	main.cpp
	ConfigurationManagerService.cpp
	ApplicationDBusAdapter.cpp
)
target_include_directories(configurationManager
	PRIVATE ${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/generated
)
target_link_libraries(configurationManager
	PRIVATE 
		SDBusCpp::sdbus-c++
		nlohmann_json::nlohmann_json
)
add_dependencies(configurationManager GenerateSDBusInterfaces)