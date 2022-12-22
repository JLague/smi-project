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

int kb_getkeys(uint8_t prev_key, uint8_t *key_out) {
	int key_len = _kb_linescan(key_out);
	if (key_len > 1 || prev_key == (*key_out)) return 0;
	return key_len;
}

/*
 * Scan keyboard lines for characters
 */
int _kb_linescan(uint8_t *key_out) {
	int char_count = 0;
	int char_code;

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

			// If more than 1 buttons pressed
			if (char_count > 1) {
				return -1;
			}

			// Find 0 in line
			int line_pos = 0;
			while (nzeros > 0) {
				if (kb_in % 2 == 0) {
					char_code = i*3 + line_pos;
					--nzeros;
				}
				++line_pos;
				kb_in = (kb_in >> 1);
			}
		}
	}

	uint8_t temp = -1;
	if (char_count > 0)
		 temp = KB_CHARS[char_code];

	(*key_out) = temp;
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

