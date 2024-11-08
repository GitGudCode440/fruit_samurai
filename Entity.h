#pragma once
#include<SDL2/SDL.h>
#include"RenderWindow.h"
#include"Math.h"

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
void Entity_applyGravity(Entity* entity, float gravity, const double* time, const double* deltaTime);
void Entity_applyVelocity(Entity* entity, const double* deltaTime);