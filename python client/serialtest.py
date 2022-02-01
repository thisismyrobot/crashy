"""

Preconditions
=============

This computer has Bluetooth. You have paired your Bluetooth serial device to
this computer as a COM port.

Example hex strings
===================

Sphero - set colour red, no response expected: FFFE02200005FF000000D9
Sphero - set colour blue, no response expected: FFFE022000050000FF00D9
Sphero - send ping, response expected: FFFF00010001FD
Sphero - send ping, no response expected: FFFE00010001FD
Sphero - get power state, response expected: FFFF00200001DE

Usage:

    pipenv run python serialtest.py COM3

"""

# The standard imports
import binascii
import sys

import serial


def main(port='COM1'):

    # Open the comm port
    try:
        ser = serial.Serial(port, 115200, timeout=0.1)
    except:
        sys.stderr.write("Error: No Bluetooth Serial Device connected\n")
        sys.stderr.write("Check for OS Bluetooth connection request - use pin 1234\n")
        sys.exit(1)

    while True:
        # Get the bytes as a hex string
        try:
            hex_str = input('> ')
        except KeyboardInterrupt:
            sys.exit(0)

        # Convert them to actual bytes
        try:
            request = binascii.unhexlify(hex_str)
        except:
            sys.stderr.write("Error: failed to parse hex string\n")
            continue

        # Send them off.
        try:
            ser.write(request)
        except:
            sys.stderr.write("Error: failed to transmit bytes\n")
            sys.exit(1)

        # Get any back
        response = ser.read(256)
        hex_str = str(binascii.hexlify(response).upper())
        print(f'< {hex_str}')


if __name__ == '__main__':
    try:
        port = sys.argv[1]
        main(port)
    except IndexError:
        main()
