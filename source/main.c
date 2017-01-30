// Main.c: Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>


#if defined(WIN32)
#include <io.h>
#else
#include <sys/io.h>
#endif

#include "main.h"



void Chip8Interpreter()
{
	//credit to wikipedia for opcode descriptions
	switch (opcode & 0xF000) //(bitwise AND operation) trims the last 3 values of the opcode, (eg. case 0x1000 gets called with 0x1225)
	{
		case 0x0000:
			switch (opcode) {
				case 0x00E0:
					//clear screen
					pc += 2;
				break;
				case 0x00EE:
					//return from subroutine
					pc += 2;
			break;
			} 
		break;

		case 0x1000: //1NNN,	Flow, goto NNN, Jumps to address NNN.
			pc = opcode & 0x0FFF; //program counter jumps to remaining 3 digits of the opcode
			printf("jumping to: %#04x \n", opcode & 0x0FFF);
			break;

		case 0x2000: //2NNN, Flow, *(0xNNN)()	Calls subroutine at NNN.

			//call subroutine code here
			pc = opcode & 0x0FFF; //program counter jumps to remaining 3 digits of the opcode
			printf("jumping to: %#04x \n", opcode & 0x0FFF);
			break;

		case 0x3000: //3XNN, Cond, if(Vx==NN)	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		case 0x4000: //4XNN, Cond, if(Vx!=NN)	Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		case 0x5000: //5XY0, Cond, if(Vx==Vy)	Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			}
			else {
				pc += 2;
			}
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
			switch(opcode & 0x000F) {
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
					if (1) { //"if there's a carry"?
						V[0x10] = 1;
					} else {
						V[0x10] = 0;
					}
					pc += 2;
					break;

				case 0x0005: //8XY5, Math, Vx -= Vy	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					pc += 2;
					break;

				case 0x0006: //8XY6, BitOp, Vx >> 1	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					pc += 2;
					break;

				case 0x0007: //8XY7, Math, Vx=Vy-Vx	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					pc += 2;
					break;

				case 0x000E: //8XYE, BitOp, Vx << 1	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					pc += 2;
					break;
			}
			break;

		case 0x9000: //9XY0, Cond, if(Vx!=Vy)	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;

		case 0xA000: //ANNN, MEM, I = NNN	Sets I to the address NNN.
			I = opcode & 0x0FFF;
			pc += 2;
			break;

		case 0xB000: //BNNN, Flow, PC=V0+NNN	Jumps to the address NNN plus V0.
			pc = (opcode & 0x0FFF) + V[0x00];
			break;

		case 0xC000: //CXNN, Rand, Vx=rand()&NN	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			pc += 2;
			break;

		case 0xD000: //DXYN, Disp, draw(Vx,Vy,N)	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
			pc += 2;
			break;

		case 0xE000:
			switch (opcode & 0x000F) {
				case 0x000E: //EX9E, KeyOp, if(key()==Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
					pc += 2;
					break;

				case 0x0001: //EXA1, KeyOp, if(key()!=Vx)	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
					pc += 2;
					break;
			}
			break;

		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0007: //FX07, Timer, Vx = get_delay()	Sets VX to the value of the delay timer.
					pc += 2;
					break;

				case 0x000A: //FX0A, KeyOp, Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
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
					pc += 2;
					break;

				case 0x0065: //FX65, MEM, reg_load(Vx,&I)	Fills V0 to VX (including VX) with values from memory starting at address I
					pc += 2;
					break;
			}
			break;

	}

}

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

		if (pc >= 4096) running = 0; //if program counter reaches end of file for whatever reason (like this emulator is not finished) then stop
	}
	return 0;
}
