/*
		INTERPRETER.H
		-------------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/
/*!
	@file
	@brief Chip-8 interpreter
	@author Timothy Kral, Vaughan Kitchen
	@copyright 2017 Timothy Kral, Vaughan Kitchen
*/

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

unsigned short opcode; //currently processed opcode
unsigned char memory[4096]; //wam size
unsigned short pc; //program counter
unsigned short pc2; //previous program counter, used to check if the program begins looping
unsigned int OpsProcessed; //counts how many opcodes have been processed
unsigned char V[0x10]; //All 16 registers, V0 to VF, 8 bit
unsigned short stack[0x10]; //16 stack, used for subroutines, capable of 16 nested subroutines
unsigned short sp; //stack pointer, or how many layers deep in a subroutine

//both timers decrement by 1 at 60hz when greater than 0
unsigned char delay_timer; 
unsigned char sound_timer;

unsigned short I; //Adress register, 16 bit

/*
	INTERPRETER()
	-------------
*/
void interpreter();

#endif
