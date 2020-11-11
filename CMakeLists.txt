cmake_minimum_required(VERSION 3.17)
project(lua_plugin)

set(CMAKE_CXX_STANDARD 14)
include_directories(luatest ${CMAKE_SOURCE_DIR}/lib/include)

add_library(lualib SHARED IMPORTED)
set_property(TARGET lualib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/liblua54.so")

add_executable(lua_plugin repl.cpp lua_plugin.cpp lua_plugin_colors.cpp lua_plugin_io.cpp liolib.c)
target_link_libraries(lua_plugin lualib "-ldl -lpthread")
