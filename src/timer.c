#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stdbool.h"

void initTimer6(void) {
	RCC->APB1ENR |= (0b1 << 4); // timer6 clk enable
	// CLK = 168MHz/2 = 84 MHz; 84MHz/(2099 + 1) = 40 kHz
	TIM6->PSC = 2099;
	// Timer count to 5000
	TIM6->ARR = 1;
	TIM6->CNT = 0;
	TIM6->CCR2 |= (0b010 << 4); // Update event use as a trigger output
	TIM6->CR1 |= 0x0001; // Counter enable
//	TIM6->DIER |= 0x0101; // interrupt and DMA request enable

	TIM6->CR2 |= TIM_CR2_MMS_1;
}
