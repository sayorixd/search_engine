cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(include/json-develop)
add_subdirectory(src)

