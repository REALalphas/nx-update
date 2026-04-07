cmake_minimum_required(VERSION 3.10)
project(nx-updater)

if(NOT DEFINED BUILD_SWITCH)
    set(BUILD_SWITCH OFF)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_SWITCH)
    if(NOT DEFINED DEVKITPRO)
        set(DEVKITPRO $ENV{DEVKITPRO})
    endif()
    include(${DEVKITPRO}/cmake/Switch.cmake)
endif()

# JSON using FetchContent to do it "right" without local files or submodules
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Borealis
add_subdirectory(external/borealis)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)

if(BUILD_SWITCH)
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
