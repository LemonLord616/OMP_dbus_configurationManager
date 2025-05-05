#pragma once

#include "Application.hpp"
#include "Configuration_adapter.hpp"
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>

using BaseConfigurationAdapter =
    sdbus::AdaptorInterfaces<com::system::configurationManager::Application::Configuration_adaptor>;

// Overrides generated abstract adaptor for interface ( define methods )
class ApplicationDBusAdapter : public BaseConfigurationAdapter {
	std::unique_ptr<Application> app_;

public:
	ApplicationDBusAdapter(std::unique_ptr<Application> app, sdbus::IConnection &connection,
			       const std::string &object_path);
	~ApplicationDBusAdapter();

	void ChangeConfiguration(const std::string &key, const sdbus::Variant &value) override;
	std::map<std::string, sdbus::Variant> GetConfiguration() override;
};