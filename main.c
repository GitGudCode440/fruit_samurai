#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Utils.h"

#define FONT_SIZE 24
int main(int argc, char* argv[])
{
	//Initialize SDL, and SDL_image. SDL_image is for loading images.

	//If Initializing SDL returns greater than 0, there is an error.
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	};

	//If Initializing SDL_image does not exist, there is an error.
	if(!IMG_Init(IMG_INIT_PNG)) {
		printf("IMG_Init failed: %s\n", SDL_GetError());
		return 1;
	};

	if (TTF_Init() == -1) {
		printf("TTF_Init failed: %s\n", SDL_GetError());
		return 1;
	}


	//RenderWindow is a combination of a renderer, and a window.
	//RenderWindow.h also comes with functions that are helpful to display things on the screen.
	const RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", 800, 600);

	//Wood background.
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");

	TTF_Font* font = TTF_OpenFont("res/fonts/Nunito-SemiBold.ttf", FONT_SIZE);
	const SDL_Color color = {0, 2, 36, 255};

	if (font == NULL) {
		printf("Failed to load font: %s\n", TTF_GetError());
		return 1;
	}

	//Entities like watermelon, bananas, cherry, strawberry, etc.
	Entity* entities = malloc(sizeof(Entity) * 4);
	Entity* fontEntities = malloc(sizeof(Entity) * 4);


	//Things initialized inside braces, only exist inside braces.
	{
		srand(time(NULL));

		//Load texture for every fruit.
		SDL_Texture* textures[4] = {
			RenderWindow_loadTexture(window, "res/textures/watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/bananas.png"),
			RenderWindow_loadTexture(window, "res/textures/cherry.png"),
			RenderWindow_loadTexture(window, "res/textures/strawberry.png")
		};



		RenderImage* fontTexture[4] = {
			RenderWindow_loadTextureFromFont(window, font, getRandomChar(), color),
			RenderWindow_loadTextureFromFont(window, font, getRandomChar(), color),
			RenderWindow_loadTextureFromFont(window, font, getRandomChar(), color),
			RenderWindow_loadTextureFromFont(window, font, getRandomChar(), color)
		};



		//Initialize entity with required params, like position, size, etc.

		for(int i = 0; i < 4; i++) {

			entities[i] = (Entity) {
				.position = {200 * (float) i, 800},
				.size = {60, 60},
				.viewRect = {0, 0, 450, 450},
				.texture = textures[i],
				.initialVelocity = {randomFloat() * 20, -360}
			};
			fontEntities[i] = (Entity) {
				.position = {
					entities[i].position.x + (float) entities[i].size.x / 2 - (float) FONT_SIZE / 2,
					entities[i].position.y + (float) entities[i].size.y / 2 - (float) FONT_SIZE /2
				},
				.size = {FONT_SIZE, FONT_SIZE * 2},
				.viewRect = {0, 0, fontTexture[i]->width, fontTexture[i]->height},
				.texture = fontTexture[i]->texture,
				.initialVelocity = entities[i].initialVelocity
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
				Entity_applyVelocity(&fontEntities[i], &frameTime);
				Entity_applyGravity(&entities[i], 150, &t, &frameTime);
				Entity_applyGravity(&fontEntities[i], 150, &t, &frameTime);
				Entity_render(window, &entities[i]);
				Entity_render(window, &fontEntities[i]);
			}

			accumulator -= stepTime;
			t += stepTime;
		}

		RenderWindow_display(window);
	}

	free(entities);
	free(fontEntities);
	RenderWindow_Destroy(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
