cmake_minimum_required (VERSION 3.16)
project (network VERSION 0.0.1
                   DESCRIPTION "Data structures and network algorithms"
                   LANGUAGES CXX)
    
add_library (network STATIC
    ${PROJECT_SOURCE_DIR}/src/network.cpp)
    
target_include_directories (network PUBLIC
    ${PROJECT_SOURCE_DIR}/include)
