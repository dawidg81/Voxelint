#pragma once
#include <SDL2/SDL.h>

class RenderWindow {
public:
    RenderWindow(const char* title, int width, int height);
    SDL_Window* GetWindow();
    SDL_GLContext GetGLContext();
    void close();

private:
    SDL_Window*   window;
    SDL_GLContext glContext;
};
