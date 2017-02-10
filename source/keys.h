/*
		KEYS.H
		------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)

		Font information courtesy of Cowgod (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4)
*/
/*!
	@file
	@brief Chip-8 keyboard lookup
	@author Timothy Kral, Vaughan Kitchen
	@copyright 2017 Timothy Kral, Vaughan Kitchen
*/

#ifndef KEYS_H_
#define KEYS_H_

#include <SDL2/SDL.h>

/*
	Keyboard is as so

	1 2 3 C
	4 5 6 D
	7 8 9 E
	A 0 B F
*/

#define ISKEY(code) (code == SDLK_q || code == SDLK_w || code == SDLK_e || code == SDLK_r \
	|| code == SDLK_a || code == SDLK_s || code == SDLK_d || code == SDLK_f || code == SDLK_z \
	|| code == SDLK_x || code == SDLK_c || code == SDLK_v )

const int keys[] =
	{
	[SDLK_q] = 0x1,
	[SDLK_w] = 0x2,
	[SDLK_e] = 0x3,
	[SDLK_r] = 0xC,
	[SDLK_a] = 0x4,
	[SDLK_s] = 0x5,
	[SDLK_d] = 0x6,
	[SDLK_f] = 0xD,
	[SDLK_z] = 0xA,
	[SDLK_x] = 0x0,
	[SDLK_c] = 0xB,
	[SDLK_v] = 0xF
	};

#endif
