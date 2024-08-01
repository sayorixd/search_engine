cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json-develop)

add_executable(search_engine main.cpp converter.cpp inverted_index.cpp)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

