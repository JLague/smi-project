#include <string.h>
#include "stm32f4xx.h"
#include "keyboard.h"


/*
 * Init keyboard GPIO
 */
int kb_init() {
	// Enable GPIO A and C clocks
	RCC->AHB1ENR |= 0b101;

	/* GPIO A */

	// Output mode
	GPIOA->MODER |=  0b01010101;
	GPIOA->MODER &= ~0b10101010;

	// Open-drain, high-speed
	GPIOA->OTYPER  |= 0xF;
	GPIOA->OSPEEDR |= 0xFF;

	// Pull-up
	GPIOA->PUPDR |=  0b01010101;
	GPIOA->PUPDR &= ~0b10101010;

	/* GPIO C */

	GPIOC->MODER = 0; 		// input
	GPIOC->OTYPER = 0;		// push-pull
	GPIOC->OSPEEDR |= 0x3F; // high-speed

	// Pull-up
	GPIOC->PUPDR |=  0b010101;
	GPIOC->PUPDR &= ~0b101010;

	return 0;
}

/*
 * Get key presses
 *
 * prev_keys: previous keys that were pressed
 * prev_state: previous state of the state machine
 * kb_out: pointer to the keys pressed
 * state_out: pointer to the new state of the state machine
 *
 * returns: length of the keys pressed
 */
int kb_getkeys(unsigned char* prev_keys, unsigned int prev_state, unsigned char** keys_out, unsigned int* state_out) {
	unsigned int new_state;
	unsigned int keys_len;

	// Wait for keyboard state to change
	while ((new_state = _kb_linescan(keys_out)) == prev_state);

	// State machine
	switch (prev_state) {
		case -1:
			if (new_state != 0) { // Only change to state 0
				new_state = -1;
			}
			keys_len = new_state;
		case 0:
			// Can go to any state
			keys_len = new_state;
			break;
		case 1:
			if (new_state == 2) {
				// If additional key, only register new key
				// New key is the one that wasn't in previous keys
				(*keys_out)[0] = prev_keys[0] == (*keys_out)[0] ? (*keys_out)[1] : (*keys_out)[0];
				keys_len = 1;
			} else {
				keys_len = new_state;
			}
			break;
		case 2:
			// If new state is 1, the only key will be in the kb_out, so
			// the state machine will still work on the next iteration.
			// Switch to new state but don't register keys
			keys_len = new_state == 1 ? 0 : new_state;
			break;
	}

	*state_out = new_state;
	return keys_len;
}

/*
 * Scan keyboard lines for characters
 */
int _kb_linescan(unsigned char** keys_out) {
	int char_count = 0;
	int counter = 0;
	int char_codes[2];

	// Iterate over keyboard lines
	for (int i = 0; i < 4; ++i) {
		// Write 0 to a line, wait a bit and read input from keyboard
		GPIOA->ODR = KB_MASK_OUT & ~(1 << i);
		for (int j = 0; j < KB_LOOP_DELAY; ++j);
		uint8_t kb_in = GPIOC->IDR & KB_MASK_IN;

		// If key pressed on this line
		if (kb_in != KB_MASK_IN) {
			int nzeros = _kb_nzeros(kb_in);
			char_count += nzeros;

			// If more than 2 buttons pressed
			if (char_count > 2) {
				return -1;
			}

			// Find 0 in line
			int line_pos = 0;
			while (nzeros > 0) {
				if (kb_in % 2 == 0) {
					char_codes[counter++] = i*3 + line_pos;
					--nzeros;
				}
				++line_pos;
				kb_in = (kb_in >> 1);
			}
		}
	}

	// Get chars from codes
	for (int i = 0; i < char_count; ++i) {
		(*keys_out)[i] = KB_CHARS[char_codes[i]];
	}

	return char_count;
}

/*
 * Get number of zeros in keyboard input
 */
int _kb_nzeros(uint8_t kb_in) {
	int nzeros = 0;
	for (int i = 0; i < 3; ++i) {
		if (kb_in % 2 == 0) ++nzeros;
		kb_in = (kb_in >> 1);
	}
	return nzeros;
}

