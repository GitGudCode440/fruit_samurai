#include"RenderWindow.h"

#include <SDL2/SDL_image.h>

void RenderWindow_Init(RenderWindow* renderWindow, const char* title, const int width, const int height) {
	renderWindow->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (!renderWindow->window) {
		printf("Failed to create window: %s\n", SDL_GetError());
	}
	renderWindow->renderer = SDL_CreateRenderer(renderWindow->window, -1, SDL_RENDERER_ACCELERATED);
}

void RenderWindow_Destroy(const RenderWindow* renderWindow) {
	SDL_DestroyWindow(renderWindow->window);
}

SDL_Texture* RenderWindow_loadTexture(const RenderWindow* renderWindow, const char* filepath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderWindow->renderer, filepath);

	if(texture == NULL) {
		printf("Failed to get texture: %s\n", SDL_GetError());
	}

	return texture;
}
void RenderWindow_clear(const RenderWindow* renderWindow) {
	SDL_RenderClear(renderWindow->renderer);
}

void RenderWindow_render(const RenderWindow* renderWindow, SDL_Texture* texture) {
	SDL_RenderCopy(renderWindow->renderer, texture, NULL, NULL);
}

void RenderWindow_display(const RenderWindow* renderWindow) {
	SDL_RenderPresent(renderWindow->renderer);
}
