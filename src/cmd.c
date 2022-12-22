#include <stdbool.h>
#include "stm32f4xx.h"
#include "wave.h"
#include "cmd.h"


bool _checksum(uint8_t cmd[])
{
	uint32_t sum = 0;
	for (uint8_t i = 0; i < CMD_LEN; ++i) {
		sum += cmd[i];
	}

	return sum % 256 == 0;
}

int cmd_exec(uint8_t cmd[])
{
	if (!_checksum(cmd)) return -1;

	uint8_t arg1 = cmd[1];
	uint16_t arg2 = cmd[2];

	int ret = 0;
	switch (cmd[0]) {
		case CMD_SS:
			if (arg1 > CMD_PARAM_SS_MAX) return -1;
			changeWaveSelected(arg1);
			break;
		case CMD_VOL:
			if (arg1 > CMD_PARAM_VOL_MAX) return -1;
			setVolume(arg1);
			break;
		case CMD_VOL_ALL:
			if (arg1 > CMD_PARAM_VOL_MAX) return -1;
			setVolumeAll(arg1);
			break;
		case CMD_PERIOD:
			if (arg1 > CMD_PARAM_SS_MAX) return -1;
			arg2 = (arg2 << 8) + cmd[3];
			changePeriod(arg2, arg1);
			break;
	}
	return ret;
}

