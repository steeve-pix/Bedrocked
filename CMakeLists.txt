cmake_minimum_required(VERSION 3.20)
project(Bedrocked LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

add_executable(bedrocked
        src/main.cpp
        src/bedrocked/core/Application.cpp
        src/bedrocked/platform/Window.cpp)

target_compile_features(bedrocked PRIVATE cxx_std_20)
target_include_directories(bedrocked PRIVATE src)
target_link_libraries(bedrocked PRIVATE glfw)