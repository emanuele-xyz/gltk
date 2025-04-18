cmake_minimum_required(VERSION 3.30)

project(glad3.3)

add_library(glad3.3)
target_sources(glad3.3 PRIVATE src/glad.c)
target_include_directories(glad3.3 PRIVATE include)
