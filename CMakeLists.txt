cmake_minimum_required(VERSION 3.20)
project(Bedrocked LANGUAGES CXX)

add_executable(bedrocked "src/main.cpp")

target_compile_features(bedrocked  PRIVATE cxx_std_20)
