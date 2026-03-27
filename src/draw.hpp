#pragma once
#include <SDL2/SDL.h>

void DrawChar(SDL_Renderer* renderer, char c, int x, int y, int scale, Uint8 r, Uint8 g, Uint8 b);
void DrawText(SDL_Renderer* renderer, const char* text, int x, int y, int scale, Uint8 r, Uint8 g, Uint8 b);
void DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void DrawRect(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);
void DrawRectFilled(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);
void DrawCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b);
void DrawCircleFilled(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b);
void DrawPixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b);
