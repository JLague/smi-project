#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define KB_MASK_OUT	0b1111
#define KB_MASK_IN	0b111
#define KB_LOOP_DELAY	1000

static const unsigned char KB_CHARS[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};

int kb_init();
int kb_getkeys(unsigned char* prev_keys, unsigned int prev_state, unsigned char** keys_out, unsigned int* state_out);
int _kb_linescan(unsigned char** keys_out);
int _kb_nzeros(uint8_t kb_in);

#endif /* KEYBOARD_H_ */
