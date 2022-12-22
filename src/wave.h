/*
 * wave.h
 *
 *  Created on: 21 d�c. 2022
 *      Author: Jean-F�lix
 */

#ifndef WAVE_H_
#define WAVE_H_

#define INCREMENT 1
#define NBUTTON 10

struct wave {
	uint16_t volume;  // from 0 to 100
	uint16_t period;  // nb clk cycle the timer needs to count
};

void initWaves(void);
void turnUpVolume(uint8_t increment);
void turnDownVolume(uint8_t increment);
void changePeriod(uint16_t period, uint8_t indexWave);
void changeWaveSelected(uint8_t index);
void calculteWaveValue(void);
void changeWave(uint8_t val);

#endif /* WAVE_H_ */
