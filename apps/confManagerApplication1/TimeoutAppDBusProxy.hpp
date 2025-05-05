#pragma once

#include "Configuration_proxy.hpp"
#include "TimeoutApp.hpp"
#include <memory>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

using BaseConfigurationProxy =
    sdbus::ProxyInterfaces<com::system::configurationManager::Application::Configuration_proxy>;

class TimeoutAppDBusProxy : public BaseConfigurationProxy {
	std::weak_ptr<TimeoutApp> app_;

public:
	TimeoutAppDBusProxy(std::shared_ptr<TimeoutApp> app, sdbus::IConnection &connection,
			    const std::string &service_name, const std::string &object_path);
	~TimeoutAppDBusProxy();

	void onConfigurationChanged(const std::map<std::string, sdbus::Variant> &configuration) override;
};