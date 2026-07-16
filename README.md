# Bedrocked

A Minecraft-inspired voxel engine built from scratch in modern C++.

[![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?logo=cmake)](https://cmake.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-4.1%2B-green.svg)](https://www.opengl.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## About

**Bedrocked** is a voxel engine written entirely from scratch in modern C++.  
It aims to recreate the core feel of Minecraft-style worlds while being a clean, educational, and extensible codebase.

The project is currently in active development.

## Current Features

- Chunk-based world rendering
- Texture atlas support
- Basic multi-chunk world
- Modern OpenGL rendering pipeline
- Clean separation between engine and application code

## Planned / In Progress

- Better world generation
- Player movement & camera
- Block breaking/placing
- Lighting system
- Performance optimizations (frustum culling, etc.)

## Getting Started

### Prerequisites

- CMake 3.20 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- OpenGL 4.1+ capable GPU + drivers

### Build Instructions

```bash
git clone https://github.com/steeve-pix/Bedrocked.git
cd Bedrocked

mkdir build && cd build
cmake ..
cmake --build . --config Release