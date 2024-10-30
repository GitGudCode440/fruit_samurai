#include "Entity.h"

void Entity_Init(Entity *entity, const float x, const float y, SDL_Texture* texture) {
	entity->x = x;
	entity->y = y;
	entity->currentFrame.x = 0;
	entity->currentFrame.y = 0;
	entity->currentFrame.w = 32;
	entity->currentFrame.h = 32;
}
