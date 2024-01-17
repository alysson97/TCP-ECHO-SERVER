cmake_minimum_required(VERSION 3.15)
project(your_project_name)

set(CMAKE_CXX_STANDARD 14)  # Set the C++ standard, adjust as needed

# Find the Boost library
find_package(Boost REQUIRED COMPONENTS system)

# Add include directories for Boost and common headers
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add your server-side executable
add_executable(server server.cpp)

# Link the server executable with Boost libraries
target_link_libraries(server ${Boost_LIBRARIES})

# Add your client-side executable
add_executable(client client.cpp)

# Link the client executable with Boost libraries
target_link_libraries(client ${Boost_LIBRARIES})
