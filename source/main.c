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
#include "interpreter.h"
#include "render.h"

const char *usage = "\
chip-8 [FILE]\n";

int main(int argc, char **argv)
	{
	SDL_Event Events;
	int running = 1;
	struct frame f;
	unsigned char memory[2048];

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

	/* Initialise the first frame */
	f.pc = 0x200;
	f.memory = memory;
	for (int i = 0; i < 16; i++)
		{
		f.V[i] = 0;
		f.stack[i] = 0;
		}
	f.sp = 0;
	f.I = 0;
	f.renderer = render_new();

	//set random number seed
	srand(time(0));

	while (running)
		{
		while (SDL_PollEvent(&Events))
			if (Events.type == SDL_QUIT)
				running = 0;

		f = interpreter_exec(f);
		}

	render_free(f.renderer);
	return 0;
	}
