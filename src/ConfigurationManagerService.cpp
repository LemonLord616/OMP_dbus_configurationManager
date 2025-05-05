#include "ConfigurationManagerService.hpp"
#include "ApplicationDBusAdapter.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <stdexcept>

const std::filesystem::path expandUserPath(const std::string &path) {
	if (!path.empty() && path[0] == '~') {
		const char *home = getenv("HOME");
		if (home) {
			return std::filesystem::path(home + path.substr(1));
		}
	}
	return std::filesystem::path(path);
}

static bool isValidObjectPath(const std::string &str) {
	static const std::regex pattern{R"(^(\~\/|\/)([A-Za-z0-9_.]+\/)*[A-Za-z0-9_.]+$)"};
	return std::regex_match(str, pattern);
}

ConfigurationManagerService::ConfigurationManagerService(sdbus::IConnection &connection,
							 const std::string &service_path)
    : connection_(connection), service_path(service_path) {
	if (!isValidObjectPath(service_path))
		throw std::invalid_argument("Service path does not match pattern: " + service_path);
}

void ConfigurationManagerService::parseConfig(const std::string &config_path) {
	if (!isValidObjectPath(config_path))
		throw std::invalid_argument("Config path does not match pattern: " + config_path);
	parseConfig(expandUserPath(config_path));
}
void ConfigurationManagerService::parseConfig(const std::filesystem::path &config_path) {
	if (!std::filesystem::exists(config_path) || !std::filesystem::is_directory(config_path)) {
		throw std::invalid_argument("Config path does not exists: " + config_path.string());
	}

	for (const auto &entry : std::filesystem::directory_iterator(config_path)) {
		if (!entry.is_regular_file())
			continue;

		auto path = entry.path();
		if (path.extension() != ".json")
			continue;

		std::ifstream file(path);
		nlohmann::json json;
		file >> json;

		const std::string app_name = path.stem().string();

		auto app = std::make_unique<Application>(app_name, std::move(json));

		const std::string object_path = service_path + "/Application/" + app_name;
		if (!isValidObjectPath(object_path)) {
			throw std::runtime_error("Object path " + object_path + " is invalid");
			// std::cerr << "Object path " + object_path + " is invalid, skipping..." << std::endl;
			// continue;
		}
		if (adapters_.find(app_name) != adapters_.end()) {
			std::cerr << "Duplicate config for " << app_name << ", skipping..." << std::endl;
			continue;
		}

		auto adapter = std::make_unique<ApplicationDBusAdapter>(std::move(app), connection_, object_path);
		adapters_[app_name] = std::move(adapter);
		std::cout << "Created Object " + app_name + " at " + object_path << std::endl;
	}
}
