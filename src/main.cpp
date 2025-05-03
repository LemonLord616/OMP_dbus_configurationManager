#include <sdbus-c++/sdbus-c++.h>
#include <iostream>

int main() 
{
	
	constexpr const char* service_name = "com.example.service";
	constexpr const char* object_path = "/com/example/configurationManager/Object";
	constexpr const char* interface_name = "com.example.service.testInterface";

	
	auto connection = sdbus::createSessionBusConnection(service_name);
	
	auto object = sdbus::createObject(*connection, object_path);

	object->registerMethod("testMethod")
		.onInterface(interface_name)
		.implementedAs([](){ return "Hello World!"; }); 

	object->finishRegistration();

	std::cout << "Service is running..." << std::endl;
	connection->enterEventLoop();

	return 0;
}