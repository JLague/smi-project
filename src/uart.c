#include "stm32f4xx.h"
#include "uart.h"

volatile unsigned char buffer[20];
unsigned char headIndex = 0;
volatile unsigned char tailIndex = 0;

void uart_init()
{
	// GPIO
	RCC->AHB1ENR |= 0x00000002; // Enable GPIO B clock
	GPIOB->MODER |=  0x00008000; // Alternate function mode on PB7
	GPIOB->MODER &= ~0x00004000;
	GPIOB->OTYPER &= ~0x0080; // Push-pull
	GPIOB->OSPEEDR |= 0x0000C000; // Very high speed
	GPIOB->PUPDR |=  0x00004000; // Pull-up
	GPIOB->PUPDR &= ~0x00008000;
	GPIOB->AFR[0] |= 0x70000000; // AF7 (USART1) on PB7
	GPIOB->AFR[0] &= ~0x80000000;

	// UART
	RCC->APB2ENR |= 0x00000010; // Enable USART1 clock
	NVIC->ISER[1] |= 0x0020; // Enable USART1 global interrupt
	USART1->BRR = 0x0578; /* USARTDIV = SystemCoreClock / (2 * 19200 * 16) = 87.5
							USARTDIV = DIV_Mantissa + DIV_Fraction/16
							DIV_Mantissa = 87 (decimal) = 0x057
							DIV_Fraction = 8 (decimal) = 0x8 */
	USART1->CR1 |= 0x2424; /* Enable USART, parity control enabled, 8-bit words
							  RXNE interrupt enable, receiver enable */
	USART1->CR1 &= ~0xDBDB; // Even parity, transmitter disabled
	USART1->CR2 = 0x0000; // 1 stop bit, clk disabled
}

void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_RXNE) {
		buffer[headIndex++] = USART1->DR & 0xFF;
		if(headIndex >= 20) headIndex = 0;
	}
}
