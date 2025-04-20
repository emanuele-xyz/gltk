cmake_minimum_required(VERSION 3.30)

project(gltk_demo
    VERSION 1.0
    LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# compile static libraries
# -----------------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

# glad
add_subdirectory(vendor/glad3.3) # for opengl3.3 core

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw3.4)

# imgui
add_subdirectory(vendor/imgui1.91.9b)

# stb
add_subdirectory(vendor/stb2.30)

# tinygltf
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_BUILD_GL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TINYGLTF_BUILD_VALIDATOR_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_BUILD_BUILDER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_HEADER_ONLY OFF CACHE BOOL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE BOOL "" FORCE)
set(TINYGLTF_INSTALL_VENDOR OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/tinygltf2.9.5)

# -----------------------------------------------------------------------------
# project
# -----------------------------------------------------------------------------
add_executable("${CMAKE_PROJECT_NAME}")

# project configuration properties
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 23)
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CMAKE_CXX_EXTENSIONS OFF)

# project source files
target_sources(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gltk/Crash.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
)

# project include directories
target_include_directories(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad3.3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw3.4/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm1.0.1/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui1.91.9b/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb2.30/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinygltf2.9.5/include"
)

# project libraries
target_link_libraries(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    glad3.3
    glfw
    imgui
    stb
    tinygltf
)

# warnings
target_compile_options(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# -----------------------------------------------------------------------------
# build commands (from project's root)
# -----------------------------------------------------------------------------
#cmake -S . -B build
#cmake --build build --verbose --parallel --config Debug
#cmake --build build --verbose --parallel --config RelWithDebInfo
#cmake --build build --verbose --parallel --config Release
