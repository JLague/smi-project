#include "stm32f4xx.h"

#include "keyboard.h"
#include "uart.h"
#include "dac.h"
#include "timer.h"
#include "DMA.h"
#include "wave.h"
#include "cmd.h"

extern volatile uint8_t buffer[20];
extern volatile uint8_t headIndex;
extern uint8_t tailIndex;

extern uint16_t waveSelectValue[2];

int main(void)
{
	// Init
	uart_init();
	kb_init();
	initWaves();
	initTimer6();
	initDMA();
	dac_init();

	int keys_len = 0;
	uint8_t prev_key;
	uint8_t key;

	uint8_t cmdIndex = 0;
	uint8_t cmd[4];

	configSrcAddrDMA((uint32_t)waveSelectValue);

	TIM6->CR1 |= TIM_CR1_CEN;

	while (1) {
		// Get key presses
		keys_len = kb_getkeys(prev_key, &key);
		if (keys_len > 0) {
			changeWave(key);
		}
		prev_key = key;

		// Get command
		// Fill command if we received data
		while(tailIndex != headIndex && cmdIndex < CMD_LEN) {
			// Dequeue
			cmd[cmdIndex++] = buffer[tailIndex++];
			if(tailIndex >= 20) tailIndex = 0;
		}

		// Command array is filled, execute command
		if(cmdIndex >= CMD_LEN) {
			cmdIndex = 0;
			cmd_exec(cmd);
		}
	}
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
	/* TODO, implement your code here */
	return -1;
}
