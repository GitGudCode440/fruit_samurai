#pragma once
#include<SDL2/SDL.h>
#include"RenderWindow.h"
#include"Math.h"
#include"Utils.h"
#include"globals.h"

enum TEXTURE_STATE {
	UNSLICED,
	SLICED
};

typedef struct {
	Vector2f position;
	Vector2f initialVelocity;
	Vector2i size;
	SDL_Rect viewRect;
	SDL_Texture* texture[2];
	enum TEXTURE_STATE textureState;
} Entity;


void Entity_render(const RenderWindow* renderWindow, const Entity* entity);
void generateEntity(const RenderWindow* window, Entity** entities);
void generateFontEntity(const RenderWindow*  window, Entity* entities, Entity* fontEntities, TTF_Font* font, const char** fontKey, const SDL_Color color);
void Entity_applyGravity(Entity* entity, float gravity, const double* time, const double* deltaTime);
void Entity_applyVelocity(Entity* entity, const double* deltaTime);