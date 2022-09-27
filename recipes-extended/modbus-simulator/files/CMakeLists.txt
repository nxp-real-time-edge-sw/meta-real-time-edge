cmake_minimum_required(VERSION 3.10)

project(modbus-simulator VERSION 1.0.0 LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MODBUS REQUIRED IMPORTED_TARGET libmodbus)

add_executable(modbus_client_simulator "${CMAKE_CURRENT_SOURCE_DIR}/modbus_client_simulator.c")
target_sources(modbus_client_simulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common.c")

target_link_libraries(modbus_client_simulator PkgConfig::MODBUS)

add_executable(modbus_device_simulator "${CMAKE_CURRENT_SOURCE_DIR}/modbus_device_simulator.c")
target_sources(modbus_device_simulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common.c")

target_link_libraries(modbus_device_simulator PkgConfig::MODBUS)

install(TARGETS modbus_device_simulator modbus_client_simulator DESTINATION ${CMAKE_INSTALL_BINDIR}
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
