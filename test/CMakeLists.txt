# BUILD_TESTING variable is created by include(CTest)
# It is set to ON by default
if (BUILD_TESTING)
    add_executable (project_test test_source.cpp)
    target_link_libraries (project_test PRIVATE 
        network
        GTest::gtest_main)
    
    target_include_directories (project_test PRIVATE 
        ${PROJECT_SOURCE_DIR}/network/include)
    
    include (GoogleTest)
    # Finds all the Google tests associated with the executable
    gtest_discover_tests (project_test)
endif()
