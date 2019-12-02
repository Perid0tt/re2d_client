#include "2d_graphics.h"


SDL_Renderer *rendererg;
char keys[4] = { '-','-','-','-' };

extern player gui;
extern player me;
extern dualnet_int Framecount;
extern  coord ForMeCalc_c;

const int FRAMES_PER_SECOND = 60;
int frame = 0;
bool cap = true;
int frametime;

void WindowSetup(int xpos, int ypos, int weidth, int height)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Window *window = SDL_CreateWindow("2D", xpos, ypos, weidth, height, SDL_WINDOW_SHOWN);
	rendererg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void GraphicsWindow()
{
	SDL_Renderer *renderer = rendererg;
	SDL_Event e;
	while (1)
	{
		frametime = clock();

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 26)keys[0] = 'w';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 4)keys[1] = 'a';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 22)keys[2] = 's';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 7)keys[3] = 'd';
				break;
			case SDL_KEYUP:
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 26)keys[0] = '-';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 4)keys[1] = '-';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 22)keys[2] = '-';
				if (SDL_GetScancodeFromKey(e.key.keysym.sym) == 7)keys[3] = '-';
				break;
			}
		}

		setphysics();

		fill_circle(renderer, gui.c.x, gui.c.y, 25, 255, 0, 0, 255);
		fill_circle(renderer, me.c.x, me.c.y, 25, 0, 255, 0, 255);
		//fill_circle(renderer, ForMeCalc_c.x, ForMeCalc_c.y, 15, 255, 0, 255, 255);

		for (int i = 0; i < gui.dobj_num; i++)
			fill_circle(renderer, gui.dobj[i].c.x + 10*cos(gui.dobj[i].speed.angle), gui.dobj[i].c.y + 10 * sin(gui.dobj[i].speed.angle), 5, 0, 200, 200, 255);
		for (int i = 0; i < me.dobj_num; i++)
			fill_circle(renderer, me.dobj[i].c.x + 10 * cos(me.dobj[i].speed.angle), me.dobj[i].c.y + 10 * sin(me.dobj[i].speed.angle), 5, 200, 0, 200, 255);

		SDL_RenderPresent(renderer);
		SDL_Delay(1);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		Framecount.me++;
		frame++;
		
		if ((cap == true) && (clock() - frametime < 1000 / FRAMES_PER_SECOND))
			SDL_Delay((1000 / FRAMES_PER_SECOND) - (clock() - frametime));
	}
}

void fill_circle(SDL_Renderer *surface, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	static const int BPP = 4;

	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(surface, r, g, b, a);
		SDL_RenderDrawLine(surface, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(surface, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
}