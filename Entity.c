#include "Entity.h"


void Entity_append(Entity** entities, Entity entity, int* count) {
	Entity* temp = realloc(*entities, sizeof(Entity) * (*count + 1));
	if (temp == NULL) return;
	*entities = temp;
	(*entities)[*count] = entity;
	(*count)++;
}

void Entity_render(const RenderWindow* renderWindow, const Entity* entity) {
	SDL_Rect src;
	src.x = entity->viewRect.x;
	src.y = entity->viewRect.y;
	src.w = entity->viewRect.w;
	src.h = entity->viewRect.h;

	SDL_Rect dest;
	dest.x = (int) entity->x;
	dest.y = (int) entity->y;
	dest.w = entity->size;
	dest.h = entity->size;
	SDL_RenderCopy(renderWindow->renderer, entity->texture, &src, &dest);
}

void Entity_applyGravity(Entity* entity, const float gravity, const uint32_t* time, const double* deltaTime) {
	entity->y += gravity * (float) *time * (float) *deltaTime;
}

void Entity_applyVelocity(Entity* entity, const double* deltaTime) {
	entity->x += entity->initialVelocity.x * (float) (*deltaTime);
	entity->y += entity->initialVelocity.y * (float) (*deltaTime);
}
