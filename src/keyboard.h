#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define KB_MASK_OUT	0b1111
#define KB_MASK_IN	0b111
#define KB_LOOP_DELAY	100

static const unsigned char KB_CHARS[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 11};

int kb_init();
int kb_getkeys(uint8_t prev_key, uint8_t *key_out);
int _kb_linescan(uint8_t *key_out);
int _kb_nzeros(uint8_t kb_in);

#endif /* KEYBOARD_H_ */
