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

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;
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

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH * PIXEL_WIDTH, SCREEN_HEIGHT * PIXEL_HEIGHT, SDL_WINDOW_SHOWN);
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

    for (int i = 0; i < 2048; i++)
        r->screen[i] = 0;

	return r;
	}

void render_free(struct renderer *r)
	{
	SDL_DestroyRenderer(r->ren);
	SDL_DestroyWindow(r->win);
	SDL_Quit();
	}

int render_draw(struct renderer *r, char x, char y, char *sprite, char height)
	{
    int out = 0;
    // printf("Sprite at %d, %d\n", x, y);
    /* Add the sprite on to the screen */
    for (int i = 0; i < height; i++)
        {
        // printf("Sprite: b"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(sprite[i]));

        for (int bs = 0; bs < 8; bs++)
            if (sprite[i] & 128 >> bs)
                {
                out |= r->screen[SCREEN_WIDTH * y + (x + bs)];
                r->screen[SCREEN_WIDTH * y + (x + bs)] ^= 1;
                }
        y++;
        }

    /* Render the screen */
    SDL_Surface *surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                   rmask, gmask, bmask, amask);

	for (int y = 0; y < SCREEN_WIDTH; y++)
		for (int x = 0; x < SCREEN_HEIGHT; x++)
			{
			SDL_Rect rect = {x, y, PIXEL_WIDTH, PIXEL_HEIGHT};

			if (r->screen[SCREEN_WIDTH * x + y])
                {
                printf("White %d, %d\n", x, y);
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
                }
			else
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
			}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(r->ren, surface);

	SDL_FreeSurface(surface);

	if (tex == NULL)
		{
		SDL_DestroyRenderer(r->ren);
		SDL_DestroyWindow(r->win);
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
		}

	//First clear the renderer
	SDL_RenderClear(r->ren);
	//Draw the texture
	SDL_RenderCopy(r->ren, tex, NULL, NULL);
	//Update the screen
	SDL_RenderPresent(r->ren);

	SDL_DestroyTexture(tex);

    return out;
	}
