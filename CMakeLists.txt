cmake_minimum_required(VERSION 3.20)
project(Bedrocked LANGUAGES C CXX)

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
        src/bedrocked/platform/Window.cpp
        src/bedrocked/core/Timer.cpp
        src/bedrocked/core/Logger.cpp
)

target_include_directories(bedrocked PRIVATE src)
target_link_libraries(bedrocked PRIVATE glfw glad OpenGL::GL)

find_package(OpenGL REQUIRED)

target_compile_features(bedrocked PRIVATE cxx_std_20)

add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
