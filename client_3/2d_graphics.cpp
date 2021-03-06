#include "2d_graphics.h"

SDL_Renderer *rendererg;
char keys[6] = { '-','-','-','-','-','-' };

extern player gui;
extern player me;
extern dualnet_int Framecount;
extern  coord ForMeCalc_c;

const int FRAMES_PER_SECOND = 60;
int frame = 0;
bool cap = true;
int frametime;
SDL_Event sev;

int WindowWeidht = 800, WindowHeight = 800;

void WindowSetup(int xpos, int ypos)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Window *window = SDL_CreateWindow("2D", xpos, ypos, WindowWeidht, WindowHeight, SDL_WINDOW_SHOWN);
	rendererg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void GetInput(SDL_Event e)
{
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
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)keys[4] = '<';
			if (e.button.button == SDL_BUTTON_RIGHT)keys[5] = '>';
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)keys[4] = '-';
			if (e.button.button == SDL_BUTTON_RIGHT)keys[5] = '-';
			break;
		}
	}
}

void GraphicsWindow()
{
	SDL_Renderer *renderer = rendererg;
	while (1)
	{
		frametime = clock();

		GetInput(sev);

		setphysics();

		fill_circle(renderer, gui.c.x + WindowWeidht / 2, gui.c.y + WindowHeight / 2, 50, 255, 0, 0, 255);
		fill_circle(renderer, me.c.x + WindowWeidht / 2, me.c.y + WindowHeight / 2, 50, 0, 255, 0, 255);
		//fill_circle(renderer, ForMeCalc_c.x, ForMeCalc_c.y, 15, 255, 0, 255, 255);

		/*for (int i = 0; i < gui.dobj_num; i++)
			fill_circle(renderer, gui.dobj[i].c.x + WindowWeidht / 2, gui.dobj[i].c.y + WindowHeight / 2, 5, 0, 200, 200, 255);
		for (int i = 0; i < me.dobj_num; i++)
			fill_circle(renderer, me.dobj[i].c.x + WindowWeidht / 2, me.dobj[i].c.y + WindowHeight / 2, 5, 200, 0, 200, 255);*/

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