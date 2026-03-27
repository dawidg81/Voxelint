#pragma once
#include <stdint.h>

#define WORLD_W 128
#define WORLD_H  64
#define WORLD_D 128

enum BlockType : uint8_t {
    BLOCK_AIR  = 0,
    BLOCK_GRASS = 1,
    BLOCK_DIRT  = 2,
};

struct World {
    uint8_t blocks[WORLD_W][WORLD_H][WORLD_D];

    void Generate();
    uint8_t Get(int x, int y, int z) const;
    bool    InBounds(int x, int y, int z) const;
};
