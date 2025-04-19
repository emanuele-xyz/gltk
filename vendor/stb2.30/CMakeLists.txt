cmake_minimum_required(VERSION 3.30)

project(stb)

add_library(stb)
target_sources(stb PRIVATE src/stb_image_implementation.cpp)
target_include_directories(stb PRIVATE include)
