cmake_minimum_required(VERSION 3.14)

project(nx-update C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure the NRO properties
set(APP_TITLE "nx-update")
set(APP_AUTHOR "Author")
set(APP_VERSION "1.0.0")

# Set build type to Release by default if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Download Borealis (XITRIX fork)
include(FetchContent)
FetchContent_Declare(
    borealis
    GIT_REPOSITORY https://github.com/XITRIX/borealis.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(borealis)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link Borealis
target_link_libraries(${PROJECT_NAME} PRIVATE borealis::borealis)

# Generate NRO manually using elf2nro and nacptool
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nacp
    COMMAND nacptool --create "${APP_TITLE}" "${APP_AUTHOR}" "${APP_VERSION}" ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nacp
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro
    COMMAND elf2nro $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro --nacp=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nacp --romfsdir=${CMAKE_SOURCE_DIR}/romfs
    DEPENDS ${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nacp
)

add_custom_target(
    nro ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro
)
