#pragma once

const char *usage = "\chip-8 [FILE]\n";
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