#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "math.hpp"
#include "world.hpp"
#include "camera.hpp"

struct Vertex {
    Vec3 pos;
    Uint8 r, g, b;
};

struct Triangle {
    Vertex v[3];
};

struct Renderer {
    int screenW, screenH;
    std::vector<float> depthBuffer;

    void Init(int w, int h);
    void Clear();
    void DrawWorld(SDL_Renderer* sdl, const World& world, const Camera& cam);

private:
    Mat4 proj;
    void DrawTriangle(SDL_Renderer* sdl, Vec3 a, Vec3 b, Vec3 c,
                      Uint8 r, Uint8 g, Uint8 b_, const Mat4& vp);
    bool ProjectVertex(Vec4 clip, Vec3& ndc) const;
    void RasterTriangle(SDL_Renderer* sdl, Vec2 p0, Vec2 p1, Vec2 p2,
                        float z0, float z1, float z2, Uint8 r, Uint8 g, Uint8 b);
};
