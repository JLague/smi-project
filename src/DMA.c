#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "DMA.h"

void initDMA(void) {
	RCC->AHB1ENR |= (1 << 21);
	// CHSEL=7, MBURST=0, PBURST=0, CT=don't_care, DBM=0, PL=3, PINCOS=0, MSIZE=1, PSIZE=1, MINC=1, PINC=0, CIRC=1, DIR=1, PFCTRL=0, TCIE=don't_care, HTIE=don't_care, TEIT=don't_care, DMEIE=don't_care
	DMA1_Stream5->CR &= ~((0b11 << 23) | (0b11 << 21) | (0b1 << 18) | (0b1 << 15) | (0b10 << 11) | (0b1 << 10) | (0b1 << 9) | (0b1 << 8) | (0b10 << 6) | (0b1 << 5));
	DMA1_Stream5->CR |= (0b111 << 25) | (0b11 << 16) | (0b01 << 13) | (0b01 << 11) | (0b01 << 6) | DMA_SxCR_MINC | DMA_SxCR_CIRC;
	DMA1_Stream5->NDTR = 2;
	DMA1_Stream5->PAR=(uint32_t)&(DAC->DHR12R1);
}

void configSrcAddrDMA(uint32_t srcAddr) {
	DMA1_Stream5->CR &= ~0b1;  // Disable DMA
	DMA1_Stream5->M0AR = srcAddr;
	DMA1_Stream5->CR |= 0b1;  // Enable DMA
}
