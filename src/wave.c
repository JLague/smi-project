#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <DMA.h>
#include <timer.h>
#include <wave.h>

struct wave waves[NBUTTON];
uint8_t indexWaveSelected = 0;
uint16_t waveSelectedLowValue;
uint16_t waveSelectedHighValue;

void initWaves(void) {
    for(int i = 0; i < NBUTTON; i++) {
    	waves[i].volume = 64;
    	waves[i].period = i;
    }
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
	if(indexWave >= NBUTTON) return;
	waves[indexWave].period = period;
}

void changeWaveSelected(uint8_t index) {
	if(index > NBUTTON) return;
	indexWaveSelected = index;
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
	waveSelectedLowValue = lowValueRoundUp;
	waveSelectedHighValue = highValueRoundUp;
}
