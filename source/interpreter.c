/*
		INTERPRETER.C
		-------------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)

		Assembly taken from Cowgod's Chip-8 Technical Reference (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
*/

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "memory.h"
#include "render.h"

/*
	FRAME_NEW()
	-----------
*/
struct frame *interpreter_frame_new(char *memory)
	{
	struct frame *f = memory_alloc(sizeof(*f));
	f->pc = 0x200;
	f->memory = memory;
	f->V = memory_alloc(sizeof(char) * 16);
	f->stack = memory_alloc(sizeof(short) * 16);
	for (int i = 0; i < 16; i++)
		{
		f->V[i] = 0;
		f->stack[i] = 0;
		}
	f->sp = 0;
	f->I = 0;
	f->renderer = render_new();
	f->keypressed = 0;
	f->key = 0;

	return f;
	}

/*
	INTERPRETER_FRAME_DUP()
	-----------------------
*/
struct frame *interpreter_frame_dup(struct frame *a)
	{
	struct frame *b = memory_alloc(sizeof(*b));
	memcpy(b, a, sizeof(*b));
	b->V = memory_alloc(sizeof(char) * 16);
	memcpy(b->V, a->V, 16);
	b->stack = memory_alloc(sizeof(short) * 16);
	memcpy(b->stack, a->stack, sizeof(short) * 16);
	return b;
	}

/*
	INTERPREFER_FRAME_CMP()
	-----------------------
*/
int interpreter_frame_cmp(struct frame *a, struct frame *b)
	{
	int out = 0;
	out |= a->pc != b->pc;
	out |= a->I != b->I;
	out |= a->sp != b->sp;
	for (int i = 0; i < 16; i++)
		{
		out |= a->V[i] != b->V[i];
		out |= a->stack[i] != b->stack[i];
		}
	return !out;
	}

/*
	INTERPRETER_FRAME_FREE()
	------------------------
*/
void interpreter_frame_free(struct frame *f)
	{
	free(f->stack);
	free(f->V);
	free(f);
	}

/*
	INTERPRETER_OPCODE_SPRINT()
	---------------------------
*/
void interpreter_opcode_sprint(char *buffer, short opcode)
	{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	switch (opcode & 0xF000)
		{
		case 0x0000:
			switch (opcode)
				{
				/* CLS */
				case 0x00E0:
					sprintf(buffer, "CLS");
					break;

				/* RET */
				case 0x00EE:
					sprintf(buffer, "RET");
					break;
				}
			break;

		/* JP addr */
		case 0x1000:
			sprintf(buffer, "JP %X", opcode & 0x0FFF);
			break;

		/* CALL addr */
		case 0x2000:
			sprintf(buffer, "JP %X", opcode & 0x0FFF);
			break;

		/* SE Vx, byte */
		case 0x3000:
			sprintf(buffer, "SE V%X, %X", x, opcode & 0x00FF);
			break;

		/* SNE Vx, byte */
		case 0x4000:
			sprintf(buffer, "SNE V%X, %X", x, opcode & 0x00FF);
			break;

		/* SE Vx, Vy */
		case 0x5000:
			sprintf(buffer, "SE V%X, V%X", x, y);
			break;

		/* LD Vx, byte */
		case 0x6000:
			sprintf(buffer, "LD V%X, %X", x, opcode & 0x00FF);
			break;

		/* ADD Vx, byte */
		case 0x7000:
			sprintf(buffer, "ADD V%X, %X", x, opcode & 0x00FF);
			break;

		case 0x8000:
			switch (opcode & 0x000F)
				{
				/* LD Vx, Vy */
				case 0x0000:
					sprintf(buffer, "LD V%X, V%X", x, y);
					break;

				/* OR Vx, Vy */
				case 0x0001:
					sprintf(buffer, "OR V%X, V%X", x, y);
					break;

				/* AND Vx, Vy */
				case 0x0002:
					sprintf(buffer, "AND V%X, V%X", x, y);
					break;

				/* XOR Vx, Vy */
				case 0x0003:
					sprintf(buffer, "XOR V%X, V%X", x, y);
					break;

				/* ADD Vx, Vy */
				/* VF is set to 1 if there's a carry. 0 otherwise */
				case 0x0004:
					sprintf(buffer, "ADD V%X, V%X", x, y);
					break;

				//TODO: check this
				/* SUB Vx, Vy */
				/* VF is set to 0 when there's a borrow. 1 otherwise */
				case 0x0005:
					sprintf(buffer, "SUB V%X, V%X", x, y);
					break;

				/* SHR Vx {, Vy} */
				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					sprintf(buffer, "SHR V%X, V%X", x, y);
					break;

				/* SUBN Vx, Vy */
				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					sprintf(buffer, "SUBN V%X, V%X", x, y);
					break;

				/* SHL Vx {, Vy} */
				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					sprintf(buffer, "SHL V%X, V%X", x, y);
					break;
				}
				break;

		/* SNE Vx, Vy */
		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			sprintf(buffer, "SNE V%X, V%X", x, y);
			break;

		/* LD I, addr */
		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			sprintf(buffer, "LD I, %X", opcode & 0xFFF);
			break;

		/* JP V0, addr */
		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			sprintf(buffer, "JP V0, %X", opcode & 0xFFF);
			break;

		/* RND Vx, byte */
		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			sprintf(buffer, "RND V%X, %X", x, opcode & 0xFF);
			break;

		/* DRW Vx, Vy, nibble */
		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			// printf("Draw sprite at (%d, %d) Height: %d\n", x, y, opcode & 0x000F);
			sprintf(buffer, "DRW V%X, V%X, %X", x, y, opcode & 0xF);
			break;

		case 0xE000:
			switch (opcode & 0x000F)
				{
				/* SKP Vx */
				case 0x000E:
					sprintf(buffer, "SKP V%X", x);
					break;

				/* SKNP Vx */
				case 0x0001:
					sprintf(buffer, "SKNP V%X", x);
					break;
				}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
				{
				/* LD Vx, DT */
				case 0x0007: //FX07, Timer, Vx = get_delay()	Sets VX to the value of the delay timer.
					sprintf(buffer, "LD V%X, DT", x);
					break;

				/* LD Vx, K */
				case 0x000A: //FX0A, KeyOp, Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					sprintf(buffer, "LD V%X, K", x);
					break;

				/* LD DT, Vx */
				case 0x0015: //FX15, Timer, delay_timer(Vx)	Sets the delay timer to VX.
					sprintf(buffer, "LD DT, V%X", x);
					break;

				/* LD ST, Vx */
				case 0x0018: //FX18, Sound, sound_timer(Vx)	Sets the sound timer to VX.
					sprintf(buffer, "LD ST, V%X", x);
					break;

				/* ADD I, Vx */
				case 0x001E: //FX1E, MEM, I +=Vx	Adds VX to I.
					sprintf(buffer, "ADD I, V%X", x);
					break;

				/* LD F, Vx */
				case 0x0029:
					sprintf(buffer, "LD F, V%X", x);
					break;

				/* LD B, Vx */
				/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
				case 0x0033:
					sprintf(buffer, "LD B, V%X", x);
					break;

				/* LD [I], Vx */
				case 0x0055: //FX55, MEM, reg_dump(Vx,&I)	Stores V0 to VX (including VX) in memory starting at address I.
					sprintf(buffer, "LD [I], V", x);
					break;

				/* LD Vx, [I] */
				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					sprintf(buffer, "LD V%X, [I]", x);
					break;
				}
			break;
		}
	}

/*
	INTERPRETER_FRAME_PRINT()
	-------------------------
*/
void interpreter_frame_print(struct frame *f)
	{
	char buffer[256];

	puts("struct frame");
	puts("    {");
	printf("    pc: %#04x\n", f->pc);
	printf("    opcode: 0x%02x%02x\n", f->memory[f->pc], f->memory[f->pc+1]);
	interpreter_opcode_sprint(&buffer[0], f->memory[f->pc] << 8 | f->memory[f->pc + 1]);
	printf("    asm: %s\n", buffer);
	printf("    V[]    ");
	for (int i = 0; i < 16; i++)
		printf(" %3d", f->V[i]);
	puts("");
	printf("    stack[]");
	for (int i = 0; i < 16; i++)
		printf(" %3d", f->stack[i]);
	puts("");
	printf("    sp: %#04x\n", f->sp);
	printf("    I: %#04x\n", f->I);
	puts("    }");
	}


/*
	INTERPRETER_EXEC()
	------------------
*/
struct frame *interpreter_exec(struct frame *f)
	{
	f = interpreter_frame_dup(f);

	unsigned short opcode = f->memory[f->pc] << 8 | f->memory[f->pc + 1];

	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	f->pc += 2;

	switch (opcode & 0xF000)
		{
		case 0x0000:
			switch (opcode)
				{
				/* CLS */
				case 0x00E0:
					render_clear(f->renderer);
					break;

				/* RET */
				case 0x00EE:
					f->pc = f->stack[--f->sp];
					break;
				}
			break;

		/* JP addr */
		case 0x1000:
			f->pc = opcode & 0x0FFF;
			break;

		/* CALL addr */
		case 0x2000:
			f->stack[f->sp++] = f->pc;
			f->pc = opcode & 0x0FFF;
			break;

		/* SE Vx, byte */
		case 0x3000:
			if (f->V[x] == (opcode & 0x00FF))
				f->pc += 2;
			break;

		/* SNE Vx, byte */
		case 0x4000:
			if (f->V[x] != (opcode & 0x00FF))
				f->pc += 2;
			break;

		/* SE Vx, Vy */
		case 0x5000:
			if (f->V[x] == f->V[y])
				f->pc += 2;
			break;

		/* LD Vx, byte */
		case 0x6000:
			f->V[x] = opcode & 0x00FF;
			break;

		/* ADD Vx, byte */
		case 0x7000:
			f->V[x] += opcode & 0x00FF;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
				{
				/* LD Vx, Vy */
				case 0x0000:
					f->V[x] = f->V[y];
					break;

				/* OR Vx, Vy */
				case 0x0001:
					f->V[x] |= f->V[y];
					break;

				/* AND Vx, Vy */
				case 0x0002:
					f->V[x] &= f->V[y];
					break;

				/* XOR Vx, Vy */
				case 0x0003:
					f->V[x] ^= f->V[y];
					break;

				/* ADD Vx, Vy */
				/* VF is set to 1 if there's a carry. 0 otherwise */
				case 0x0004:
					if ((((int)f->V[x] + (int)f->V[y]) < 255))
						f->V[0xF] = 0;
					else
						f->V[0xF] = 1;

					f->V[x] += f->V[y];
					break;

				//TODO: check this
				/* SUB Vx, Vy */
				/* VF is set to 0 when there's a borrow. 1 otherwise */
				case 0x0005:
					if (f->V[x] > f->V[y]) //if VY <= VX
						f->V[0xF] = 1;
					else
						f->V[0xF] = 0;

					f->V[x] -= f->V[y];
					break;

				/* SHR Vx {, Vy} */
				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					f->V[0xF] = f->V[x] & 0x1; //TODO: check this
					f->V[x] >>= 1;
					break;

				/* SUBN Vx, Vy */
				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if (f->V[x] < f->V[y]) //if VY <= VX
						f->V[0xF] = 1;
					else
						f->V[0xF] = 0;

					f->V[x] = f->V[y] - f->V[x];
					break;

				/* SHL Vx {, Vy} */
				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					f->V[0xF] = f->V[x] >> 7;
					f->V[x] <<= 1;
					break;
				}
				break;

		/* SNE Vx, Vy */
		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			if (f->V[x] != f->V[y])
				f->pc += 2;
			break;

		/* LD I, addr */
		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			f->I = opcode & 0x0FFF;
			//printf("Setting I register to: %#04x", I);
			break;

		/* JP V0, addr */
		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			f->pc = (opcode & 0x0FFF) + f->V[0];
			//printf("Jumping to: %#04x", pc);
			break;

		/* RND Vx, byte */
		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			f->V[x] = rand() & (opcode & 0x00FF);
			break;

		/* DRW Vx, Vy, nibble */
		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			// printf("Draw sprite at (%d, %d) Height: %d\n", x, y, opcode & 0x000F);
			f->V[0xF] = render_draw(f->renderer, f->V[x], f->V[y], &f->memory[f->I], opcode & 0x000F);
			break;

		case 0xE000:
			switch (opcode & 0x000F)
				{
				/* SKP Vx */
				case 0x000E:
					if (f->keypressed && f->key == f->V[x])
						f->pc += 2;
					f->keypressed = 0;
					break;

				/* SKNP Vx */
				case 0x0001:
					if (!(f->keypressed && f->key == f->V[x]))
						f->pc += 2;
					f->keypressed = 0;
					break;
				}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
				{
				/* LD Vx, DT */
				case 0x0007: //FX07, Timer, Vx = get_delay()	Sets VX to the value of the delay timer.
					puts("WARNING: LD Vx, DT not implemented");
					break;

				/* LD Vx, K */
				case 0x000A: //FX0A, KeyOp, Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					if (f->keypressed)
						f->V[x] = f->key;
					else
						f->pc -= 2;
					f->keypressed = 0;
					break;

				/* LD DT, Vx */
				case 0x0015: //FX15, Timer, delay_timer(Vx)	Sets the delay timer to VX.
					puts("WARNING: LD DT, Vx not implemented");
					break;

				/* LD ST, Vx */
				case 0x0018: //FX18, Sound, sound_timer(Vx)	Sets the sound timer to VX.
					puts("WARNING: LD ST, Vx not implemented");
					break;

				/* ADD I, Vx */
				case 0x001E: //FX1E, MEM, I +=Vx	Adds VX to I.
					f->I += f->V[x];
					break;

				/* LD F, Vx */
				case 0x0029:
					f->I = f->fonts[f->V[x]];
					break;

				/* LD B, Vx */
				/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
				case 0x0033:
					puts("WARNING: LD B, Vx not implemented");
					break;

				/* LD [I], Vx */
				case 0x0055: //FX55, MEM, reg_dump(Vx,&I)	Stores V0 to VX (including VX) in memory starting at address I.
					for (int i = 0; i <= x; i++)
						f->memory[f->I + i] = f->V[i];
					break;

				/* LD Vx, [I] */
				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					for (int i = 0; i <= x; i++)
						f->V[i] = f->memory[f->I + i];
					break;
				}
			break;
		}

	return f;
	}
