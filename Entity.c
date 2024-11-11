#include "Entity.h"

void generateEntity(const RenderWindow* window, Entity** entities) {
	//Load texture for every fruit.
	SDL_Texture* fruitTextures[4] = {
		RenderWindow_loadTexture(window, "res/textures/fruits/watermelon.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/coconut.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/peach.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/strawberry.png")
	};

	SDL_Texture* slicedFruitTexture[4] = {
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_watermelon.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_coconut.png"),
		RenderWindow_loadTexture(window, "res/textures/fruits/cut_peach.png"),
		RenderWindow_loadTexture(window,"res/textures/fruits/cut_strawberry.png"),
	};


	//Initialize entity with required params, like position, size, etc.

	for(int i = 0; i < ENTITY_AMOUNT; i++) {
		const int entityTextureRandomIndex = (int) (randomFloat() * 3);
		(*entities)[i] = (Entity) {
			.position = {  WINDOW_WIDTH / ENTITY_AMOUNT * i, WINDOW_HEIGHT},
			.size = {60, 60},
			.viewRect = {0, 0, 450, 450},
			.texture[UNSLICED] = fruitTextures[entityTextureRandomIndex],
			.texture[SLICED] = slicedFruitTexture[entityTextureRandomIndex],
			.initialVelocity = {randomFloat() * 20, -360},
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
			.position = {
				entities[i].position.x + (float) entities[i].size.x / 2 - (float) FONT_SIZE / 2,
				entities[i].position.y + (float) entities[i].size.y / 2 - (float) FONT_SIZE
			},
			.size = {FONT_SIZE, FONT_SIZE * 2},
			.viewRect = {0, 0, fontTextures[i]->width, fontTextures[i]->height},
			.texture[0] = fontTextures[i]->texture,
			.initialVelocity = entities[i].initialVelocity
		};

	}

	free(fontTextures);

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
