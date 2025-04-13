cmake_minimum_required(VERSION 3.10)
project(Synthara VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(Curses REQUIRED)

# Include directories
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add external dependencies
include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(synthara ${SOURCES})

# Link libraries
target_link_libraries(synthara PRIVATE
    ${CURSES_LIBRARIES}
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# Install
install(TARGETS synthara DESTINATION bin)
