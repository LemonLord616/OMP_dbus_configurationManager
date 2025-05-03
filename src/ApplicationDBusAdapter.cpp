#include "ApplicationDBusAdapter.hpp"
#include "Application.hpp"
#include <cstdint>
#include <iostream>


ApplicationDBusAdapter::ApplicationDBusAdapter(std::unique_ptr<Application> app,
	sdbus::IConnection& connection, const std::string& object_path)
	: app_(std::move(app)), BaseConfigurationAdapter(connection, std::move(object_path)) {
		registerAdaptor();
}
ApplicationDBusAdapter::~ApplicationDBusAdapter() {
	unregisterAdaptor();
}

static std::map<std::string, sdbus::Variant>
makeVariantMap(const nlohmann::json& data){
	std::map<std::string, sdbus::Variant> map;
	for (auto it = data.begin(); it != data.end(); ++it) {
		const auto& key = it.key();
		const auto& val = it.value();
		if (val.is_string()) map[key] = val.get<std::string>();
		else if (val.is_number_integer()) map[key] = val.get<int>();
		else if (val.is_number_float()) map[key] = val.get<double>();
		else if (val.is_boolean()) map[key] = val.get<bool>();
	}
	return map;
} 

void ApplicationDBusAdapter::ChangeConfiguration(const std::string& key, const sdbus::Variant& value) {
	nlohmann::json newValue;
	if (value.containsValueOfType<int16_t>())	newValue = value.get<int16_t>();
	else if (value.containsValueOfType<uint16_t>())	newValue = value.get<uint16_t>();
	else if (value.containsValueOfType<int32_t>())	newValue = value.get<int32_t>();
	else if (value.containsValueOfType<uint32_t>())	newValue = value.get<uint32_t>();
	else if (value.containsValueOfType<int64_t>())	newValue = value.get<int64_t>();
	else if (value.containsValueOfType<uint64_t>())	newValue = value.get<uint64_t>();
	else if (value.containsValueOfType<double>())	newValue = value.get<double>();
	else if (value.containsValueOfType<bool>())		newValue = value.get<bool>();
	else if (value.containsValueOfType<std::string>())	newValue = value.get<std::string>();
	else
		std::cerr << "Unsupported type for key '" + key + "'" << std::endl;

	app_->changeConfiguration(key, std::move(newValue));
	emitConfigurationChanged(makeVariantMap(app_->data()));
}

std::map<std::string, sdbus::Variant> ApplicationDBusAdapter::GetConfiguration() {
	return makeVariantMap(app_->data());
}