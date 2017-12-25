#include <stdint.h>
#include <iostream>
#include <assert.h>

//#define NDEBUG

class chip8
{
	uint16_t opcode;
	uint8_t memory[4096];
	uint8_t v[16]; // V flags
	uint16_t I; // Index Register	
	uint16_t pc; // Program Counter
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint16_t stack[16];
	uint8_t sp; // Stack Pointer
	uint8_t key[16];
	uint8_t screen[64 * 32];

	void initialize();
	void emulateCycle();	
	void fetchOpcode();
	void decodeOpcode();
};

