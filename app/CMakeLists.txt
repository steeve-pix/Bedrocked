add_executable(bedrocked
        main.cpp
)

target_compile_features(bedrocked
        PRIVATE
        cxx_std_20
)

target_link_libraries(bedrocked
        PRIVATE
        Bedrocked::Engine
)

# Copy runtime assets beside the executable after building.
add_custom_command(
        TARGET bedrocked
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:bedrocked>/assets"

        COMMENT "Copying Bedrocked assets"
)