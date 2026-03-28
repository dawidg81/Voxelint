#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <stdio.h>
#include "window.hpp"
#include "draw.hpp"
#include "math.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "renderer.hpp"

int SCREEN_WIDTH  = 640;
int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());

    RenderWindow window("Voxelint", SCREEN_WIDTH, SCREEN_HEIGHT);

    // SDL renderer for HUD only (no 3D)
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window.GetWindow(), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    World world;
    world.Generate();

    Camera cam;
    cam.Init({64, 40, 64});

    Renderer renderer;
    renderer.Init(SCREEN_WIDTH, SCREEN_HEIGHT);

    bool gameRunning = true;
    SDL_Event event;

    Uint32 lastTick = SDL_GetTicks();
    Uint32 fpsTimer = SDL_GetTicks();
    int frameCount = 0, fps = 0;
    char fpsText[32];
    char camText[64];

    while(gameRunning){
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTick) / 1000.f;
        lastTick = now;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) gameRunning = false;
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                gameRunning = false;
            if(event.type == SDL_MOUSEMOTION)
                cam.HandleMouse(event.motion.xrel, event.motion.yrel);
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        cam.HandleInput(keys, dt);

        // --- 3D world (OpenGL) ---
        renderer.BeginFrame();
        renderer.DrawWorld(world, cam);

        // --- 2D HUD (SDL drawn into GL framebuffer via glDrawPixels workaround)
        // Simplest approach: use glRasterPos + SDL surface, OR just use GL lines/quads for text
        // Since draw.hpp uses SDL_Renderer, we render HUD to a texture then blit
        // For now: draw directly with OpenGL immediate mode text workaround
        // by re-enabling 2D mode and using our font manually via GL_POINTS

        // FPS counter
        frameCount++;
        if(SDL_GetTicks() - fpsTimer >= 1000){
            fps = frameCount;
            frameCount = 0;
            fpsTimer = SDL_GetTicks();
        }

        // Switch to ortho 2D, draw HUD text via OpenGL points (reuse font data)
        renderer.BeginHUD(window.GetWindow());

        snprintf(fpsText, sizeof(fpsText), "%d fps", fps);
        snprintf(camText, sizeof(camText), "%.1f %.1f %.1f  yaw %.2f pitch %.2f",
            cam.pos.x, cam.pos.y, cam.pos.z, cam.yaw, cam.pitch);

        DrawText(sdlRenderer, "0.1.0", 0, 0,  1, 255, 255, 255);
        DrawText(sdlRenderer, fpsText,  0, 8,  1, 255, 255, 255);
        DrawText(sdlRenderer, camText,  0, 16, 1, 255, 255, 255);

        renderer.EndHUD(window.GetWindow());

        SDL_GL_SwapWindow(window.GetWindow());
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_DestroyRenderer(sdlRenderer);
    window.close();
    return 0;
}
