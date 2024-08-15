cmake_minimum_required(VERSION 3.21)

add_executable(GTests
    inverted_index_test.cpp
    search_server_test.cpp
    ${MY_INCLUDE_DIR}/inverted_index.h
    ${MY_INCLUDE_DIR}/converter.h
    ${MY_SOURCE_DIR}/search_server.cpp
    ${MY_SOURCE_DIR}/inverted_index.cpp
)

target_link_libraries(GTests GTest::gtest_main)
target_include_directories(GTests PUBLIC ${MY_INCLUDE_DIR})

include(GoogleTest)

gtest_discover_tests(GTests)
