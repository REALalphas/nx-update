cmake_minimum_required(VERSION 3.14)
project(nx-updater)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if(NOT DEFINED PLATFORM_SWITCH)
    set(PLATFORM_SWITCH OFF)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(PLATFORM_SWITCH)
    if(NOT DEFINED DEVKITPRO)
        set(DEVKITPRO $ENV{DEVKITPRO})
    endif()
    include(${DEVKITPRO}/cmake/Switch.cmake)
endif()

# JSON
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# Borealis integration
# We need to set BOREALIS_PATH for its internal build system
set(BOREALIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/borealis/library)
add_subdirectory(external/borealis/library)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)

if(PLATFORM_SWITCH)
    add_executable(nx-updater.elf ${SOURCE_FILES})
    target_link_libraries(nx-updater.elf
        borealis
        nlohmann_json::nlohmann_json
        nx
        curl
        mbedtls
        mbedx509
        mbedcrypto
        z
        archive
    )

    if(COMMAND add_nro_target)
        add_nro_target(nx-updater.elf
            NAME "NX Updater"
            AUTHOR "REALalphas"
            ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.jpg"
            ROMFS "${CMAKE_CURRENT_SOURCE_DIR}/resources/romfs"
        )
    endif()
else()
    add_executable(nx-updater ${SOURCE_FILES})
    target_link_libraries(nx-updater borealis nlohmann_json::nlohmann_json)
endif()
