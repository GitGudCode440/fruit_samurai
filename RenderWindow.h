#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include"Utils.h"


typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} RenderWindow;

typedef struct {
	SDL_Texture* texture;
	int width, height;
} RenderImage;

RenderWindow* RenderWindow_Init(const char* title, int width, int height);
void RenderWindow_Destroy(const RenderWindow* renderWindow);

RenderImage* RenderWindow_loadTexture(const RenderWindow* renderWindow, const char* filepath);
RenderImage* RenderWindow_loadTextureFromFont(const RenderWindow* renderWindow, TTF_Font* font, const char* title, const SDL_Color color);
void RenderWindow_clear(const RenderWindow* renderWindow);
void RenderWindow_render(const RenderWindow* renderWindow, SDL_Texture* texture);
void RenderWindow_display(const RenderWindow* renderWindow);