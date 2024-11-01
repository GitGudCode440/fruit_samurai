#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"
#include "Entity.h"

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
	};

	if(!IMG_Init(IMG_INIT_PNG)) {
		printf("IMG_Init failed: %s\n", SDL_GetError());
	};

	RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", 800, 600);
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");



	Entity* entities[4];

	{
		SDL_Texture* textures[4] = {
			RenderWindow_loadTexture(window, "res/textures/watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/bananas.png"),
			RenderWindow_loadTexture(window, "res/textures/cherry.png"),
			RenderWindow_loadTexture(window, "res/textures/strawberry.png")
		};


		for(int i = 0; i < 4; i++) {
			entities[i] = Entity_Init(200 * (float) i + 64, 40, 40, 450, textures[i]);
		}
	}


	bool gameRunning = true;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	SDL_Event event;

	while (gameRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				gameRunning = false;

		}

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double) (NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency() ;

		RenderWindow_clear(window);
		RenderWindow_render(window, backgroundTexture);
		for(int i = 0; i < 4; i++) {
			Entity_applyGravity(entities[i], 0.1f, deltaTime);
			Entity_render(window, entities[i]);

		}
		RenderWindow_display(window);

	}

	RenderWindow_Destroy(window);

	SDL_Quit();
	return 0;
}
