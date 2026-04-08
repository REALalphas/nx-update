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

# We need libnx tools
find_package(libnx REQUIRED)

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

# Need at least one source file to define the target
if(NOT SOURCES)
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp" "int dummy() { return 0; }")
    list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link Borealis (this brings in libnx, glad, glfw etc. depending on platform)
target_link_libraries(${PROJECT_NAME} PRIVATE borealis::borealis)

# Add RomFS if the directory exists
if(EXISTS "${CMAKE_SOURCE_DIR}/romfs")
    nx_add_romfs(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/romfs")
endif()

# Generate the .nro
nx_generate_nro(${PROJECT_NAME}
    NAME ${APP_TITLE}
    AUTHOR ${APP_AUTHOR}
    VERSION ${APP_VERSION}
)
