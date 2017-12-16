#include "chip8.h"

void chip8::initialize()
{
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
	
	for(int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

}

void chip8::fetchOpcode()
{
	opcode = memory[pc] << 8 | memory[pc + 1];
}

void chip8::decodeOpcode()
{
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch
}
