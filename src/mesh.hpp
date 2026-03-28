#pragma once
#include <vector>
#include <stdint.h>
#include "math.hpp"

struct Vertex {
    Vec3 pos;
    uint8_t r, g, b;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    void Clear() {
        vertices.clear();
        indices.clear();
    }
};
