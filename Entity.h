#pragma once
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "RenderWindow.h"

typedef struct {
	float x, y, z;
	int size;
	SDL_Rect viewRect;
	SDL_Texture* texture;
} Entity;

void Entity_Init(Entity *entity, float x, float y, int size, int textureSize, SDL_Texture* texture);
void Entity_render(const RenderWindow* renderWindow, const Entity* entity);
void Entity_applyGravity(Entity* entity, float gravity);