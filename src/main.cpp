#include "ConfigurationManagerService.hpp"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

int main() {

	const std::string service_name = "com.system.configurationManager";
	const std::string service_path = "/com/system/configurationManager";
	const std::string config_path = "~/com.system.configurationManager";

	auto connection = sdbus::createSessionBusConnection(service_name);
	std::cout << "Connection created" << std::endl;

	auto service = ConfigurationManagerService(*connection, service_path);

	service.parseConfig(config_path);

	std::cout << "Service started" << std::endl;
	std::cout << "Service is running..." << std::endl;
	connection->enterEventLoop();

	return 0;
}