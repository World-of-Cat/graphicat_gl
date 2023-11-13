cmake_minimum_required(VERSION 3.26)

add_executable(example src/main.cpp)
target_link_libraries(example PRIVATE graphicat::graphicat)