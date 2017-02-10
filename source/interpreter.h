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

#include "render.h"

/* TODO explicitely copy the arrays inside */
struct frame
	{
	unsigned short pc;			//!< Program Counter
	unsigned char *memory;
	unsigned char V[16];		//!< Registers, V0 to VF
	unsigned char stack[16];
	unsigned short sp;			//!< Stack pointer
	unsigned short I;			//!< Address register
	unsigned short fonts[16];	//!< Array of 0-F font sprites
	struct renderer *renderer;	//!< Handles the screen drawing
	};

/*
	INTERPRETER_PRINT_FRAME()
	-------------------------
*/
void interpreter_print_frame(struct frame f);

/*
	INTERPRETER_EXEC()
	------------------
*/
struct frame interpreter_exec(struct frame f);

#endif
