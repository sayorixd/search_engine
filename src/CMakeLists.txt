cmake_minimum_required(VERSION 3.21)

add_executable(search_engine ${CMAKE_SOURCE_DIR}/main.cpp ${MY_INCLUDE_DIR}/converter.h ${MY_INCLUDE_DIR}/inverted_index.h ${MY_INCLUDE_DIR}/search_server.h converter.cpp inverted_index.cpp search_server.cpp)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})
