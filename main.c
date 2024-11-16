#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<stdio.h>
#include<stdbool.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "Game.h"
#include "LibsInitAndDestroy.h"
#include "Utils.h"
#include "globals.h"



int main(int argc, char* argv[])
{
	//Initialize libraries.
	if(LibsInit() != 0) return 1;


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	srand(time(NULL));



	//RenderWindow is a combination of a renderer, and a window.
	//RenderWindow.h also comes with functions that are helpful to display things on the screen.
	const RenderWindow* window = RenderWindow_Init("Fruit Samurai: A Typing Madness", WINDOW_WIDTH, WINDOW_HEIGHT);
	const RenderImage* backgroundTexture = RenderWindow_loadTexture(window, "res/textures/background.png");
	SDL_Event event;


	MainGame(window, backgroundTexture, &event);
	GameOver(window, backgroundTexture, &event);



	RenderWindow_Destroy(window);

	//Destroy libraries.
	LibsDestroy();

	return 0;
}