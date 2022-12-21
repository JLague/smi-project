#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stdbool.h"

void initTimer6(void) {
	RCC->APB1ENR |= (0b1 << 4); // timer6 clk enable
	// CLK = 168MHz/2 = 84 MHz; 84MHz/(2099 + 1) = 40 kHz
	TIM6->PSC = 2099;
	// Timer count to 5000
	TIM6->ARR = 10;
	TIM6->CNT = 0;
	TIM6->CCR2 |= (0b010 << 4); // Update event use as a trigger output
	TIM6->CR1 |= 0x0001; // Counter enable
	TIM6->DIER |= 0x0101; // interrupt and DMA request enable

	TIM6->CR2 |= TIM_CR2_MMS_1;

	NVIC->ISER[1] |= (0x00400000);
}

// Fait flasher la led 4, juste pour tester le timer
void TIM6_DAC_IRQHandler(void) {
	static bool ledInit = false;
	if(!ledInit) {
		ledInit = true;
		RCC->AHB1ENR |= 0b1001;
		GPIOD->MODER |= (0b01 << 24); // LED4 PD12 -> output
		GPIOD->OTYPER &= ~(0b0 << 12); // LED4 PD12 -> push-pull
		GPIOD->OSPEEDR |= (0b11 << 24); // LED4 PD12 -> low speed
		GPIOD->PUPDR &= ~(0b00 << 24); // LED4 PD12 -> no pull-up, pull-down
		GPIOD->ODR &= ~(0b1 << 12); // LED4 PD12 -> reset
	}
    TIM6->SR &= ~0x0001; // Clear interrupt flag
    static int i = 0;
    i++;
    if(i >= 4000) {
    	i = 0;
    	GPIOD->ODR ^= (0b1 << 12);  // LED 4
    }
}
