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
#include "render.h"

/*
	INTERPRETER()
	-------------
*/
struct frame interpreter_exec(struct frame f)
	{
	unsigned short opcode = f.memory[f.pc] << 8 | f.memory[f.pc + 1];

	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	f.pc += 2;

	switch (opcode & 0xF000)
		{
		case 0x0000:
			switch (opcode)
				{
				/* CLS */
				case 0x00E0:
					render_clear(f.renderer);
					break;

				/* RET */
				case 0x00EE:
					f.pc = f.stack[f.sp--];
					break;
				}
			break;

		/* JP addr */
		case 0x1000:
			f.pc = opcode & 0x0FFF;
			break;

		/* CALL addr */
		case 0x2000:
			f.stack[f.sp] = f.pc;
			f.sp++;
			f.pc = opcode & 0x0FFF;
			break;

		/* SE Vx, byte */
		case 0x3000:
			if (f.V[x] == (opcode & 0x00FF))
				f.pc += 2;
			break;

		/* SNE Vx, byte */
		case 0x4000:
			if (f.V[x] != (opcode & 0x00FF))
				f.pc += 2;
			break;

		/* SE Vx, Vy */
		case 0x5000:
			if (f.V[x] == f.V[y])
				f.pc += 2;
			break;

		/* LD Vx, byte */
		case 0x6000:
			f.V[x] = opcode & 0x00FF;
			break;

		/* ADD Vx, byte */
		case 0x7000:
			f.V[x] += opcode & 0x00FF;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
				{
				/* LD Vx, Vy */
				case 0x0000:
					f.V[x] = f.V[y];
					break;

				/* OR Vx, Vy */
				case 0x0001:
					f.V[x] |= f.V[y];
					break;

				/* AND Vx, Vy */
				case 0x0002:
					f.V[x] &= f.V[y];
					break;

				/* XOR Vx, Vy */
				case 0x0003:
					f.V[x] ^= f.V[y];
					break;

				/* ADD Vx, Vy */
				/* VF is set to 1 if there's a carry. 0 otherwise */
				case 0x0004:
					if ((((int)f.V[x] + (int)f.V[y]) < 255))
						f.V[0xF] = 0;
					else
						f.V[0xF] = 1;

					f.V[x] += f.V[y];
					break;

				//TODO: check this
				/* SUB Vx, Vy */
				/* VF is set to 0 when there's a borrow. 1 otherwise */
				case 0x0005:
					if ((f.V[x] <= f.V[y])) //if VY <= VX
						f.V[0xF] = 1;
					else
						f.V[0xF] = 0;

					f.V[x] -= f.V[y];
					break;

				/* SHR Vx {, Vy} */
				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					f.V[0xF] = f.V[x] & 0x1; //TODO: check this
					f.V[x] >>= 1;
					break;

				/* SUBN Vx, Vy */
				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if (f.V[x] <= f.V[y]) //if VY <= VX
						f.V[0xF] = 1;
					else
						f.V[0xF] = 0;

					f.V[(opcode & 0x0F00) >> 8] = f.V[(opcode & 0x00F0) >> 4] - f.V[(opcode & 0x0F00) >> 8];
					break;

				/* SHL Vx {, Vy} */
				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					f.V[0xF] = f.V[(opcode & 0x0F00) >> 8] >> 7;
					f.V[(opcode & 0x0F00) >> 8] = f.V[(opcode & 0x0F00) >> 8] << 1;
					break;
				}
				break;

		/* SNE Vx, Vy */
		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			if (f.V[x] != f.V[y])
				f.pc += 2;
			break;

		/* LD I, addr */
		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			f.I = opcode & 0x0FFF;
			//printf("Setting I register to: %#04x", I);
			break;

		/* JP V0, addr */
		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			f.pc = (opcode & 0x0FFF) + f.V[0];
			//printf("Jumping to: %#04x", pc);
			break;

		/* RND Vx, byte */
		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			f.V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
			break;

		/* DRW Vx, Vy, nibble */
		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			// printf("Draw sprite at (%d, %d) Height: %d\n", x, y, opcode & 0x000F);
			f.V[0xF] = render_draw(f.renderer, f.V[x], f.V[y], &f.memory[f.I], opcode & 0x000F);
			break;

		case 0xE000:
			switch (opcode & 0x000F)
				{
				/* SKP Vx */
				case 0x000E: //EX9E, KeyOp, if(key()==Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
					puts("WARNING: SKP Vx not implemented");
					break;

				/* SKNP Vx */
				case 0x0001: //EXA1, KeyOp, if(key()!=Vx)	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
					puts("WARNING: SKNP Vx not implemented");
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
					puts("WARNING: LD Vx, K not implemented");
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
					f.I += f.V[x];
					break;

				/* LD F, Vx */
				case 0x0029:
					puts("WARNING: LD F, Vx not implemented");
					break;

				/* LD B, Vx */
				/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
				case 0x0033:
					puts("WARNING: LD B, Vx not implemented");
					break;

				/* LD [I], Vx */
				case 0x0055: //FX55, MEM, reg_dump(Vx,&I)	Stores V0 to VX (including VX) in memory starting at address I.
					for (int i = 0; i <= x; i++)
						f.memory[f.I + i] = f.V[i];
					break;

				/* LD Vx, [I] */
				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					for (int i = 0; i <= x; i++)
						f.V[i] = f.memory[f.I + i];
					break;
				}
			break;
		}

	return f;
	}
