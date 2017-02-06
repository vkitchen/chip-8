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
#include "render.h"

#ifdef WIN32
	#include <io.h>
#else
	#include <sys/io.h>
#endif

#include "interpreter.h"

const char *usage = "\
chip-8 [FILE]\n";

void errordump() {
	printf("\n\033[11;0HPC: %#04x OP: %04X SP: %d DT: %d", pc, opcode, sp, delay_timer);
	printf("\n\nOpcodes processed: %d\n", OpsProcessed);

}

void errorcheck() {
	if (pc == pc2)
	{
		printf("\033[10;0HERROR: Pointer counter frozen\n");
		errordump();
		running = 0;
	}

	if (sp > 0xF) {
		printf("\033[10;0HERROR: Stack pointer overflow\n");
		errordump();
		running = 0;
	}
}

int main(int argc, char **argv)
	{
	FILE *rom;
	int file_size;
	running = 1;

	if (argc != 2)
		{
		printf(usage);
		exit(0);
		}

	//open the rom we're working with
	rom = fopen(argv[1], "rb");

	//get the size of the rom
	file_size = ftell(rom);

	//clear the screen
	printf("\033[2J\033[1;1H"); //TODO: only works on linux

	//print name of the rom (useless)
	printf("\nFile: %s\n", argv[1]);

	pc = 0x200; //apparently this is where the program counter should start

	//set random number seed
	srand(time(0));

	//dump the rom data into the memory array, then close the file
	fread(memory + 0x200, 1, 4096 - 0x200, rom);
	fclose(rom);

	printf("\nCPU clock running at: %d Khz", (cyclesperframe * 60) / 1024);
	printf("\033[7;0HFrame: 0");

	struct renderer *r = render_new();

	while (running)
		{
		//Get and decode opcode
		opcode = memory[pc] << 8 | memory[pc + 1];

		//execute opcode
		pc2 = pc;
		interpreter(r);

		cyclesSLF++;
		OpsProcessed++;
		errorcheck();

		//update screen
		if (cyclesSLF >= cyclesperframe)
			{

				updatetimers(); //these timers should be called at 60hz
				frame_count++;
				cyclesSLF = 0; //reset cycles since last frame
				printf("\033[7;0HFrame: %d", frame_count);
				printf("\033[8;0HDraw calls per frame: %d", draw_calls);
				draw_calls = 0;
				//insert some function for updating screen here

				//First clear the renderer
				SDL_RenderClear(r->ren);

				//Update the screen
				SDL_RenderPresent(r->ren);
			}
		}

	render_free(r);

	sleep(2); /* TODO remove, should programs stay open when execution stops? */
	return 0;
	}
