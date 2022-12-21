#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <DMA.h>
#include <timer.h>
#include <wave.h>

struct wave waves[NBUTTON];
uint8_t indexWaveSelected = 0;
uint16_t waveSelectValue[4];

void initWaves(void) {
	// From 100 Hz to 4kHz
    for(int i = 0; i < NBUTTON; i++) {
    	waves[i].volume = 64;
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
	if(volume+increment < 100) volume = 100; // Check for overflow
	else volume += increment;
	waves[indexWaveSelected].volume = volume;
}

void turnDownVolume(uint8_t increment) {
	uint16_t volume = waves[indexWaveSelected].volume;
	if(volume-increment > volume) volume = 0; // Check for overflow
	else volume -= increment;
	waves[indexWaveSelected].volume = volume;
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
}

void changeWaveSelected(uint8_t index) {
	if(index > NBUTTON) return;
	indexWaveSelected = index;
	TIM6->ARR = waves[index].period;
}

void calculteWaveValue(void) {
	// y = mx + b; x : volume and y : wave value
	// For high value -> when x = 100, y = 4095; when x = 0, y = 2048; b = 2048 and m = 20.47
	// For low value -> when x = 100, y = 0; when x = 0, y = 2048; b = 2048 and m = -20.47
	uint32_t b = 2097152; // 2048 << 10 = 2097152
	uint32_t m = 20961;   // 20.47 << 10 = 20961
	uint32_t highValue = waves[indexWaveSelected].volume;
	highValue *= m;
	highValue += b;
	uint32_t highValueRoundUp = highValue;
	highValueRoundUp >>= 10;
	highValueRoundUp <<= 10;
	if(highValue - highValueRoundUp > 512) highValueRoundUp += 1024; // 0.5 << 10 = 512
	highValueRoundUp >>= 10;
	int32_t lowValue = waves[indexWaveSelected].volume;
	lowValue *= -m;
	lowValue += b;
	uint32_t lowValueRoundUp = lowValue;
	lowValueRoundUp >>= 10;
	if(lowValueRoundUp != 0) {
		lowValueRoundUp <<= 10;
		if(lowValue - lowValueRoundUp > 512) lowValueRoundUp += 1024; // 0.5 << 10 = 512
		lowValueRoundUp >>= 10;
	}
	waveSelectValue[0] = lowValueRoundUp;
	waveSelectValue[1] = lowValueRoundUp;
	waveSelectValue[1] = highValueRoundUp;
	waveSelectValue[2] = highValueRoundUp;
}
