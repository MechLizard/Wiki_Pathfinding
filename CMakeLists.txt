set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

cmake_minimum_required(VERSION 3.10)
project(Wiki_Pathfinding)

set(CMAKE_CXX_STANDARD 23)

include_directories(backend/node_modules/node-addon-api)
include_directories(lib)
include_directories(src)
include_directories(backend/src)
include_directories(${CMAKE_JS_INC})
add_library(addon SHARED backend/src/addon.cpp)
set_target_properties(addon PROPERTIES PREFIX "" SUFFIX ".node")


add_library(WikipediaImporter
        src/main.cpp
        src/Article.cpp
        src/Article.h
        src/WikiImporter.cpp
        src/WikiImporter.h src/main.cpp src/bfs.h src/dfs.h)

target_link_libraries(addon ${CMAKE_JS_LIB})
