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

# -----------------------------------------------------------------------------
# project
# -----------------------------------------------------------------------------
add_executable("${CMAKE_PROJECT_NAME}")

# project configuration properties
set_target_properties(
    "${CMAKE_PROJECT_NAME}"
    PROPERTIES
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

# project source files
target_sources(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Hello.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
)

# project include directories
target_include_directories(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad3.3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw3.4/include"
)

# project libraries
target_link_libraries(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    glad3.3
    glfw
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
