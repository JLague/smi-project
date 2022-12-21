import os
import cmd
import serial

# OS specific serial port
WINDOWS_PORT = 'COM3'
POSIX_PORT = '/dev/ttyUSB0'

BAUDRATE = 19200

# Available commands
SOUND_SELECT_CMD = b'\x41'

# Parameters
PARAM_MAX = 11

def uart_init(baudrate=BAUDRATE):
    """Initialize UART port"""
    if os.name == 'nt':
        port = WINDOWS_PORT
    else:
        port = POSIX_PORT

    return serial.Serial(port, baudrate, timeout=1)

def calculate_checksum(data):
    """Calculate checksum for data"""
    sum = 0
    for byte in data:
        sum += byte
    return 256 - sum % 256

def get_full_command(command, params):
    """Get full command with checksum"""
    data = bytearray(command + params)
    checksum = calculate_checksum(data)
    return data + bytes([checksum])

class UartCmd(cmd.Cmd):
    """UART command line interface"""
    prompt = 'uart> '
    intro = 'UART command line interface'

    def __init__(self, uart):
        super().__init__()
        self.uart = uart

    def do_play(self, arg):
        """Play sound"""
        if arg.isdigit() and int(arg) <= PARAM_MAX:
            self.uart.write(get_full_command(SOUND_SELECT_CMD, bytes([int(arg)])))
        else:
            print('Invalid parameter')

    def do_exit(self, arg):
        """Exit UART command line interface"""
        return True

if __name__ == '__main__':
    uart = uart_init()
    UartCmd(uart).cmdloop()
