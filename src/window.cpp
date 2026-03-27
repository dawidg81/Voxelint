#include <SDL2/SDL.h>
#include <stdio.h>
#include "window.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height) : window(NULL), renderer(NULL){
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	if(window == NULL)
		printf("SDL could not create window! SDL Error: %s\n", SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL)
		printf("SDL could not create renderer! SDL Error: %s\n", SDL_GetError());
}

SDL_Renderer* RenderWindow::GetRenderer(){
	return renderer;
}

void RenderWindow::close(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	SDL_Quit();
}
