#include"RenderWindow.h"


RenderWindow* RenderWindow_Init(const char* title, const int width, const int height) {
	RenderWindow* renderWindow = malloc(sizeof(RenderWindow));

	if (!renderWindow) {
		return NULL;
	}

	renderWindow->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (!renderWindow->window) {
		printf("Failed to create window: %s\n", SDL_GetError());
		return NULL;
	}
	renderWindow->renderer = SDL_CreateRenderer(renderWindow->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderWindow->renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(renderWindow->window);
		return NULL;
	}

	return renderWindow;
}

void RenderWindow_Destroy(const RenderWindow* renderWindow) {
	SDL_DestroyWindow(renderWindow->window);
	free((void*) renderWindow);
}

RenderImage* RenderWindow_loadTexture(const RenderWindow* renderWindow, const char* filepath) {
	SDL_Surface* textureSurface = IMG_Load(filepath);

	if (textureSurface == NULL) {
		printf("Failed to load text surface: %s\n", TTF_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderWindow->renderer, textureSurface);


	if(texture == NULL) {
		printf("Failed to get texture: %s\n", SDL_GetError());
		return NULL;
	}

	RenderImage* textureImage = malloc(sizeof(RenderImage));

	if (!textureImage) {
		printf("Failed to allocate for RenderImage\n");
		return NULL;
	}

	textureImage->texture = texture;
	textureImage->width = textureSurface->w;
	textureImage->height = textureSurface->h;

	free(textureSurface);
	return textureImage;
}

RenderImage* RenderWindow_loadTextureFromFont(const RenderWindow* renderWindow, TTF_Font* font, const char* title, const SDL_Color color) {
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, title, color);

	if (textSurface == NULL) {
		printf("Failed to load text surface: %s\n", TTF_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderWindow->renderer, textSurface);

	if (texture == NULL) {
		printf("Failed to get texture: %s\n", SDL_GetError());
		return NULL;
	}

	RenderImage* fontImage = malloc(sizeof(RenderImage));

	if (!fontImage) {
		printf("Failed to allocate for RenderImage\n");
		return NULL;
	}

	fontImage->texture = texture;
	fontImage->width = textSurface->w;
	fontImage->height = textSurface->h;


	free(textSurface);
	return fontImage;
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
