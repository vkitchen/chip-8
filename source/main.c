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

#include "interpreter.h"
#include "render.h"

const char *usage = "\
chip-8 [FILE]\n";

int main(int argc, char **argv)
	{
	SDL_Event Events;

	running = 1;

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

	pc = 0x200; //apparently this is where the program counter should start

	//set random number seed
	srand(time(0));

	struct renderer *r = render_new();

	while (running)
		{
		while (SDL_PollEvent(&Events))
			if (Events.type == SDL_QUIT)
				running = 0;


		//Get and decode opcode
		opcode = memory[pc] << 8 | memory[pc + 1];

		//execute opcode
		pc2 = pc;
		interpreter(r);

		cyclesSLF++;
		OpsProcessed++;

		//update screen
		if (cyclesSLF >= cyclesperframe)
			{
				framesSLS++;
				updatetimers(); //these timers should be called at 60hz
				frame_count++;
				cyclesSLF = 0; //reset cycles since last frame
				// printf("\033[7;0HFrame: %d\n", frame_count);
				// printf("\033[8;0HDraw calls per frame: %d\n", draw_calls);
				draw_calls = 0;
				//insert some function for updating screen here

				// //First clear the renderer
				// SDL_RenderClear(r->ren);
				//
				// //Update the screen
				// SDL_RenderPresent(r->ren);

				if (t != (unsigned)time(NULL)) //if 1 second has passed
				{
					if(framesSLS == 60)
						printf("\x1B[32m");
					else
						printf("\x1B[33m");

					printf("\033[7;15H FPS: %d", framesSLS);
					printf("\x1B[0m");
					framesSLS = 0;
				}
				t = (unsigned)time(NULL);
			}
		}

	render_free(r);
	return 0;
	}
