#include "TimeoutAppDBusProxy.hpp"
#include "TimeoutApp.hpp"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <stdexcept>
#include <string>

TimeoutAppDBusProxy::TimeoutAppDBusProxy(std::shared_ptr<TimeoutApp> app, sdbus::IConnection &connection,
					 const std::string &service_name, const std::string &object_path)
    : app_(app), BaseConfigurationProxy(connection, service_name, object_path) {
	registerProxy();
}

TimeoutAppDBusProxy::~TimeoutAppDBusProxy() {
	unregisterProxy();
}

void TimeoutAppDBusProxy::onConfigurationChanged(const std::map<std::string, sdbus::Variant> &configuration) {
	std::cout << "Caught signal: ConfigurationChanged" << std::endl;
	auto a = app_.lock();
	if (!a)
		throw std::runtime_error("App is not running");

	auto timeoutIt = configuration.find("Timeout");
	uint32_t timeout_ms = a->getTimeoutMs();
	if (timeoutIt != configuration.end() && timeoutIt->second.containsValueOfType<uint32_t>())
		timeout_ms = timeoutIt->second.get<uint32_t>();

	auto timeoutPhraseIt = configuration.find("TimeoutPhrase");
	std::string timeout_phrase = a->getTimeoutPhrase();
	if (timeoutPhraseIt != configuration.end() && timeoutPhraseIt->second.containsValueOfType<std::string>())
		timeout_phrase = timeoutPhraseIt->second.get<std::string>();

	a->changeConfiguration(timeout_ms, timeout_phrase);
}