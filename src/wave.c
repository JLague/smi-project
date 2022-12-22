#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <DMA.h>
#include <timer.h>
#include <wave.h>

struct wave waves[NBUTTON];
uint8_t indexWaveSelected = 0;
uint16_t waveSelectValue[2];

void initWaves(void) {
	// From 100 Hz to 4kHz
    for(int i = 0; i < NBUTTON; i++) {
    	waves[i].volume = 50;
    }
    changePeriod(3822, 0); // Do 261.63 Hz
    changePeriod(3405, 1); // Re 293.66 Hz
    changePeriod(3034, 2); // Mi 329.63 Hz
    changePeriod(2863, 3); // Fa 349.23 Hz
    changePeriod(2551, 4); // Sol 392.00 Hz
    changePeriod(2273, 5); // La 440.00 Hz
    changePeriod(2025, 6); // Si 493.88 Hz
    changePeriod(1911, 7); // Do 523.25 Hz
    changePeriod(16667, 8); // Fucking grave 60 Hz
    changePeriod(100, 9);  // Je saigne des oreilles 10 kHz
    calculteWaveValue();

}

void turnUpVolume(uint8_t increment) {
	uint16_t volume = waves[indexWaveSelected].volume;
	if(volume+increment > 100) volume = 100; // Check for overflow
	else volume += increment;
	waves[indexWaveSelected].volume = volume;
	calculteWaveValue();
}

void turnDownVolume(uint8_t increment) {
	uint16_t volume = waves[indexWaveSelected].volume;
	if(volume-increment > volume) volume = 0; // Check for overflow
	else volume -= increment;
	waves[indexWaveSelected].volume = volume;
	calculteWaveValue();
}

void changePeriod(uint16_t period, uint8_t indexWave) {
	// period is in us
	if(indexWave >= NBUTTON) return;
	uint32_t ARR = period; // The ARR register of timer 6
	ARR *= 20972; // (0.04/2) << 20 = 20972
	uint32_t ARR_roundUp = ARR;
	ARR_roundUp >>= 20;
	ARR_roundUp <<= 20;
	if(ARR - ARR_roundUp > 524288) ARR_roundUp += 1048576; // 0.5 << 20 = 524288
	ARR_roundUp >>= 20;
	waves[indexWave].period = ARR_roundUp;
	if(indexWave == indexWaveSelected) TIM6->ARR = waves[indexWave].period;
}

void changeWaveSelected(uint8_t index) {
	if(index > NBUTTON) return;
	indexWaveSelected = index;
	TIM6->ARR = waves[index].period;
	calculteWaveValue();
}

void calculteWaveValue(void) {
	if(waves[indexWaveSelected].volume == 0) {
		waveSelectValue[0] = 2048;
		waveSelectValue[1] = 2048;
	}
	else {
		uint32_t b_max = 3982274; // 3889 << 10 = 3982274
		uint32_t m_max = 2110;    // 2.06 << 10 = 2110
		uint32_t highValue = waves[indexWaveSelected].volume;
		highValue *= m_max;
		highValue += b_max;
		uint32_t highValueRoundUp = highValue;
		highValueRoundUp >>= 10;
		highValueRoundUp <<= 10;
		if(highValue - highValueRoundUp > 512) highValueRoundUp += 1024; // 0.5 << 10 = 512
		highValueRoundUp >>= 10;
		uint32_t b_min = 212040; // 207.07 << 10 = 212040
		int32_t m_min = -2120;    // -2.07 << 10 = -2120
		int32_t lowValue = waves[indexWaveSelected].volume;
		lowValue *= m_min;
		lowValue += b_min;
		uint32_t lowValueRoundUp = lowValue;
		lowValueRoundUp >>= 10;
		if(lowValueRoundUp != 0) {
			lowValueRoundUp <<= 10;
			if(lowValue - lowValueRoundUp > 512) lowValueRoundUp += 1024; // 0.5 << 10 = 512
			lowValueRoundUp >>= 10;
		}
		waveSelectValue[0] = lowValueRoundUp;
		waveSelectValue[1] = highValueRoundUp;
	}
}

void setVolumeAll(uint8_t volume) {
    for(int i = 0; i < NBUTON; i++) {
    	waves[i].volume = volume;
    }
    calculteWaveValue();
}

void setVolume(uint8_t volume) {
	waves[indexWaveSelected].volume = volume;
	calculteWaveValue();
}
void changeWave(uint8_t val) {
	if (val == 10) turnDownVolume(INCREMENT);
	else if (val == 11) turnUpVolume(INCREMENT);
	else changeWaveSelected(val);
}
