#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "LibsInitAndDestroy.h"
#include "Utils.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define ENTITY_AMOUNT 4
#define FONT_SIZE 24
#define INITIAL_UPWARD_VELOCITY (-360)
#define GRAVITY 150

int main(int argc, char* argv[])
{
	//Initialize libraries.
	if(LibsInit() != 0) return 1;


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	srand(time(NULL));



	//RenderWindow is a combination of a renderer, and a window.
	//RenderWindow.h also comes with functions that are helpful to display things on the screen.
	const RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", WINDOW_WIDTH, WINDOW_HEIGHT);

	//Wood background.
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");

	TTF_Font* font = TTF_OpenFont("res/fonts/Nunito-SemiBold.ttf", FONT_SIZE);
	const SDL_Color color = {0, 2, 36, 255};

	if (font == NULL) {
		printf("Failed to load font: %s\n", TTF_GetError());
		return 1;
	}

	Mix_Music* backgroundMusic = Mix_LoadMUS("res/audio/Game-start.wav");
	Mix_Chunk* fruitSliceSound = Mix_LoadWAV("res/audio/Splatter-Small-1.wav");
	Mix_Chunk* sliceSound = Mix_LoadWAV("res/audio/Sword-swipe-7.wav");

	if (backgroundMusic == NULL && fruitSliceSound == NULL) {
		printf("Failed to load background music. Error: %s\n", Mix_GetError());
	}

	Mix_VolumeMusic(8);
	Mix_VolumeChunk(fruitSliceSound, 26);
	Mix_VolumeChunk(sliceSound, 16);


	//Entities like watermelon, bananas, cherry, strawberry, etc.
	Entity* entities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
	Entity* fontEntities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
	const char* fontKey[ENTITY_AMOUNT] = {
		getRandomChar(),
		getRandomChar(),
		getRandomChar(),
		getRandomChar(),
	};

	//Things initialized inside braces, only exist inside braces.
	{


		//Load texture for every fruit.
		SDL_Texture* textures[ENTITY_AMOUNT] = {
			RenderWindow_loadTexture(window, "res/textures/watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/coconut.png"),
			RenderWindow_loadTexture(window, "res/textures/peach.png"),
			RenderWindow_loadTexture(window, "res/textures/strawberry.png")
		};

		SDL_Texture* slicedTexture[ENTITY_AMOUNT] = {
			RenderWindow_loadTexture(window, "res/textures/cut_watermelon.png"),
			RenderWindow_loadTexture(window, "res/textures/cut_coconut.png"),
			RenderWindow_loadTexture(window, "res/textures/cut_peach.png"),
			RenderWindow_loadTexture(window,"res/textures/cut_strawberry.png"),
		};

		RenderImage* fontTexture[ENTITY_AMOUNT] = {
			RenderWindow_loadTextureFromFont(window, font, fontKey[0], color),
			RenderWindow_loadTextureFromFont(window, font, fontKey[1], color),
			RenderWindow_loadTextureFromFont(window, font, fontKey[2], color),
			RenderWindow_loadTextureFromFont(window, font, fontKey[3], color)
		};



		//Initialize entity with required params, like position, size, etc.

		for(int i = 0; i < ENTITY_AMOUNT; i++) {
			const int entityTextureRandomIndex = (int) (randomFloat() * 3.0f);
			entities[i] = (Entity) {
				.position = {200 * (float) i, 800},
				.size = {60, 60},
				.viewRect = {0, 0, 450, 450},
				.texture[UNSLICED] = textures[entityTextureRandomIndex],
				.texture[SLICED] = slicedTexture[entityTextureRandomIndex],
				.initialVelocity = {randomFloat() * 20, -360},
			};
			fontEntities[i] = (Entity) {
				.position = {
					entities[i].position.x + (float) entities[i].size.x / 2 - (float) FONT_SIZE / 2,
					entities[i].position.y + (float) entities[i].size.y / 2 - (float) FONT_SIZE
				},
				.size = {FONT_SIZE, FONT_SIZE * 2},
				.viewRect = {0, 0, fontTexture[i]->width, fontTexture[i]->height},
				.texture[0] = fontTexture[i]->texture,
				.initialVelocity = entities[i].initialVelocity
			};

		}
	}

	bool gameRunning = true;


	Mix_PlayMusic(backgroundMusic, 0);

	while (Mix_PlayingMusic()) {}

	Mix_FreeMusic(backgroundMusic);


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
				else if (event.type == SDL_KEYDOWN) {
					for(int i = 0; i < ENTITY_AMOUNT; i++) {
						if (event.key.keysym.sym == fontKey[i][0] && entities[i].textureState == UNSLICED) {
							entities[i].textureState = SLICED;
							Mix_PlayChannel(-1, fruitSliceSound, 0);
							fontEntities[i].texture[0] = NULL;
						}
						else {
							Mix_PlayChannel(-1, sliceSound, 0);
						}
					}
				}
			}


			RenderWindow_clear(window);
			RenderWindow_render(window, backgroundTexture);

			for(int i = 0; i < ENTITY_AMOUNT; i++) {
				Entity_applyVelocity(&entities[i], &frameTime);
				Entity_applyVelocity(&fontEntities[i], &frameTime);
				Entity_applyGravity(&entities[i], GRAVITY, &t, &frameTime);
				Entity_applyGravity(&fontEntities[i], GRAVITY, &t, &frameTime);
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
	free(fruitSliceSound);
	free(sliceSound);
	RenderWindow_Destroy(window);

	//Destroy libraries.
	LibsDestroy();

	return 0;
}
