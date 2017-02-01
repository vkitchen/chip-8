/*
		INTERPRETER.C
		-------------
		Copyright (c) 2017 Timothy Kral, Vaughan Kitchen
		Released under the MIT license (https://opensource.org/licenses/MIT)
*/

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

/*
	INTERPRETER()
	-------------
*/
void interpreter()
	{
		
	//credit to wikipedia for opcode descriptions
	switch (opcode & 0xF000) //(bitwise AND operation) trims the last 3 values of the opcode, (eg. case 0x1000 gets called with 0x1225)
		{
		case 0x0000:
			switch (opcode)
				{
				case 0x00E0: //clear screen
					printf("Clearing screen");
					pc += 2;
					break;
				case 0x00EE: //return from subroutine				
					sp--; //go one layer up
					pc = stack[sp]; //go back to the original pc position, this will be the original subroutine call
					pc += 2; //so we skip that
					printf("Finished subroutine, jumping to: %#04x", pc);
					break;
				}
			break;

		case 0x1000: //1NNN,	Flow, goto NNN, Jumps to address NNN.
			pc = opcode & 0x0FFF; //program counter jumps to remaining 3 digits of the opcode
			printf("Jumping to: %#04x", pc);
			break;

		case 0x2000: //2NNN, Flow, *(0xNNN)()	Calls subroutine at NNN.			
			stack[sp] = pc; //saves the position of the pc before jumping to a subroutine
			sp++; //Increment depth of subroutine
			pc = opcode & 0x0FFF; //program counter jumps to remaining 3 digits of the opcode
			printf("Calling subroutine at: %#04x", opcode & 0x0FFF);
			break;

		case 0x3000: //3XNN, Cond, if(Vx==NN)	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		case 0x4000: //4XNN, Cond, if(Vx!=NN)	Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
			break;

		case 0x5000: //5XY0, Cond, if(Vx==Vy)	Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		case 0x6000: //6XNN, Const, Vx = NN	Sets VX to NN.
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;

		case 0x7000: //7XNN, Const, Vx += NN	Adds NN to VX.
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
			break;

		case 0x8000:
			switch (opcode & 0x000F)
				{
				case 0x0000: //8XY0, Assign, Vx=Vy	Sets VX to the value of VY.
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 8];
					pc += 2;
					break;

				case 0x0001: //8XY1, BitOp, Vx=Vx|Vy	Sets VX to VX or VY. (Bitwise OR operation)
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0002: //8XY2, BitOp, Vx=Vx&Vy	Sets VX to VX and VY. (Bitwise AND operation)
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0003: //8XY3, BitOp, Vx=Vx^Vy	Sets VX to VX xor VY.
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0004: //8XY4, Math, Vx += Vy	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					if ((((int)V[(opcode & 0x0F00) >> 8] + (int)V[(opcode & 0x00F0) >> 4]) < 255)) //idk whats happening here had to cheat
						V[0xF] = 0;
					else
						V[0xF] = 1;

					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0005: //8XY5, Math, Vx -= Vy	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if (((int)V[(opcode & 0x0F00) >> 8] < (int)V[(opcode & 0x00F0) >> 4])) //idk should this be < or <=, same with the few below
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					//half done
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
					pc += 2;
					break;

				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if ((int)V[(opcode & 0x0F00) >> 8] < (int)V[(opcode & 0x00F0) >> 4])
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;

				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					//half done
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
					pc += 2;
					break;
				}
				break;

		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
			break;

		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			I = opcode & 0x0FFF;
			printf("Setting I register to: %#04x", I);
			pc += 2;
			break;

		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			pc = (opcode & 0x0FFF) + V[0x00];
			printf("Jumping to: %#04x", pc);
			break;

		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
			pc += 2;
			break;

		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			printf("Draw sprite at (%d, %d) Height: %d", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, opcode & 0x000F);
			pc += 2;
			break;

		case 0xE000:
			switch (opcode & 0x000F)
				{
				case 0x000E: //EX9E, KeyOp, if(key()==Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
					pc += 2;
					break;

				case 0x0001: //EXA1, KeyOp, if(key()!=Vx)	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
					pc += 2;
					break;
				}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
				{
				case 0x0007: //FX07, Timer, Vx = get_delay()	Sets VX to the value of the delay timer.
					pc += 2;
					break;

				case 0x000A: //FX0A, KeyOp, Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					printf("Get user input");
					pc += 2;
					break;

				case 0x0015: //FX15, Timer, delay_timer(Vx)	Sets the delay timer to VX.
					pc += 2;
					break;

				case 0x0018: //FX18, Sound, sound_timer(Vx)	Sets the sound timer to VX.
					pc += 2;
					break;

				case 0x001E: //FX1E, MEM, I +=Vx	Adds VX to I.
					I += (opcode & 0x0F00) >> 8;
					pc += 2;
					break;

				case 0x0029: //FX29, MEM, I=sprite_addr[Vx]	Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
					pc += 2;
					break;

				case 0x0033: //just google it too long
					pc += 2;
					break;

				case 0x0055: //FX55, MEM, reg_dump(Vx,&I)	Stores V0 to VX (including VX) in memory starting at address I.
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
						memory[I + i] = V[i];
					pc += 2;
					break;

				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
						V[i] = memory[I + i];
					pc += 2;
					break;
				}
			break;
		}
	}
