#pragma once
#include <stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<SDL2/SDL.h>

#include "globals.h"

static  float hireTimeInSeconds() {
	float t = (float) SDL_GetTicks();
	t /= 1000;
	return t;
}

static float randomFloat() {
	return (float)rand() / (float)RAND_MAX;
}

static const char* getRandomChar() {
	const char ch = (char) ((int) 'a' + (int) (randomFloat() * 26.0f));
	char* str =  malloc(sizeof(char) * 2);

	if (str != NULL) {
		str[0] = ch;
		str[1] = '\0';
	}

	return str;
}

static const char** getRandomUniqueCharArray() {

	const char** charStrArr = malloc(sizeof(char*) * ENTITY_AMOUNT);

	if (charStrArr ==  NULL) {
		return NULL;
	}

	for (int i = 0; i < ENTITY_AMOUNT; i++) {
		const char* strCh = getRandomChar();
		charStrArr[i] = strCh;

	}

	return charStrArr;
}