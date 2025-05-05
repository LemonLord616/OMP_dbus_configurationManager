#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <sdbus-c++/sdbus-c++.h>

class TimeoutApp {
	uint32_t timeout_ms;
	std::string timeout_phrase;

public:
	uint32_t getTimeoutMs();
	std::string getTimeoutPhrase();
	// uint32_t setTimeoutMs();
	// std::string setTimeoutPhrase();

	bool running_;
	TimeoutApp();
	void parseConfig(const std::string &config_file_path);
	void parseConfig(const std::filesystem::path &config_file_path);

	void run();
	// void stop();

	void changeConfiguration(int timeout_ms, const std::string &timeout_phrase);
};