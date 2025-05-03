#include "Configuration_adapter.hpp"
#include <map>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IObject.h>
#include <sdbus-c++/Types.h>


using BaseConfigurationAdapter = sdbus::AdaptorInterfaces<com::system::configurationManager::Application::Configuration_adaptor>;

// Overrides generated abstract adaptor for interface ( define methods )
class ConfigurationApplicationAdapter : public BaseConfigurationAdapter {
public:
	ConfigurationApplicationAdapter(sdbus::IConnection& connection, const std::string& object_path) :
		BaseConfigurationAdapter(connection, std::move(object_path)) {}
	
	void ChangeConfiguration(const std::string& key, const sdbus::Variant& value) override {
		// TODO: implement logic
	}
	std::map<std::string, sdbus::Variant> GetConfiguration() override {
		// TODO: implement logic
		return {};
	}
};