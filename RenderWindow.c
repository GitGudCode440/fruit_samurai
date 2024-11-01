#include"RenderWindow.h"
#include"Entity.h"


RenderWindow* RenderWindow_Init(const char* title, const int width, const int height) {
	RenderWindow* renderWindow = malloc(sizeof(RenderWindow));

	if (!renderWindow) {
		return NULL;
	}

	renderWindow->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (!renderWindow->window) {
		printf("Failed to create window: %s\n", SDL_GetError());
		free(renderWindow);
		return NULL;
	}
	renderWindow->renderer = SDL_CreateRenderer(renderWindow->window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderWindow->renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(renderWindow->window);
		free(renderWindow);
		return NULL;
	}

	return renderWindow;
}

void RenderWindow_Destroy(const RenderWindow* renderWindow) {
	SDL_DestroyWindow(renderWindow->window);
	free((void*) renderWindow);
}

SDL_Texture* RenderWindow_loadTexture(const RenderWindow* renderWindow, const char* filepath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderWindow->renderer, filepath);

	if(texture == NULL) {
		printf("Failed to get texture: %s\n", SDL_GetError());
		return NULL;
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
