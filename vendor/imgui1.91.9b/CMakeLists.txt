cmake_minimum_required(VERSION 3.30)

project(imgui)

add_library(imgui)

target_sources(
    imgui
    PRIVATE
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_widgets.cpp
    src/imgui_draw.cpp
    src/imgui_tables.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_impl_glfw.cpp
)

target_include_directories(
    imgui
    PRIVATE
    include
    ../glfw3.4/include
)
