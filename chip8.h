#include <stdint.h>

class chip8
{
	uint16_t opcode;
	uint8_t memory[4096];
	uint8_t v[16]; // V flags
	uint16_t i; // Index Register	
	uint16_t pc; // Program Counter
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint16_t stack[16];
	uint8_t sp; // Stack Pointer
	uint8_t key[16];
	bool screen[64 * 32];

	void initialize();
	void emulateCycle();	
	void fetchOpcode();
};

void chip8::fetchOpcode()
{
	opcode = memory[pc] << 8 | memory[pc + 1];
}
