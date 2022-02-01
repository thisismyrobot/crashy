import sys

import serial


def set_checksum(cmd):
    cmd[-1] = ~(sum(cmd[2:-1]) % 256) & 0xFF


def main(port='COM1'):
    ping_cmd =  [
        0xff,  # SOP
        0xff,  # Reset timeout, expect an answer.
        0x00,  # Device ID 0 - the core.
        0x01,  # Command ID 1 - Ping.
        0x05,  # Sequence ID.
        0x01,  # Data length.
        0x00,  # Checksum - to be calculated.
    ]

    set_checksum(ping_cmd)

    print(ping_cmd)

    with serial.Serial(port, 115200, timeout=5, writeTimeout=5) as ser:
        ser.write(bytearray(ping_cmd))
        print(ser.read(256))

    roll_cmd =  [
        0xff,  # SOP
        0xfe,  # Reset timeout, don't expect an answer.
        0x02,  # Device ID 2 - Sphero.
        0x30,  # Command ID 30 - Roll.
        0x01,  # Sequence ID.
        0x05,  # Data length.
        0x32,  # Speed.
        0x00,  # Heading MSB.
        0x00,  # Heading LSB.
        0x01,  # State for Go var - normal mode.
        0x00,  # Checksum - to be calculated.
    ]

    set_checksum(roll_cmd)

    print(roll_cmd)

    with serial.Serial(port, 115200, timeout=5, writeTimeout=5) as ser:
        ser.write(bytearray(roll_cmd))
#        print(ser.read(256))


if __name__ == '__main__':
    main(sys.argv[1])
