#pragma once
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>

typedef struct {
	float x, y, z;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
} Entity;

void Entity_Init(Entity *entity, float x, float y, SDL_Texture* texture);