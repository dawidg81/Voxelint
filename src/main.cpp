#include <SDL2/SDL.h>
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
    SDL_Renderer* sdlRenderer = window.GetRenderer();

    if(!sdlRenderer)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

    // Lock mouse to window
    SDL_SetRelativeMouseMode(SDL_TRUE);

    World world;
    world.Generate();

    printf("block at 64,32,64 = %d\n", world.Get(64, 32, 64)); // should print 1
    printf("block at 64,33,64 = %d\n", world.Get(64, 33, 64)); // should print 0 (air)

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

        SDL_SetRenderDrawColor(sdlRenderer, 135, 206, 235, 255); // sky blue
        SDL_RenderClear(sdlRenderer);
        renderer.Clear();

        renderer.DrawWorld(sdlRenderer, world, cam);

        // FPS
        frameCount++;
        if(SDL_GetTicks() - fpsTimer >= 1000){
            fps = frameCount; frameCount = 0;
            fpsTimer = SDL_GetTicks();
        }
        snprintf(fpsText, sizeof(fpsText), "%d fps", fps);
        DrawText(sdlRenderer, "0.1.0", 0, 0, 1, 255, 255, 255);
        DrawText(sdlRenderer, fpsText, 0, 8, 1, 255, 255, 255);

	char camText[64];
	snprintf(camText, sizeof(camText), "%.1f %.1f %.1f %.2f %.2f",
	    cam.pos.x, cam.pos.y, cam.pos.z, cam.yaw, cam.pitch);
	DrawText(sdlRenderer, camText, 0, 16, 1, 255, 255, 255);

        SDL_RenderPresent(sdlRenderer);
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);
    window.close();
    return 0;
}
