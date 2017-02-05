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

#ifdef WIN32
	#include <io.h>
#else
	#include <sys/io.h>
#endif

#include "interpreter.h"

const char *usage = "\
chip-8 [FILE]\n";

void errordump() {
	printf("\nPC: %#04x ", pc);
	printf("\nOP: %04X", opcode);
	printf("\nSP: %d", sp);
	printf("\n\nDelay timer: %ds", delay_timer);
	printf("\nOpcodes processed: %d", OpsProcessed);
}

void errorcheck() {
	if (pc == pc2)
	{
		printf("\n\nERROR: Pointer counter frozen\n");
		errordump();
		running = 0;
	}

	if (sp > 16) {
		printf("\n\nERROR: Stack pointer overflow\n");
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

	//print name of the rom (useless)
	printf("File: %s\n", argv[1]);

	pc = 0x200; //apparently this is where the program counter should start

	//set random number seed
	srand(time(0));

	//dump the rom data into the memory array, then close the file
	fread(memory + 0x200, 1, 4096 - 0x200, rom);
	fclose(rom);


	while (running)
		{
		//Get and decode opcode
		opcode = memory[pc] << 8 | memory[pc + 1];
		printf("\n || PC: %#04x OP: %04X SP: %d || ", pc, opcode, sp);


		//execute opcode
		pc2 = pc;
		interpreter();

		cyclesSLF++;
		OpsProcessed++;
		errorcheck();

		//update screen
		if (cyclesSLF >= cyclesperframe)
			{
				updatetimers(); //these timers should be called at 60hz
				cyclesSLF = 0; //reset cycles since last frame
				//insert some function for updating screen here
			}
		}
	return 0;
	}
