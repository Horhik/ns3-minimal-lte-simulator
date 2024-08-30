cmake_minimum_required(VERSION 3.0)
project(my_ns3_project)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

# Specify the path to the ns-3 installation
set(NS3_DIR "~/ns3-install")

# Set up the include directories
include_directories(${NS3_DIR}/include)

# Link against ns-3 libraries
link_directories(${NS3_DIR}/lib)

# Add your source files here
set(SOURCE_FILES minimal-lte.cc)

# Create an executable
add_executable(my_ns3_app ${SOURCE_FILES})

# Link the ns-3 libraries
include_directories(${NS3_DIR}/include/ns3)
target_link_libraries(my_ns3_app
    ns3-core
    ns3-network
    ns3-mobility
    ns3-lte
    ns3-internet
    ns3-point-to-point
    ns3-ipv4-global-routing
    ns3-udp-client-server
    ns3-pf-ff-mac
)
