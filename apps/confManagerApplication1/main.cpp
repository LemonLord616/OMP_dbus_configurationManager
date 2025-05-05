#include "TimeoutApp.hpp"
#include "TimeoutAppDBusProxy.hpp"
#include <memory>
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>


int main() {
	const std::string config_file = "~/com.system.configurationManager/confManagerApplication1.json";
	const std::string service_name = "com.system.configurationManager";
	const std::string object_path = "/com/system/configurationManager/Application/confManagerApplication1";

	auto app = std::make_shared<TimeoutApp>();
	app->parseConfig(config_file);
	std::cout << "Config loaded: " + config_file << std::endl;

	auto connection = sdbus::createSessionBusConnection();
	std::cout << "DBus connection created" << std::endl;
	TimeoutAppDBusProxy proxy(app, *connection, service_name, object_path);
	std::cout << "Proxy created" << std::endl;

	connection->enterEventLoopAsync();
	app->run();

	return 0;
}