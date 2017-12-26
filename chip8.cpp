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
			switch(opcode & 0x000F)
			{
				case 0x0000:
					for(int i = 0; i < 64 * 32; ++i)
						screen[i] = 0;
					pc += 2;
				break;
				case 0x000E:
					assert(sp > 0);
					--sp;
					pc = stack[sp];
					pc += 2;	
				break;
				default:
					printf("Unknown opcode 0x%X\n", opcode);
			}
		break;

		case 0x1000:
			pc = opcode & 0x0FFF;
		break;

		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
		break;

		case 0x3000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			assert(X >= 0 && X < 16);
	   		pc = (v[X] == opcode & 0x00FF) ? pc + 4 : pc + 2;	   
		break;

		case 0x4000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			assert(X >= 0 && X < 16);
	   		pc = (v[X] != opcode & 0x00FF) ? pc + 4 : pc + 2;	   
		break;

		case 0x5000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			uint8_t Y = (opcode & 0x00F0) >> 4;
			assert(X >= 0 && X < 16);
			assert(Y >= 0 && Y < 16);
	   		pc = (v[X] == v[Y]) ? pc + 4 : pc + 2;	   
		break;

		case 0x6000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			assert(X >= 0 && X < 16);
			v[X] = opcode & 0x00FF;
			pc += 2;
		break;

		case 0x7000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			assert(X >= 0 && X < 16);
			v[X] += opcode & 0x00FF;
			pc += 2;
		break;
		
		case 0x8000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			uint8_t Y = (opcode & 0x00F0) >> 4;
			assert(X >= 0 && X < 16);
			assert(Y >= 0 && Y < 16);
			switch(opcode & 0x000F)
			{
				case 0x0000:
					v[X] = v[Y];
				break;

				case 0x0001:
					v[X] = v[Y] | v[X];
				break;

				case 0x0002:
					v[X] = v[Y] & v[X];
				break;

				case 0x0003:
					v[X] = v[Y] ^ v[X];
				break;

				case 0x0004:
					v[0xF] = (v[X] <= 0xFF - v[Y]) ? 0 : 1;
					v[X] += v[Y];
				break;

				case 0x0005:
					v[0xF] = (v[X] >= v[Y]) ? 1 : 0;
					v[X] -= v[Y];
				break;

				case 0x0006:
					v[0xF] = v[Y] & 0x0001; // least significant bit
					v[X] = v[Y] >> 1;
				break;

				case 0x0007:
					v[0xF] = (v[Y] >= v[X]) ? 1 : 0;
					v[X] = v[Y] - v[X];
				break;

				case 0x000E:
					v[0xF] = v[Y] & 0x0080; // most significant bit
					v[X] = v[Y] << 1;
				break;

				default:
					printf("Unknown opcode 0x%X\n", opcode);
			}
			pc += 2;
		break;

		case 0x9000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			uint8_t Y = (opcode & 0x00F0) >> 4;
			assert(X >= 0 && X < 16);
			assert(Y >= 0 && Y < 16);
	   		pc = (v[X] != v[Y]) ? pc + 4 : pc + 2;	   
		break;

		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0xB000:
			pc = v[0] + (opcode & 0x0FFF);
		break;

		case 0xC000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			assert(X >= 0 && X < 16);
			v[X] = (rand() % 256) & (opcode & 0x00FF);	
			pc += 2;
		break;

		case 0xD000:
			uint8_t X = (opcode & 0x0F00) >> 8;
			uint8_t Y = (opcode & 0x00F0) >> 4;
			assert(X >= 0 && X < 16);
			assert(Y >= 0 && Y < 16);
			uint16_t coord = v[X] + 64 * v[Y]; 
			for(int i = 0; i < (opcode & 0x000F); ++i)
			{
				uint8_t row = memory[I + 8*i];
            	for(int j = 0; j < 8; ++j)
				{
					uint8_t current = screen[coord + j + 64*i];
					uint8_t next = (row >> j) & 0x1;
					v[0xF] = (current == 1 && next == 0) ? 1 : v[0xF];  
					screen[coord + j + 64*i] = next;
				}
			pc += 2;
		break;

		case 0xE000:
			switch(opcode & 0x00F0)
			{
				case 0x0090:
					uint8_t X = (opcode & 0x0F00) >> 8;
					assert(X >= 0 && X < 16);
					pc = (key[v[X]] != 0) ? pc + 4 : pc + 2;
				break;

				case 0x00A0:
					uint8_t X = (opcode & 0x0F00) >> 8;
					assert(X >= 0 && X < 16);
					pc = (key[v[X]] == 0) ? pc + 4 : pc + 2;
				break;

				default:
					printf("Unknown opcode 0x%X\n", opcode);
			}
		break;

		case 0xF000:
			switch(opcode & 0x00FF)
			{
				case 0x0007:
				break;

				case 0x000A:
				break;

				case 0x0015:
				break;

				case 0x0018:
				break;

				case 0x001E:
				break;

				case 0x0029:
				break;

				case 0x0033:
				break;

				case 0x0055:
				break;

				case 0x0065:
				break;
				
				default:
					printf("Unknown opcode 0x%X\n", opcode);
		break;

		default:
			printf("Unknown opcode 0x%X\n", opcode);
	}
}
