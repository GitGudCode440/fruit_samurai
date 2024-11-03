#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>


typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} RenderWindow;

RenderWindow* RenderWindow_Init(const char* title, int width, int height);
void RenderWindow_Destroy(const RenderWindow* renderWindow);

SDL_Texture* RenderWindow_loadTexture(const RenderWindow* renderWindow, const char* filepath);
void RenderWindow_clear(const RenderWindow* renderWindow);
void RenderWindow_render(const RenderWindow* renderWindow, SDL_Texture* texture);
void RenderWindow_display();