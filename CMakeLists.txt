cmake_minimum_required (VERSION 3.16)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (network VERSION 0.0.1
                    DESCRIPTION "My new project with unit test"
                    LANGUAGES CXX)

# Update the submodules automatically
include (cmake/UpdateSubmodules.cmake)

# Enable testing for this project
include (CTest)

# Add subdirectories with code
add_subdirectory (external)
add_subdirectory (network)
add_subdirectory (test)
