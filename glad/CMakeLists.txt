cmake_minimum_required(VERSION 3.26)

add_library(glad src/gl.c)
target_include_directories(glad PUBLIC include/)

add_library(glad::glad ALIAS glad)