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
* **Client adapter** (`ApplicationDBusAdapter`) generated via `sdbus-c++-xml2cpp` and extended to implement business logic.

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
├── 📂 build
├── 📂 cache
├── 📂 cmake
│ └── GenerateSDBusInterfaces.cmake
├── 📂 include
│ ├── Application.hpp
│ ├── ApplicationDBusAdapter.hpp
│ └── ConfigurationManagerService.hpp
├── 📂 interfaces
│ └── com.system.configurationManager.Application.Configuration.xml
├── 📂 src
│ ├── 📂 generated
│ │ ├── Configuration_adapter.hpp
│ │ └── Configuration_proxy.hpp
│ ├── ApplicationDBusAdapter.cpp
│ ├── ConfigurationManagerService.cpp
│ └── main.cpp
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