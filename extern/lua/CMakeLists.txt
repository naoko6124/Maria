cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(lua)
file (GLOB_RECURSE lua_src src/**.*)
add_library(lua STATIC ${lua_src})
target_include_directories(lua PUBLIC src)