cmake_minimum_required(VERSION 3.26)
project(graphicat_gl)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glad)


add_library(graphicat src/graphicat/graphicat.cpp
        src/graphicat/graphicat.hpp)


target_link_libraries(graphicat PUBLIC glfw glad::glad)


add_subdirectory(example)
