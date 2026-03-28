#include "window.hpp"
#include <stdio.h>

RenderWindow::RenderWindow(const char* title, int width, int height)
    : window(NULL), glContext(NULL)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(!window)
        printf("Window error: %s\n", SDL_GetError());

    glContext = SDL_GL_CreateContext(window);
    if(!glContext)
        printf("GL context error: %s\n", SDL_GetError());

    SDL_GL_SetSwapInterval(0); // 0 = uncapped, 1 = vsync
}

SDL_Window* RenderWindow::GetWindow() { return window; }
SDL_GLContext RenderWindow::GetGLContext() { return glContext; }

void RenderWindow::close(){
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
