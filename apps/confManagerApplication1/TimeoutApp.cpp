#include "TimeoutApp.hpp"
#include <chrono>
#include <cstdint>
#include <functional>
#include <regex>
#include <fstream>
#include <csignal>
#include <iostream>
#include <thread>


const std::filesystem::path expandUserPath(const std::string& path) {
	if (!path.empty() && path[0] == '~') {
		const char* home = getenv("HOME");
		if (home) {
			return std::filesystem::path(home + path.substr(1));
		}
	}
	return std::filesystem::path(path);
}

static bool isValidFilePath(const std::string& str) {
	static const std::regex pattern{R"(^(\~\/|\/)([A-Za-z0-9_.]+\/)*[A-Za-z0-9_.]+$)"};
	return std::regex_match(str, pattern);
}

uint32_t TimeoutApp::getTimeoutMs() { return timeout_ms; }
std::string TimeoutApp::getTimeoutPhrase() { return timeout_phrase; }

TimeoutApp::TimeoutApp() : timeout_ms(1000), timeout_phrase("placeholder"), running_(false) {}

void TimeoutApp::parseConfig(const std::string& config_file_path) {
	if (!isValidFilePath(config_file_path))
		throw std::invalid_argument("Config path does not match pattern: " + config_file_path);
	parseConfig(expandUserPath(config_file_path)); 
}

void TimeoutApp::parseConfig(const std::filesystem::path& config_file_path) {
	if (!std::filesystem::exists(config_file_path) || 
		!std::filesystem::is_regular_file(config_file_path)) {
		throw std::invalid_argument("Config file does not exist or is not a regular file: " + config_file_path.string());
	}
	
	std::ifstream file(config_file_path);
	if (!file) {
		throw std::runtime_error("Failed to open config file: " + config_file_path.string());
	}
	
	nlohmann::json jsonConfig;
	try {
		file >> jsonConfig;
	} catch (const std::exception& e) {
		throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
	}
	
	if (jsonConfig.contains("Timeout") && jsonConfig["Timeout"].is_number()) {
		timeout_ms = jsonConfig["Timeout"];
	}
	if (jsonConfig.contains("TimeoutPhrase") && jsonConfig["TimeoutPhrase"].is_string()) {
		timeout_phrase = jsonConfig["TimeoutPhrase"];
	}
}

std::function<void()> shutdown_handler;
void signal_handler() { if(shutdown_handler) shutdown_handler(); }
void TimeoutApp::run() {
	running_ = true;

	std::signal(SIGINT, [](int){ signal_handler(); });

	shutdown_handler = [&](){ running_ = false; };

	while (running_) {
		std::cout << timeout_phrase << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
	}
	std::cout << "Stopping..." << std::endl;
}

void TimeoutApp::changeConfiguration(int timeout_ms, const std::string& timeout_phrase) {
	this->timeout_ms = timeout_ms;
	this->timeout_phrase = timeout_phrase;
}