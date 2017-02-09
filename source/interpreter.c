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
UPDATETIMERS
-------------
*/
void updatetimers()
	{
		if (sound_timer > 0) //when greater than 0, the buzzer is on
			sound_timer--;
		if (delay_timer > 0)
			delay_timer--;
}

/*
	INTERPRETER()
	-------------
*/
void interpreter(struct renderer *r)
	{
	char x = (opcode & 0x0F00) >> 8;
	char y = (opcode & 0x00F0) >> 4;

	pc += 2;

	//credit to wikipedia for opcode descriptions
	switch (opcode & 0xF000) //(bitwise AND operation) trims the last 3 values of the opcode, (eg. case 0x1000 gets called with 0x1225)
		{
		case 0x0000:
			switch (opcode & 0x000F)
				{
				/* CLS */
				case 0x0000:
					break;

				/* RET */
				case 0x000E:
					pc = stack[sp--];
					//printf("Finished subroutine, jumping to: %#04x", pc);
					break;
				}
			break;

		/* JP addr */
		case 0x1000:
			pc = opcode & 0x0FFF;
			//printf("Jumping to: %#04x", pc);
			break;

		/* CALL addr */
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;
			//printf("Calling subroutine at: %#04x", opcode & 0x0FFF);
			break;

		/* SE Vx, byte */
		case 0x3000:
			if (V[x] == (opcode & 0x00FF))
				pc += 2;
			break;

		/* SNE Vx, byte */
		case 0x4000:
			if (V[x] != (opcode & 0x00FF))
				pc += 2;
			break;

		/* SE Vx, Vy */
		case 0x5000:
			if (V[x] == V[y])
				pc += 2;
			break;

		/* LD Vx, byte */
		case 0x6000:
			V[x] = opcode & 0x00FF;
			break;

		/* ADD Vx, byte */
		case 0x7000:
			V[x] += opcode & 0x00FF;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
				{
				/* LD Vx, Vy */
				case 0x0000:
					V[x] = V[y];
					break;

				/* OR Vx, Vy */
				case 0x0001:
					V[x] |= V[y];
					break;

				/* AND Vx, Vy */
				case 0x0002:
					V[x] &= V[y];
					break;

				/* XOR Vx, Vy */
				case 0x0003:
					V[x] ^= V[y];
					break;

				/* ADD Vx, Vy */
				case 0x0004: //8XY4, Math, Vx += Vy	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
					if ((((int)V[x] + (int)V[y]) < 255))
						V[0xF] = 0;
					else
						V[0xF] = 1;

					V[x] += V[y];
					break;

				//TODO: check this
				/* SUB Vx, Vy */
				case 0x0005: //8XY5, Math, Vx -= Vy	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if ((V[x] <= V[y])) //if VY <= VX
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[x] -= V[y];
					break;

				/* SHR Vx {, Vy} */
				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					V[0xF] = V[x] & 0x1; //TODO: check this
					V[x] >>= 1;
					break;

				/* SUBN Vx, Vy */
				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if (V[x] <= V[y]) //if VY <= VX
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					break;

				/* SHL Vx {, Vy} */
				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
					break;
				}
				break;

		/* SNE Vx, Vy */
		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			if (V[x] != V[y])
				pc += 2;
			break;

		/* LD I, addr */
		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			I = opcode & 0x0FFF;
			//printf("Setting I register to: %#04x", I);
			break;

		/* JP V0, addr */
		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			pc = (opcode & 0x0FFF) + V[0];
			//printf("Jumping to: %#04x", pc);
			break;

		/* RND Vx, byte */
		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
			break;

		/* DRW Vx, Vy, nibble */
		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			// printf("Draw sprite at (%d, %d) Height: %d\n", x, y, opcode & 0x000F);
			V[0xF] = render_draw(r, V[x], V[y], &memory[I], opcode & 0x000F);
			break;

		case 0xE000:
			switch (opcode & 0x000F)
				{
				/* SKP Vx */
				case 0x000E: //EX9E, KeyOp, if(key()==Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
					break;

				/* SKNP Vx */
				case 0x0001: //EXA1, KeyOp, if(key()!=Vx)	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
					break;
				}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
				{
				/* LD Vx, DT */
				case 0x0007: //FX07, Timer, Vx = get_delay()	Sets VX to the value of the delay timer.
					V[x] = delay_timer;
					break;

				/* LD Vx, K */
				case 0x000A: //FX0A, KeyOp, Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					break;

				/* LD DT, Vx */
				case 0x0015: //FX15, Timer, delay_timer(Vx)	Sets the delay timer to VX.
					delay_timer = V[x];
					break;

				/* LD ST, Vx */
				case 0x0018: //FX18, Sound, sound_timer(Vx)	Sets the sound timer to VX.
					sound_timer = V[x];
					break;

				/* ADD I, Vx */
				case 0x001E: //FX1E, MEM, I +=Vx	Adds VX to I.
					I += V[x];
					break;

				/* LD F, Vx */
				case 0x0029: //FX29, MEM, I=sprite_addr[Vx]	Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
					//TODO
					break;

				/* LD B, Vx */
				case 0x0033: //just google it too long
				//TODO
					break;

				/* LD [I], Vx */
				case 0x0055: //FX55, MEM, reg_dump(Vx,&I)	Stores V0 to VX (including VX) in memory starting at address I.
					for (int i = 0; i <= x; i++)
						memory[I + i] = V[i];
					break;

				/* LD Vx, [I] */
				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					for (int i = 0; i <= x; i++)
						V[i] = memory[I + i];
					break;
				}
			break;
		}
}
