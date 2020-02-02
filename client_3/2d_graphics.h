#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "physics.h"
#include "convert.h"
#include <ctime>

#define FRAMELENGH = 16.66666666;

using namespace std;

void WindowSetup(int, int);

void GraphicsWindow();

void fill_circle(SDL_Renderer *suface, int, int, int, Uint8, Uint8, Uint8, Uint8);