import sys

import serial


def set_checksum(cmd):
    cmd[-1] = ~(sum(cmd[2:-1]) % 256) & 0xFF


def main(port='COM1'):
    ping_cmd =  [
        0xff,  # SOP
        0xff,  # Reset timeout and expect an answer.
        0x00,  # Device ID 0 - the core.
        0x01,  # Command ID 1 - Ping.
        0x01,  # Sequence ID.
        0x01,  # Data length.
        0x00,  # Checksum - to be calculated.
    ]

    set_checksum(ping_cmd)

    print(ping_cmd)

#    set_checksum([0xFF, 0xFF, 0x00, 0x01, 0x52, 0x01, 0x00])


#    return

    with serial.Serial(port, 115200, timeout=5, writeTimeout=5) as ser:
        ser.write(bytearray(ping_cmd))
        print(ser.read(256))


if __name__ == '__main__':
    main(sys.argv[1])
