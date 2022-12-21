#ifndef CMD_H_
#define CMD_H_

// Available commands
#define CMD_SOUND_SELECT	0x41

#define CMD_PARAM_MAX		11


int cmd_exec(unsigned char cmd, unsigned char arg, unsigned char chksum);

#endif /* CMD_H_ */
