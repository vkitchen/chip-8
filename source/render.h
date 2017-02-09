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

struct renderer *render_new();
int render_draw(struct renderer *r, char x, char y, char *sprite, char height);

unsigned int frame_count; //total number of frames rendered
unsigned int draw_calls; //total amount of objects begin rendered during that frame

#endif
