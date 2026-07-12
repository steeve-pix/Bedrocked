cmake_minimum_required(VERSION 3.20)
project(Bedrocked LANGUAGES CXX)

add_executable(bedrocked
        src/main.cpp
        src/bedrocked/core/Application.cpp)

target_compile_features(bedrocked  PRIVATE cxx_std_20)
target_include_directories(bedrocked PRIVATE src)