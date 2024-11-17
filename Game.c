#include "Game.h"

const SDL_Color white = {232, 244, 250, 255};
const SDL_Color darkBlue = {0, 2, 36, 255};


int MainGame(const RenderWindow* window, const RenderImage* backgroundTexture, SDL_Event* inputEvent, bool* isGameEnd)
{
	TTF_Font* goodBrushFont = TTF_OpenFont("res/fonts/Good_Brush.ttf", FONT_SIZE);
	TTF_Font* nunitoFont = TTF_OpenFont("res/fonts/Nunito-SemiBold.ttf", FONT_SIZE);


	if (nunitoFont == NULL) {
		printf("Failed to load nunitoFont: %s\n", TTF_GetError());
		return 1;
	}

	if (goodBrushFont == NULL) {
		printf("Failed to load goodBrushFont: %s\n", TTF_GetError());
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


	int scoreCounter = 0;

	RenderImage* scoreTexture = RenderWindow_loadTextureFromFont(window, goodBrushFont,  "SCORE: 69", white);
	const Entity scoreEntity = {
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

	//Showing black screen, before game starts.
	RenderWindow_clear(window);
	RenderWindow_display(window);


	Mix_PlayMusic(backgroundMusic, 0);

	while (Mix_PlayingMusic()) {}

	Mix_FreeMusic(backgroundMusic);



	int timePositionOffset = WINDOW_HEIGHT + 300;

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
			while (SDL_PollEvent(inputEvent)) {
				if (inputEvent->type == SDL_QUIT)
				{
					gameRunning = false;
					(*isGameEnd) = true;
				}
				else if (inputEvent->type == SDL_KEYDOWN) {
					for(int i = 0; i < ENTITY_AMOUNT; i++) {
						if (inputEvent->key.keysym.sym == fontKeys[i][0] && entities[i].textureState == UNSLICED && entities[i].position.y < WINDOW_HEIGHT + WINDOW_BORDER_PADDING) {
							entities[i].textureState = SLICED;
							fontEntities[i].textureState = SLICED;

							if (entities[i].isBomb == true)
							{
								gameRunning = false;
							}

							Mix_PlayChannel(-1, fruitSliceSound, 0);
							scoreCounter += 10;
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

			for(int i = 0; i < ENTITY_AMOUNT; i++) {
				Entity_applyVelocity(&entities[i], &frameTime);
				Entity_applyVelocity(&fontEntities[i], &frameTime);
				Entity_applyGravity(&entities[i], GRAVITY, &t, &frameTime);
				Entity_applyGravity(&fontEntities[i], GRAVITY, &t, &frameTime);

			}

			accumulator -= stepTime;
			t += stepTime;
		}


		RenderWindow_render(window, backgroundTexture->texture);

		for (int i = 0; i < ENTITY_AMOUNT; i++)
		{
			Entity_render(window, &entities[i]);
			Entity_render(window, &fontEntities[i]);
			Entity_render(window, &scoreEntity);
		}

		RenderWindow_display(window);
	}

	free(entities);
	free(fontEntities);
	free(fontKeys);
	free(fruitSliceSound);
	free(sliceSound);
	free(nunitoFont);
	free(goodBrushFont);

	return 0;

}

int GameOver(const RenderWindow* window, const RenderImage* backgroundTexture, SDL_Event* inputEvent, bool* isGameEnd)
{
	if ((*isGameEnd) == true) return 1;

	TTF_Font* goodBrushFont = TTF_OpenFont("res/fonts/Good_Brush.ttf", FONT_SIZE * 3);

	if (goodBrushFont == NULL)
	{
		printf("Failed to load good brush font. Error: %s\n", TTF_GetError());
		return 1;
	}

	RenderImage* gameOverFontImage = RenderWindow_loadTextureFromFont(window, goodBrushFont,  "GAME OVER!", white);
	const Entity gameOverEntity = (Entity) {
		.position = {
			WINDOW_WIDTH / 2 - gameOverFontImage->width / 2,
			WINDOW_HEIGHT / 2 - gameOverFontImage->height / 2,
		},
		.size = {gameOverFontImage->width, gameOverFontImage->height},
		.viewRect = {0, 0, gameOverFontImage->width, gameOverFontImage->height},
		.textureState = 0,
		.texture[0] = gameOverFontImage->texture
	};


	goodBrushFont = TTF_OpenFont("res/fonts/Good_Brush.ttf", FONT_SIZE);
	const RenderImage* gameOverMessage = RenderWindow_loadTextureFromFont(window, goodBrushFont, "CLOSE THE WINDOW, OR PRESS R TO TRY AGAIN!", white);
	const Entity gameOverMessageEntity = (Entity) {
		.position = {
			gameOverEntity.position.x -  ((float) (gameOverMessage->width - gameOverFontImage->width) / 2),
			gameOverEntity.position.y + FONT_SIZE * 3  + 16
		},
		.size = {gameOverMessage->width, gameOverMessage->height},
		.viewRect = {0, 0, gameOverMessage->width, gameOverMessage->height},
		.textureState = 0,
		.texture[0] = gameOverMessage->texture
	};

	free(gameOverFontImage);

	bool isGameOverRunning = true;
	while(isGameOverRunning)
	{
		while(SDL_PollEvent(inputEvent))
		{
			if (inputEvent->type == SDL_QUIT)
			{
				*isGameEnd = true;
				isGameOverRunning = false;
			}
			else if (inputEvent->type == SDL_KEYDOWN)
			{
				if (inputEvent->key.keysym.sym == 'r')
				{
					*isGameEnd = false;
					isGameOverRunning = false;
				}
			}
		}

		RenderWindow_clear(window);
		RenderWindow_render(window, backgroundTexture->texture);
		Entity_render(window, &gameOverEntity);
		Entity_render(window, &gameOverMessageEntity);
		RenderWindow_display(window);
	}

	free(goodBrushFont);

	return 0;
}