set(SDBUS_INTROSPECTION_XML ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/com.system.configurationManager.Application.Configuration.xml)

set(GENERATED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/generated)
file(MAKE_DIRECTORY ${GENERATED_PATH})

add_custom_command(
	OUTPUT
		${GENERATED_PATH}/Configuration_adapter.hpp
		${GENERATED_PATH}/Configuration_proxy.hpp
	COMMAND
		sdbus-c++-xml2cpp
		${SDBUS_INTROSPECTION_XML}
		--adaptor=${GENERATED_PATH}/Configuration_adapter.hpp
		--proxy=${GENERATED_PATH}/Configuration_proxy.hpp
	DEPENDS
		${SDBUS_INTROSPECTION_XML}
	COMMENT
		"Generating D-Bus interfaces"
)

add_custom_target(GenerateSDBusInterfaces
	DEPENDS
		${GENERATED_PATH}/Configuration_adapter.hpp
		${GENERATED_PATH}/Configuration_proxy.hpp
)