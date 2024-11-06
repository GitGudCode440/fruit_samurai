#include "Entity.h"


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
