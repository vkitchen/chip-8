#pragma once

const char *usage = "\chip-8 [FILE]\n";
unsigned short opcode; //currently processed opcode
unsigned char memory[4096]; //wam size
unsigned short pc; //program counter
int running = 1;
FILE *rom;
int file_size;
