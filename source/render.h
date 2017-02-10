/*
		RENDER.H
		--------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/
/*!
	@file
	@brief Render functions for CHIP-8 pixel art
	@author Timothy Kral, Vaughan Kitchen
	@copyright 2017 Timothy Kral, Vaughan Kitchen
*/

#ifndef RENDER_H_
#define RENDER_H_

#include <SDL2/SDL.h>

struct renderer
	{
	SDL_Window *win;
	SDL_Renderer *ren;
	char screen[2048]; //!< The screen represented in row order with one byte per pixel
	};

/*
	RENDER_NEW()
	------------
*/
struct renderer *render_new();

/*
	RENDER_CLEAR()
	--------------
*/
void render_clear(struct renderer *r);

/*
	RENDER_DRAW()
	-------------
*/
int render_draw(struct renderer *r, int x, int y, char *sprite, char height);

/*
	RENDER_FREE()
	-------------
*/
void render_free(struct renderer *r);

#endif
