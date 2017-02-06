/*
		RENDER.C
		--------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include "render.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PIXEL_WIDTH_SIZE = 10;
const int PIXEL_HEIGHT_SIZE = 10;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 rmask = 0xff000000;
    const Uint32 gmask = 0x00ff0000;
    const Uint32 bmask = 0x0000ff00;
    const Uint32 amask = 0x000000ff;
#else
    const Uint32 rmask = 0x000000ff;
    const Uint32 gmask = 0x0000ff00;
    const Uint32 bmask = 0x00ff0000;
    const Uint32 amask = 0xff000000;
#endif

struct renderer *render_new()
	{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
		}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 320, SDL_WINDOW_SHOWN);
	if (win == NULL)
		{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
		}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL)
		{
		SDL_DestroyWindow(win);
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
		}

	struct renderer *r = memory_alloc(sizeof(*r));
	r->ren = ren;
	return r;
	}


void render_draw(struct renderer *r, char x, char y, char *sprite, char length)
	{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 64, 32, 32,
                                   rmask, gmask, bmask, amask);

	SDL_Rect rect = {0, 0, 10, 10};
	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r->ren, surface);
	printf("Draw sprite at %d %d\n", x, y);
	}
