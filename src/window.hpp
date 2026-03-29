#pragma once

#include <SDL2/SDL.h>

class RenderWindow {
public:
	RenderWindow(const char* title, int width, int height);
	SDL_Renderer* GetRenderer();
	void close();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
