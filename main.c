#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
	};

	if(!IMG_Init(IMG_INIT_PNG)) {
		printf("IMG_Init failed: %s\n", SDL_GetError());
	};

	RenderWindow window;
	RenderWindow_Init(&window, "Cheeki Breeki!", 800, 600);
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(&window, "res/textures/background.png");

	bool gameRunning = true;

	SDL_Event event;

	while (gameRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				gameRunning = false;
		}
		RenderWindow_clear(&window);
		RenderWindow_render(&window, backgroundTexture);
		RenderWindow_display(&window);

	}
	RenderWindow_Destroy(&window);

	SDL_Quit();
	return 0;
}
