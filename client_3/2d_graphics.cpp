#include "2d_graphics.h"

SDL_Renderer *rend;

void WindowSetup(int xpos, int ypos, int weidth, int height)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Window *window = SDL_CreateWindow("2D", xpos, ypos, weidth, height, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void GraphicsWindow()
{
	SDL_Renderer *renderer = rend;
	SDL_Event e;
	int a = 0;
	while (1)
	{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{

		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawPoint(renderer, a, 300);
	SDL_RenderPresent(renderer);
	a++;
	SDL_Delay(100);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	}
}