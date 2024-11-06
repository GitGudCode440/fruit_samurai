#include "Utils.h"

float hireTimeInSeconds() {
	float t = (float) SDL_GetTicks();
	t /= 1000;
	return t;
}

float randomFloat() {
	return (float)rand() / (float)RAND_MAX;
}

const char* getRandomChar() {
	const char ch = (char) ((int) 'a' + (int) (randomFloat() * 26.0f));
	char* str =  malloc(sizeof(char) * 2);

	if (str != NULL) {
		str[0] = ch;
		str[1] = '\0';
	}

	return str;
}