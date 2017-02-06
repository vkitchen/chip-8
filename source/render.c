/*
		RENDER.C
		--------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include "memory.h"
#include "render.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PIXEL_WIDTH = 10;
const int PIXEL_HEIGHT = 10;

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
		return NULL;
		}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL)
		{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
		}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL)
		{
		SDL_DestroyWindow(win);
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
		}

	struct renderer *r = memory_alloc(sizeof(*r));
	r->win = win;
	r->ren = ren;
	r->screen[5] = 1; // XXX test
	return r;
	}

void render_free(struct renderer *r)
	{
	SDL_DestroyRenderer(r->ren);
	SDL_DestroyWindow(r->win);
	SDL_Quit();
	}

void render_draw(struct renderer *r, char x, char y, char *sprite, char length)
	{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, PIXEL_WIDTH, PIXEL_HEIGHT, 32,
                                   rmask, gmask, bmask, amask);

	for (int x = 0; x < SCREEN_WIDTH/PIXEL_WIDTH; x++)
		for (int y = 0; y < SCREEN_HEIGHT/PIXEL_HEIGHT; y++)
			{
			SDL_Rect rect = {x, y, PIXEL_WIDTH, PIXEL_HEIGHT};
			if (r->screen[SCREEN_WIDTH/PIXEL_WIDTH * y + x])
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
			else
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
			}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r->ren, surface);

	SDL_FreeSurface(surface);
  printf("\033[6;0HDraw sprite at %d %d\n", x, y);
  draw_calls++;
	//printf("Draw sprite at %d %d\n", x, y);

	if (tex == NULL)
		{
		SDL_DestroyRenderer(r->ren);
		SDL_DestroyWindow(r->win);
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		SDL_Quit();
		return;
		}

	//First clear the renderer
	SDL_RenderClear(r->ren);
	//Draw the texture
	SDL_RenderCopy(r->ren, tex, NULL, NULL);
	//Update the screen
	SDL_RenderPresent(r->ren);

	SDL_DestroyTexture(tex);
	}
