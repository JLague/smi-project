#ifndef CMD_H_
#define CMD_H_

#define CMD_LEN				5

// Available commands
#define CMD_SS				0x41
#define CMD_VOL				0x42
#define CMD_VOL_ALL			0x43
#define CMD_PERIOD			0x44

// Params
#define CMD_PARAM_SS_MAX	9
#define CMD_PARAM_VOL_MAX	100


int cmd_exec(uint8_t cmd[]);

#endif /* CMD_H_ */
