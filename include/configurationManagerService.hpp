#pragma once

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/sdbus-c++.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <map>
#include <memory>
#include <string>


struct App {
	std::string name;
	nlohmann::json data;
	std::unique_ptr<sdbus::IObject> object_ptr;
};


class ConfigurationManagerService {
	sdbus::IConnection& connection_;
	const std::string& service_path;
	std::map<std::string, std::unique_ptr<App>> apps_;
	// std::vector<unique_ptr<App>> apps_;

	bool isValidObjectPath(const std::string& str) const;
public:
	ConfigurationManagerService(
		sdbus::IConnection& connection,
		const std::string& service_path
	);
	void parseConfig(const std::string& config_path) const;
	void parseConfig(const std::filesystem::path& config_path) const;
};