cmake_minimum_required(VERSION 3.20)

project(Bedrocked LANGUAGES C CXX)

# Generates compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# GLFW libs
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

#
add_library(glad STATIC
        third_party/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        ${PROJECT_SOURCE_DIR}/third_party/glad/include
)

add_library(stb INTERFACE)

target_include_directories(stb
        INTERFACE
        "${PROJECT_SOURCE_DIR}/third_party/stb"
)

add_subdirectory(engine)
add_subdirectory(app)