#include "Entity.h"

void generateEntity(const RenderWindow* window, Entity* entities) {
	//Load texture for every fruit.
	RenderImage* fruitTextures[4] = {
		RenderWindow_loadTexture(window, "res/textures/fruits/watermelon.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/coconut.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/peach.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/strawberry.png")
	};

	RenderImage* slicedFruitTexture[4] = {
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_watermelon.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_coconut.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_peach.png"),
		RenderWindow_loadTexture(window,"res/textures/fruits/cut_strawberry.png"),
	};

	RenderImage* bomb = RenderWindow_loadTexture(window, "res/textures/bomb.png");

	//Initialize entity with required params, like position, size, etc.

	for(int i = 0; i < ENTITY_AMOUNT; i++) {
		const int entityTextureRandomIndex = (int) (randomFloat() * 3.9);
		const float scale = 0.25f;
		const bool isBomb = randomFloat() > 0.9;
		const RenderImage* unslicedImage = isBomb ? bomb : fruitTextures[entityTextureRandomIndex];
		entities[i] = (Entity) {
			.isBomb = isBomb,
			.textureState = UNSLICED,
			.position = {  WINDOW_WIDTH / ENTITY_AMOUNT * i, WINDOW_HEIGHT},
			.size = {
				(int) ((float) unslicedImage->width * scale),
				(int) ((float) unslicedImage->height * scale)
			},
			.viewRect = {0, 0, unslicedImage->width, unslicedImage->height},
			.texture[UNSLICED] = unslicedImage->texture,
			.texture[SLICED] = isBomb ? NULL : slicedFruitTexture[entityTextureRandomIndex]->texture,
			.initialVelocity = {randomFloat() * 20, INITIAL_UPWARD_VELOCITY - randomFloat() * 20},
		};
	}

}

void generateFontTextureArray(RenderImage** fontTextureArray, const RenderWindow* window, TTF_Font* font, const char** fontKeys, const SDL_Color color) {
	for (int i = 0; i < ENTITY_AMOUNT; i++) {
		fontTextureArray[i] = RenderWindow_loadTextureFromFont(window, font, fontKeys[i], color);
	}
}

void generateFontEntity(const RenderWindow*  window, const Entity* entities, Entity* fontEntities, TTF_Font* font, const char** fontKey, const SDL_Color color) {
	RenderImage** fontTextures = malloc(sizeof(RenderImage) * ENTITY_AMOUNT);

	if (fontTextures == NULL) {
		return;
	}


	generateFontTextureArray(fontTextures, window, font, fontKey, color);

	for (int i = 0; i < ENTITY_AMOUNT; i++) {
		fontEntities[i] = (Entity) {
			.textureState = UNSLICED,
			.position = {
				entities[i].position.x + (float) entities[i].size.x / 2 - (float) FONT_SIZE / 2,
				entities[i].position.y + (float) entities[i].size.y / 2 - (float) FONT_SIZE
			},
			.size = {FONT_SIZE, FONT_SIZE * 2},
			.viewRect = {0, 0, fontTextures[i]->width, fontTextures[i]->height},
			.texture[UNSLICED] = fontTextures[i]->texture,
			.texture[SLICED] = NULL,
			.initialVelocity = entities[i].initialVelocity
		};

	}

	free(fontTextures);

}

void generateScoreEntity(const RenderWindow* window, Entity* entity, const RenderImage* scoreTexture)
{
	*entity = (Entity) {
		.position = {WINDOW_WIDTH - scoreTexture->width - WINDOW_BORDER_PADDING, WINDOW_BORDER_PADDING},
		.viewRect = {0, 0, scoreTexture->width, scoreTexture->height},
		.size = {scoreTexture->width, scoreTexture->height},
		.texture[0] = scoreTexture->texture
	};

}


void Entity_render(const RenderWindow* renderWindow, const Entity* entity) {
	SDL_Rect src;
	src.x = entity->viewRect.x;
	src.y = entity->viewRect.y;
	src.w = entity->viewRect.w;
	src.h = entity->viewRect.h;

	SDL_Rect dest;
	dest.x = (int) entity->position.x;
	dest.y = (int) entity->position.y;
	dest.w = entity->size.x;
	dest.h = entity->size.y;
	SDL_RenderCopy(renderWindow->renderer, entity->texture[entity->textureState], &src, &dest);
}

void Entity_applyGravity(Entity* entity, const float gravity, const double* time, const double* deltaTime) {
	entity->position.y += gravity * (float) *time * (float) *deltaTime;
}

void Entity_applyVelocity(Entity* entity, const double* deltaTime) {
	entity->position.x += entity->initialVelocity.x * (float) (*deltaTime);
	entity->position.y += entity->initialVelocity.y * (float) (*deltaTime);
}
