// Main.c: Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <sys/io.h>
#endif

unsigned short opcode; //currently processed opcode
unsigned char memory[4096]; //wam size
unsigned short pc; //program counter
unsigned short pc2; //previous program counter, used to check if the program begins looping
unsigned int OpsProcessed; //counts how many opcodes have been processed
unsigned char V[0x10]; //All 16 registers, V0 to VF, 8 bit
unsigned short I; //Adress register, 16 bit
int running = 1;
FILE *rom;
int file_size;

#include "interpreter.c"

const char *usage = "\
chip-8 [FILE]\n";

int main(int argc, char **argv)
{
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

	//dump the rom data into the memory array, then close the file
	fread(memory + 0x200, 1, 4096 - 0x200, rom);
	fclose(rom);

	while (running) {
		//Get and decode opcode
		opcode = memory[pc] << 8 | memory[pc + 1];
		printf("Executing %#04x at %04X \n", opcode, pc);

		//execute opcode
		pc2 = pc;
		Chip8Interpreter();
		OpsProcessed++;

		if (pc == pc2) {
			printf("PC Frozen, Number of Opcodes processed: %d\n", OpsProcessed);
			running = 0;
		}
	}
	return 0;
}
