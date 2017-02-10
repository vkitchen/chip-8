/*
		MAIN.C
		-------------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/
/*!
	@file
	@brief Program entry point
	@author Timothy Kral, Vaughan Kitchen
	@copyright 2017 Timothy Kral, Vaughan Kitchen
*/

//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <time.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <sys/io.h>
#endif

#include "file.h"
#include "fonts.h"
#include "interpreter.h"
#include "keys.h"
#include "memory.h"
#include "render.h"

const char *usage = "\
chip-8 [FILE]\n";

int main(int argc, char **argv)
	{
	SDL_Event Events;
	int running = 1;
	int keypressed = 0;
	int key = 0;
	struct frame *f;
	unsigned char memory[2048];
	for (int i = 0; i < 2048; i++)
		memory[i] = 0;

	f = interpreter_frame_new(&memory[0]);
	f->fonts = memory_alloc(sizeof(short) * 16);

	/* load fonts */
	for (int i = 0; i < 16; i++)
		{
		memcpy(&memory[i*5], fonts[i], 5);
		f->fonts[i] = i*5;
		}

	if (argc != 2)
		{
		printf(usage);
		exit(0);
		}

	if (!file_slurp_buffer_c(argv[1], &memory[0x200], 4096 - 0x200))
		{
		printf("ERROR: File \"%s\" could not be read\n", argv[1]);
		exit(1);
		}

	//set random number seed
	srand(time(0));

	while (running)
		{
		while (SDL_PollEvent(&Events))
			{
			if (Events.type == SDL_QUIT)
				running = 0;
			if (Events.type == SDL_KEYDOWN && ISKEY(Events.key.keysym.sym))
				{
				f->keypressed = 1;
				f->key = keys[Events.key.keysym.sym];
				// printf("key pressed %X\n", f.key);
				}
			}

		//interpreter_print_frame(f);
		struct frame *f_new = interpreter_exec(f);
		interpreter_frame_free(f);
		f = f_new;
		}

	render_free(f->renderer);
	free(f->fonts);
	interpreter_frame_free(f);
	return 0;
	}
