#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdbool.h>
#include <time.h>

#include "RenderWindow.h"
#include "Entity.h"

float randomFloat() {
	return (float)rand() / (float)RAND_MAX;
}

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



	Entity* entities = malloc(sizeof(Entity) * 4);

	{
		SDL_Texture* textures[4] = {
			RenderWindow_loadTexture(window, "res/textures/watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/bananas.png"),
			RenderWindow_loadTexture(window, "res/textures/cherry.png"),
			RenderWindow_loadTexture(window, "res/textures/strawberry.png")
		};

		srand(time(NULL));
		for(int i = 0; i < 4; i++) {
			entities[i] = (Entity) {
				.x = 200 * (float) i + 64,
				.y = 800,
				.size = 60,
				.viewRect = {0, 0, 450, 450},
				.texture = textures[i],
				.initialVelocity = {randomFloat() / 10.0f, -1}
			};
		}
	}

	// SDL_Texture* cherryTexture = RenderWindow_loadTexture(window, "res/textures/cherry.png");
	// const Entity cherry = (Entity) {
	// 	.x = 200,
	// 	.y = 400,
	// 	.size = 40,
	// 	.viewRect = {0, 0, 450, 450},
	// 	.texture = cherryTexture
	// };
	//
	// Entity_append(&entities, cherry, &entityAmount);
	bool gameRunning = true;

	/*	https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl
	*	Solution for calculating deltaTime which is the top answer.
	*/
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	SDL_Event event;

	while (gameRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				gameRunning = false;

		}
		uint32_t timeElapsed = SDL_GetTicks();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();


		deltaTime = (double) ((NOW - LAST) * 1000) / (double) SDL_GetPerformanceFrequency() ;

		RenderWindow_clear(window);
		RenderWindow_render(window, backgroundTexture);
		for(int i = 0; i < 4; i++) {
			Entity_applyVelocity(&entities[i], &deltaTime);
			Entity_applyGravity(&entities[i], 0.00035f, &timeElapsed, &deltaTime);
			Entity_render(window, &entities[i]);


		}
		RenderWindow_display(window);

	}

	free(entities);
	RenderWindow_Destroy(window);

	SDL_Quit();
	return 0;
}
