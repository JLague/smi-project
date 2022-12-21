#include <stdbool.h>
#include "stm32f4xx.h"
#include "cmd.h"


bool _checksum(unsigned char cmd, unsigned char arg, unsigned char chksum)
{
	return (cmd + arg + chksum) % 256 == 0;
}

int cmd_exec(unsigned char cmd, unsigned char arg, unsigned char chksum)
{
	if (!_checksum(cmd, arg, chksum)) return -1;

	int ret = 0;
	switch (cmd) {
		case CMD_SOUND_SELECT:
			if (arg > CMD_PARAM_MAX) return -1;
			// TODO handle sélection d'un son
			break;
	}
	return ret;
}

