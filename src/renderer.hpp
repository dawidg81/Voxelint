#pragma once
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "math.hpp"
#include "camera.hpp"
#include "world.hpp"

struct Renderer {
    int screenW, screenH;

    void Init(int w, int h);
    void BeginFrame();
    void DrawWorld(const World& world, const Camera& cam);

    // HUD: temporarily switch to SDL, draw, then back
    void BeginHUD(SDL_Window* win);
    void EndHUD(SDL_Window* win);
};
