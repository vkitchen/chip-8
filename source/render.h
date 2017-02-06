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
	char screen[2048]; //!< We actually only need one bit per pixel but chars are easier to deal with.
	};

struct renderer *render_new();
void render_draw(struct renderer *r, char x, char y, char *sprite, char length);

#endif
