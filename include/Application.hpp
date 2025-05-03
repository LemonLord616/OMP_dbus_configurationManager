#pragma once

#include <nlohmann/json.hpp>
#include <string>

class Application {
	std::string name_;
	nlohmann::json data_;
public:
	Application(std::string name, nlohmann::json data) :
		name_(std::move(name)), data_(std::move(data)) {}

	const std::string& name() const noexcept { return name_; }
	// GetConfiguration
	const nlohmann::json& data() const noexcept  { return data_; }

	void changeConfiguration(std::string key, nlohmann::json value) {
		data_[std::move(key)] = std::move(value);
	}

	// Add more methods for more functionality
};