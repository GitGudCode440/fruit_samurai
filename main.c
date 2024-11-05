#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
	//Initialize SDL, and SDL_image. SDL_image is for loading images.

	//If Initializing SDL returns greater than 0, there is an error.
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	};

	//If Initializing SDL_image does not exist, there is an error.
	if(!IMG_Init(IMG_INIT_PNG)) {
		printf("IMG_Init failed: %s\n", SDL_GetError());
		return -1;
	};


	//RenderWindow is a combination of a renderer, and a window.
	//RenderWindow.h also comes with functions that are helpful to display things on the screen.
	RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", 800, 600);

	//Wood background.
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");

	//Entities like watermelon, bananas, cherry, strawberry, etc.
	Entity* entities = malloc(sizeof(Entity) * 4);

	//Things initialized inside braces, only exist inside braces.
	{
		//Load texture for every fruit.
		SDL_Texture* textures[4] = {
			RenderWindow_loadTexture(window, "res/textures/watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/bananas.png"),
			RenderWindow_loadTexture(window, "res/textures/cherry.png"),
			RenderWindow_loadTexture(window, "res/textures/strawberry.png")
		};

		//Initialize entity with required params, like position, size, etc.

		srand(time(NULL));
		for(int i = 0; i < 4; i++) {

			entities[i] = (Entity) {
				.x = 200 * (float) i,
				.y = 800,
				.size = 60,
				.viewRect = {0, 0, 450, 450},
				.texture = textures[i],
				.initialVelocity = {randomFloat() * 20, -360}
			};
		}
	}

	bool gameRunning = true;


	SDL_Event event; //Input event.
	double t = 0.0; // Time for how long the game has been open.
	const double stepTime = 1.0f / 100.0f; // Step time is used for minimum time taken for game to update.

	double currentTime = hireTimeInSeconds(); //Time taken for current frame.
	double accumulator = 0.0; // Used to execute statements until update according to frame time has been compensated.


	//Game loop.
	while (gameRunning) {
		const double newTime = hireTimeInSeconds(); // Time taken for new frame.
		const double frameTime = newTime - currentTime; //Used to keep motion same, in faster, and slower hardware.
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= stepTime) {
			//Listen to window being closed.
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}


			RenderWindow_clear(window);
			RenderWindow_render(window, backgroundTexture);

			for(int i = 0; i < 4; i++) {
				Entity_applyVelocity(&entities[i], &frameTime);
				Entity_applyGravity(&entities[i], 150, &t, &frameTime);
				Entity_render(window, &entities[i]);
			}

			accumulator -= stepTime;
			t += stepTime;
		}

		RenderWindow_display(window);
	}

	free(entities);
	RenderWindow_Destroy(window);

	SDL_Quit();
	return 0;
}
