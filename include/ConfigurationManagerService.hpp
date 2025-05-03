#pragma once

#include "ApplicationDBusAdapter.hpp"
#include <sdbus-c++/sdbus-c++.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <map>
#include <memory>
#include <string>


class ConfigurationManagerService {
	sdbus::IConnection& connection_;
	const std::string& service_path;
	std::map<const std::string, std::unique_ptr<ApplicationDBusAdapter>> adapters_;
public:
	ConfigurationManagerService(
		sdbus::IConnection& connection,
		const std::string& service_path
	);
	void parseConfig(const std::string& config_path);
	void parseConfig(const std::filesystem::path& config_path);
};