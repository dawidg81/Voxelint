#include "world.hpp"
#include <string.h>

void World::Generate(){
    memset(blocks, BLOCK_AIR, sizeof(blocks));
    // flat world: one layer of grass at y=32, dirt below
    for(int x = 0; x < WORLD_W; x++){
        for(int z = 0; z < WORLD_D; z++){
            blocks[x][32][z] = BLOCK_GRASS;
            for(int y = 0; y < 32; y++)
                blocks[x][y][z] = BLOCK_DIRT;
        }
    }
}

uint8_t World::Get(int x, int y, int z) const {
    if(!InBounds(x,y,z)) return BLOCK_AIR;
    return blocks[x][y][z];
}

bool World::InBounds(int x, int y, int z) const {
    return x>=0 && x<WORLD_W && y>=0 && y<WORLD_H && z>=0 && z<WORLD_D;
}
