# D-Bus Configuration Manager

This project implements a **D-Bus**-based configuration manager in C++ using **sdbus-c++** and **nlohmann/json**. It contains a core service that creates application configuration objects over D-Bus, and generated adapters/proxies for easy integration.

## Features

* **Dynamic configuration loading** from JSON files in `~/com.system.configurationManager/` directory.
* **D-Bus service** (`com.system.configurationManager`) registers per-application objects at paths like `/com/system/configurationManager/Application/<appName>`.
* **Methods**:

	* `ChangeConfiguration(s key, v value)` — change a single configuration parameter.
	* `GetConfiguration()` — get the full configuration as a map of `<string, variant>`.
* **Signal**:

	* `configurationChanged(a{sv} configuration)` — emitted whenever a configuration changes.
* **Adapter/Proxy** (`Configuration_adapter.hpp` and `Configuration_proxy.hpp`) generated via `sdbus-c++-xml2cpp` and extended to implement business logic.
* **Sample Client** (`TimeoutApp`) that subscribes to `configurationChanged` signal.

## Prerequisites

* Linux (Debian-based recommended)
* **CMake** ≥ 3.10
* **C++17** compiler (GCC, Clang)
* **sdbus-c++** library and headers (e.g. `libsdbus-c++-dev`)
* **nlohmann/json** library (e.g. `nlohmann-json3-dev`)
* **libglib2.0-bin** (for `gdbus`) to introspect and call methods

## Repository Structure

```
📦
│
├── 📂 apps/confManagerApplication1
│ ├── 📂 configs
│ │ └── confManagerApplication1.json
│ ├── CMakeLists.txt
│ ├── main.cpp
│ ├── TimeoutApp.cpp
│ ├── TimeoutApp.hpp
│ ├── TimeoutAppDBusProxy.cpp
│ └── TimeoutAppDBusProxy.hpp
├── 📂 cmake
│ └── GenerateSDBusInterfaces.cmake
├── 📂 configs
│ ├── music_player.json
│ ├── notes_app.json
│ └── weather_app.json
├── 📂 included
│ ├── Configuration_adapter.hpp
│ └── Configuration_proxy.hpp
├── 📂 include
│ ├── Application.hpp
│ ├── ApplicationDBusAdapter.hpp
│ └── ConfigurationManagerService.hpp
├── 📂 interfaces
│ └── com.system.configurationManager.Application.Configuration.xml
├── 📂 src
│ ├── ApplicationDBusAdapter.cpp
│ ├── CMakeLists.txt
│ ├── ConfigurationManagerService.cpp
│ └── main.cpp
├── .clang-format
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Build Instructions

1. **Clone** the repository:

	```bash
	git clone https://github.com/LemonLord616/OMP_dbus_configurationManager.git
	cd OMP_dbus_configurationManager
	```

2. **Install** dependencies:

	```bash
	sudo apt update
	sudo apt install cmake libsdbus-c++-dev nlohmann-json3-dev libglib2.0-bin
	```

3. **Configure the project**:

	```bash
	mkdir -p build && cd build
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=0N
	cmake ..
	```

4. **Build service or client selectively**
	* Build **only the service**:

		```bash
		cmake --build . --target service
		```
	* Build **only the client application**:

		```bash
		cmake --build . --target client
		```
	* Build **everything (default)**:

		```bash
		cmake --build .
		```


> ⚙️ **Note:**
> Generated adapter/proxy headers are already present in `./generated` and committed to this repository, so regeneration is optional. To manually regenerate after updating the XML, run:
>
> ```bash
> make GenerateSDBusInterfaces
> ```

## Running the Service

1. **Prepare configuration directory** (one-time):

	```bash
	mkdir -p ~/com.system.configurationManager
	cp configs/com.system.configurationManager/*.json ~/com.system.configurationManager/
	```

2. **Start** the D-Bus service:

	```bash
	./build/src/configurationManager
	```

	The service will read all `*.json` files in the config folder and register corresponding D-Bus objects.

## Sample client

A sample client (`confManagerApplication1`) is located at `apps/confManagerApplication1`. It:

1. **Reads its config from `~/com.system.configurationManager/confManagerApplication1.json`**

2. **Contains values `Timeout` and `TimeoutPhrase`**

3. **Subscribes to the `configurationChanged` signal**

4. **Prints `TimeoutPhrase` every `Timeout` milliseconds**

## Running sample client

1. **Prepare configuration directory**:

	```bash
	mkdir -p ~/com.system.configurationManager
	cp apps/confManagerApplication1/configs/*.json ~/com.system.configurationManager/
	```

2. **Start**:

	```bash
	./build/apps/confManagerApplication1/confManagerApplication1
	```

## Introspection & Method Calls

* **List services**:

	```bash
	gdbus list --session
	```
* **Introspect an object**:

	```bash
	gdbus introspect --session \
		--dest com.system.configurationManager \
		--object-path /com/system/configurationManager/Application/music_player
	```
* **Call a method** (e.g., get config):

	```bash
	gdbus call --session \
		--dest com.system.configurationManager \
		--object-path /com/system/configurationManager/Application/music_player \
		--method com.system.configurationManager.Application.Configuration.GetConfiguration
	```
* **Change a parameter**:

	```bash
	gdbus call --session \
		--dest com.system.configurationManager \
		--object-path /com/system/configurationManager/Application/music_player \
		--method com.system.configurationManager.Application.Configuration.ChangeConfiguration \
		'"volume"' 'uint32 60'
	```

## Usage example

* **In terminal 1**, service active process.

* **In terminal 2**, the `confManagerApplication1` is running, which displays the current `TimeoutPharse` once every `Timeout` ms.

* **In terminal 3**, run the command

	```bash
	gdbus call --session \
	--dest com.system.configurationManager \
	--object-path /com/system/configurationManager/Application/confManagerApplication1 \
	--method com.system.configurationManager.Application.Configuration.ChangeConfiguration \
	'"TimeoutPhrase"' "<'Please stop me'>"
	```

* **Terminal 1** should start displaying the phrase `"Please stop me`.
