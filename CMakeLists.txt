cmake_minimum_required(VERSION 3.20)

project(Bedrocked LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_library(glad STATIC
        third_party/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        third_party/glad/include
)

add_executable(bedrocked
        src/main.cpp
        src/bedrocked/core/Application.cpp
        src/bedrocked/core/Logger.cpp
        src/bedrocked/core/Timer.cpp
        src/bedrocked/platform/Window.cpp
        src/bedrocked/rendering/Renderer.cpp
        src/bedrocked/rendering/ShaderProgram.cpp
        src/bedrocked/rendering/VertexBuffer.cpp
        src/bedrocked/rendering/VertexArray.cpp
        src/bedrocked/rendering/Mesh.cpp
        src/bedrocked/rendering/IndexBuffer.cpp
)

target_compile_features(bedrocked PRIVATE cxx_std_20)

target_include_directories(bedrocked PRIVATE src)

target_link_libraries(bedrocked PRIVATE glfw glad OpenGL::GL)