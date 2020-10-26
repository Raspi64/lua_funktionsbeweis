cmake_minimum_required(VERSION 3.17)
project(luatest)

set(CMAKE_CXX_STANDARD 14)
include_directories(luatest ${CMAKE_SOURCE_DIR}/lib/include)

add_library(lualib SHARED IMPORTED)
set_property(TARGET lualib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/liblua54.so")

add_executable(luatest luaexample.cpp)
target_link_libraries(luatest lualib "-ldl")
