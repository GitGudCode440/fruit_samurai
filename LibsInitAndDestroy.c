//
// Created by mmaha on 11/7/24.
//

#include "LibsInitAndDestroy.h"

int LibsInit() {
	//Initialize SDL, and SDL_image. SDL_image is for loading images.

	//If Initializing SDL returns greater than 0, there is an error.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	};

	//If Initializing SDL_image does not exist, there is an error.
	if(!IMG_Init(IMG_INIT_PNG)) {
		printf("IMG_Init failed: %s\n", SDL_GetError());
		return 1;
	};

	if (TTF_Init() == -1) {
		printf("TTF_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return 1;
	}

	return 0;

}

void libsDestroy() {
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}