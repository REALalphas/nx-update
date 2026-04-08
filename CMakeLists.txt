cmake_minimum_required(VERSION 3.13)

project(nx-updater)

# Basic definitions
set(APP_TITLE "NX Updater")
set(APP_AUTHOR "Open Source")
set(APP_VERSION "1.0.0")

# If we are cross-compiling for Switch
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Generic" AND NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    # For local building (e.g., using Github Actions we will pass -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Switch.cmake)
    message(STATUS "Not building for Switch natively without toolchain, but we will proceed. To build for Switch, use devkitpro's CMake toolchain.")
endif()

# We need C++17 at least
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Add the main executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

if(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    # Link against libnx and borealis
    target_link_libraries(${PROJECT_NAME} PRIVATE borealis nx)

    # Generate NRO
    include(SwitchNRO)
    add_nro_target(${PROJECT_NAME}
        APP_TITLE ${APP_TITLE}
        APP_AUTHOR ${APP_AUTHOR}
        APP_VERSION ${APP_VERSION}
    )
else()
    # Mocking environment for standard build (to check syntax locally if needed)
    message(STATUS "Building for Host OS, linking standard libraries.")
    # No external libraries linked for mocked build since they might not be installed
endif()
