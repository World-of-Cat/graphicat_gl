cmake_minimum_required(VERSION 3.26)
project(graphicat_gl)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(glad)

add_library(graphicat src/graphicat/graphicat.cpp
        src/graphicat/graphicat.hpp
        src/graphicat/os/window.cpp
        src/graphicat/os/window.hpp
        src/graphicat/graphics/utils.cpp
        src/graphicat/graphics/utils.hpp
        src/graphicat/graphics/buffer.cpp
        src/graphicat/graphics/buffer.hpp
        src/graphicat/graphics/vertex_array.cpp
        src/graphicat/graphics/vertex_array.hpp
        src/graphicat/graphics/shader.cpp
        src/graphicat/graphics/shader.hpp
)

target_include_directories(graphicat PUBLIC src/)

target_link_libraries(graphicat PUBLIC glfw glad::glad spdlog::spdlog)
target_compile_definitions(graphicat PUBLIC -DGLFW_INCLUDE_NONE)

add_library(graphicat::graphicat ALIAS graphicat)

add_subdirectory(example)
