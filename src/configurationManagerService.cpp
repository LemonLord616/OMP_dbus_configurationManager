#include "configurationManagerService.hpp"
#include "configurationApplicationAdapter.hpp"
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>


bool ConfigurationManagerService::isValidObjectPath(const std::string& str) const {
	static const std::regex pattern{R"(^\/([A-Za-z0-9_]+\/)*[A-Za-z0-9_]+$)"};
	return std::regex_match(service_path, pattern);
}

ConfigurationManagerService::ConfigurationManagerService(
	sdbus::IConnection& connection, const std::string& service_path)
: connection_(connection), service_path(service_path) {
	if (!isValidObjectPath(service_path))
		throw std::invalid_argument("Service path does not match pattern");
}

void ConfigurationManagerService::parseConfig(const std::string& config_path) const {
	parseConfig(std::filesystem::path(config_path)); 
}
void ConfigurationManagerService::parseConfig(const std::filesystem::path& config_path) const {
	if (!std::filesystem::exists(config_path)
	|| !std::filesystem::is_directory(config_path)) {
		throw std::invalid_argument("Config path does not exists: " + config_path.string());
	}

	for (const auto& entry : std::filesystem::directory_iterator(config_path)) {
		if (!entry.is_regular_file()) continue;

		auto path = entry.path();
		if (path.extension() != ".json") continue;

		std::ifstream file(path);
		nlohmann::json json;
		file >> json;
		
		App app;

		auto app_name = path.stem().string();
		app.name = app_name;
		app.data = std::move(json);

		const std::string object_path = service_path + "/Application/" + app_name;
		if (!isValidObjectPath(object_path)) {
			throw std::runtime_error("Object path " + object_path + " is invalid");
			std::cerr << "Object path " + object_path + " is invalid, skipping..." << std::endl;
			continue;
		}
		if (apps_.find(app_name) == apps_.end()) {
			std::cerr << "Duplicate config for " << app_name << ", skipping..." << std::endl;
			continue;
		}

		// TODO: create object
	}
}
