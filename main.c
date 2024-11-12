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
#include "globals.h"

const SDL_Color darkBlue = {0, 2, 36, 255};


int main(int argc, char* argv[])
{
	int timePositionOffset = WINDOW_HEIGHT + 3000;
	//Initialize libraries.
	if(LibsInit() != 0) return 1;


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	srand(time(NULL));



	//RenderWindow is a combination of a renderer, and a window.
	//RenderWindow.h also comes with functions that are helpful to display things on the screen.
	const RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", WINDOW_WIDTH, WINDOW_HEIGHT);

	//Wood background.
	SDL_Texture* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");

	TTF_Font* nunitoFont = TTF_OpenFont("res/fonts/Nunito-SemiBold.ttf", FONT_SIZE);

	TTF_Font* brushKingFont = TTF_OpenFont("res/fonts/Good_Brush.ttf", FONT_SIZE);
	const SDL_Color white = {232, 244, 250, 255};

	if (nunitoFont == NULL) {
		printf("Failed to load nunitoFont: %s\n", TTF_GetError());
		return 1;
	}

	if (brushKingFont == NULL) {
		printf("Failed to load brushKingFont: %s\n", TTF_GetError());
		return 1;
	}

	Mix_Music* backgroundMusic = Mix_LoadMUS("res/audio/Game-start.wav");
	Mix_Chunk* fruitSliceSound = Mix_LoadWAV("res/audio/Splatter-Small-1.wav");
	Mix_Chunk* sliceSound = Mix_LoadWAV("res/audio/Sword-swipe-7.wav");

	if (backgroundMusic == NULL && fruitSliceSound == NULL) {
		printf("Failed to load background music. Error: %s\n", Mix_GetError());
	}

	Mix_VolumeMusic(8);
	Mix_VolumeChunk(fruitSliceSound, 36);
	Mix_VolumeChunk(sliceSound, 16);


	//Entities like watermelon, bananas, cherry, strawberry, etc.



	int scoreCounter = 0;

	RenderImage* scoreTexture = RenderWindow_loadTextureFromFont(window, brushKingFont,  "SCORE: 69", white);
	Entity scoreEntity = {
		.position = {WINDOW_WIDTH - scoreTexture->width - WINDOW_BORDER_PADDING, WINDOW_BORDER_PADDING},
		.viewRect = {0, 0, scoreTexture->width, scoreTexture->height},
		.size = {scoreTexture->width, scoreTexture->height},
		.texture[0] = scoreTexture->texture
	};

	//Things initialized inside braces, only exist inside braces.

	Entity* entities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
	generateEntity(window, entities);

	const char** fontKeys = getRandomUniqueCharArray();
	Entity* fontEntities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
	generateFontEntity(window, entities, fontEntities, nunitoFont, fontKeys, darkBlue);


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
						//TODO: The third condition of if must be rewritten to account for future random upward velocity.
						if (event.key.keysym.sym == fontKeys[i][0] && entities[i].textureState == UNSLICED && entities[0].position.y < WINDOW_HEIGHT + WINDOW_BORDER_PADDING) {
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

			while(entities[0].position.y > WINDOW_HEIGHT + timePositionOffset) {
				free(entities);
				free(fontEntities);
				free(fontKeys);

				entities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
				fontEntities = malloc(sizeof(Entity) * ENTITY_AMOUNT);
				fontKeys = getRandomUniqueCharArray();

				generateEntity(window, entities);
				generateFontEntity(window, entities, fontEntities, nunitoFont, fontKeys, darkBlue);

				t = 0.0;
				if (timePositionOffset > 0)
					timePositionOffset -= 10;

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
				Entity_render(window, &scoreEntity);
			}

			accumulator -= stepTime;
			t += stepTime;
		}

		RenderWindow_display(window);

	}

	free(entities);
	free(fontEntities);
	free(fontKeys);
	free(fruitSliceSound);
	free(sliceSound);

	RenderWindow_Destroy(window);

	//Destroy libraries.
	LibsDestroy();

	return 0;
}