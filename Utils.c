#include "Utils.h"
#include<SDL2/SDL.h>

float hireTimeInSeconds() {
	float t = (float) SDL_GetTicks();
	t /= 1000;
	return t;
}

float randomFloat() {
	srand(time(NULL));
	return (float)rand() / (float)RAND_MAX;
}