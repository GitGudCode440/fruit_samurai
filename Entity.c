#include "Entity.h"
#include"RenderWindow.h"

void Entity_Init(Entity *entity,
	const float x, const float y,
	const int size, const int textureSize,  SDL_Texture* texture
	)
{
	entity->x = x;
	entity->y = y;
	entity->viewRect.x = 0;
	entity->viewRect.y = 0;
	entity->viewRect.w = textureSize;
	entity->viewRect.h = textureSize;
	entity->size = size;
	entity->texture = texture;
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

void Entity_applyGravity(Entity* entity, const float gravity) {
	entity->y += gravity;
}
