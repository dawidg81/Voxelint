#include <SDL2/SDL.h>
#include <stdio.h>
#include "window.hpp"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());

	RenderWindow window("Voxelint", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer* renderer = window.GetRenderer();

	if(renderer == NULL)
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

	bool gameRunning = true;

	SDL_Event event;

	while(gameRunning){
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT){
				gameRunning = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

		SDL_Delay(60);
	}
	window.close();
	return 0;
}
