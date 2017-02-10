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
	unsigned char *V;			//!< Registers, V0 to VF
	unsigned char *stack;		//!< Stack, 16 levels
	unsigned short sp;			//!< Stack pointer
	unsigned short I;			//!< Address register
	unsigned short *fonts;		//!< Array of 0-F font sprites
	struct renderer *renderer;	//!< Handles the screen drawing
	int keypressed;				//!< Boolean indicating whether a key has been pressed
	unsigned char key;			//!< The key value according to the keyboard layout
	};

/*
	INTERPRETER_FRAME_NEW()
	-----------------------
*/
struct frame *interpreter_frame_new(char *memory);

/*
	INTERPRETER_FRAME_DUP()
	-----------------------
*/
struct frame *interpreter_frame_dup(struct frame *a);

/*
	INTERPRETER_FRAME_FREE()
	------------------------
*/
void interpreter_frame_free(struct frame *f);

/*
	INTERPRETER_FRAME_PRINT()
	-------------------------
*/
void interpreter_frame_print(struct frame *f);

/*
	INTERPRETER_EXEC()
	------------------
*/
struct frame *interpreter_exec(struct frame *f);

#endif
