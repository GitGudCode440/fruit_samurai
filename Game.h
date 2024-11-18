#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<stdio.h>
#include<stdbool.h>
#include<math.h>

#include "RenderWindow.h"
#include "Entity.h"
#include "LibsInitAndDestroy.h"
#include "Utils.h"
#include "globals.h"

// RETUNS 0, IF SUCCESSFUL. OTHERWISE 1.
int StartMenu(const RenderWindow* window, const RenderImage* backgroundTexture, SDL_Event* inputEvent, bool* isStartMenuEnd);
int MainGame(const RenderWindow* window, const RenderImage* backgroundTexture, SDL_Event* inputEvent, bool* isGameEnd);
int GameOver(const RenderWindow* window, const RenderImage* backgroundTexture, SDL_Event* inputEvent, bool* isGameEnd);

