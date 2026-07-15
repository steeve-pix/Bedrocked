add_library(bedrocked_engine STATIC
        src/assets/Image.cpp

        src/core/Application.cpp
        src/core/Logger.cpp
        src/core/Timer.cpp

        src/platform/Window.cpp

        src/rendering/Camera.cpp
        src/rendering/Mesh.cpp
        src/rendering/Renderer.cpp

        src/rendering/opengl/IndexBuffer.cpp
        src/rendering/opengl/ShaderProgram.cpp
        src/rendering/opengl/Texture2D.cpp
        src/rendering/opengl/VertexArray.cpp
        src/rendering/opengl/VertexBuffer.cpp

        src/world/chunk/Chunk.cpp
        src/world/chunk/ChunkMesher.cpp
        src/world/World.cpp

        src/world/generation/ValueNoise.cpp
        src/world/generation/FractalNoise.cpp
)

#
add_library(Bedrocked::Engine ALIAS bedrocked_engine)

target_compile_features(bedrocked_engine
        PUBLIC
            cxx_std_20
)

target_include_directories(bedrocked_engine
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(bedrocked_engine
        PRIVATE
        glfw
        glad
        OpenGL::GL
        stb
)