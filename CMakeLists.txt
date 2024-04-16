cmake_minimum_required(VERSION 3.24.2)
project(Wiki_Pathfinding)

set(CMAKE_CXX_STANDARD 23)

include_directories(lib)

add_executable(WikipediaImporter
        src/main.cpp
        src/Article.cpp
        src/Article.h
        src/WikiImporter.cpp
        src/WikiImporter.h src/main.cpp src/bfs.h src/dfs.h)