# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MinimalLteProject VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use environment variables for NS-3 path and version
# Default to empty if not set
set(NS3_PATH $ENV{NS3_PATH})
set(NS3_VERSION $ENV{NS3_VERSION})

# Check if NS3_PATH and NS3_VERSION are set
if(NOT NS3_PATH)
    message(FATAL_ERROR "NS3_PATH environment variable is not set.")
endif()

if(NOT NS3_VERSION)
    message(FATAL_ERROR "NS3_VERSION environment variable is not set.")
endif()

# Include the directory with NS-3 headers
include_directories(${NS3_PATH}/build/include)

# Create a directory for the executable
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable
add_executable(minimal-lte minimal-lte.cc)

# Link the NS-3 shared libraries
target_link_libraries(minimal-lte
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-core-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-network-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-internet-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-applications-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-point-to-point-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-mobility-default.so
    ${NS3_PATH}/build/lib/libns3.${NS3_VERSION}-lte-default.so
)

# Add rpath to find libraries at runtime
set_target_properties(minimal-lte PROPERTIES
    INSTALL_RPATH ${NS3_PATH}/build/lib
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Set the output directory for the executable
set_target_properties(minimal-lte PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
