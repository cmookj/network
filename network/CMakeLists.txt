cmake_minimum_required (VERSION 3.16)
project (network VERSION 0.0.1
                   DESCRIPTION "A test project"
                   LANGUAGES CXX)
    
add_library (network STATIC
    ${PROJECT_SOURCE_DIR}/src/source.cpp)
    
target_include_directories (network PUBLIC
    ${PROJECT_SOURCE_DIR}/include)
