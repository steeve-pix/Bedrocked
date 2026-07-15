#pragma once
#include "bedrocked/world/chunk/Chunk.hpp"
#include "bedrocked/world/chunk/ChunkMeshData.hpp"

namespace bedrocked {
    ChunkMeshData buildChunkMeshData(const Chunk &chunk);
}
