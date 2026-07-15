#pragma once
namespace bedrocked {
    class Chunk;

    struct ChunkNeighbors {
        const Chunk *front{};
        const Chunk *back{};
        const Chunk *left{};
        const Chunk *right{};
        const Chunk *top{};
        const Chunk *bottom{};
    };
} // namespace bedrocked
