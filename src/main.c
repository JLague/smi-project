#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "keyboard.h"
#include "uart.h"
#include "dac.h"
#include "timer.h"
#include "DMA.h"

//extern volatile uint8_t buffer[];
//extern volatile uint8_t headIndex;
//extern uint8_t tailIndex;

const uint16_t square[2] = {0, 4095};

int main(void)
{
	// Init
//	uart_init();
//	kb_init();
	initTimer6();
	initDMA();
	dac_init();
	configSrcAddrDMA((uint32_t)square);

	TIM6->CR1 |= TIM_CR1_CEN;

	while (1) {
	}

//	uint8_t commandIndex = 0;
//	uint8_t command[3];
//
//	/* Infinite loop */
//	while (1) {
//		// Fill command if we received data
//		while(tailIndex != headIndex && commandIndex < 3) {
//			// Dequeue
//			command[commandIndex++] = buffer[tailIndex++];
//			if(tailIndex >= 20) tailIndex = 0;
//		}
//
//		// Command array is filled, execute command
//		if(commandIndex >= 3) {
//			commandIndex = 0;
//			cmd_exec(command[0], command[1], command[2]);
//		}
//	}
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
