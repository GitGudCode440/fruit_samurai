#pragma once
#include<SDL2/SDL.h>
#include"RenderWindow.h"
#include"Vector2f.h"

typedef struct {
	float x, y, z;
	Vector2f initialVelocity;
	int size;
	SDL_Rect viewRect;
	SDL_Texture* texture;
} Entity;


void Entity_render(const RenderWindow* renderWindow, const Entity* entity);
void Entity_applyGravity(Entity* entity, float gravity, const double* time, const double* deltaTime);
void Entity_applyVelocity(Entity* entity, const double* deltaTime);