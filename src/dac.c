#include "stm32f4xx.h"
#include "dac.h"


void dac_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_DACEN; // Enable DAC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock
	GPIOA->MODER |= 0x0300; // PA4 in analog mode

	DAC->CR |= DAC_CR_WAVE1_0; // Enable noise
	DAC->CR |= 0x0F00; // amplitude = 4095
	DAC->CR |= 0x0070; // Software trigger, comment for Timer 6 trigger (default)
	DAC->CR |= DAC_CR_TEN1; // DAC trigger enable
	DAC->CR |= DAC_CR_EN1;
}
