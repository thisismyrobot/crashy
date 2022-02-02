import sys
import time

import serial


class Sphero2CommandBuilder():
    def __init__(self, sequence_ids=True):
        self._sequence_id = 54
        self._sequence_ids = sequence_ids

    @property
    def _sequence(self):
        if self._sequence_ids:
            self._sequence_id += 1
        return self._sequence_id

    def _build(self, device_id, command_id, *data, expect_answer=False):
        cmd = [
            0xff,  # SOP
            0xff if expect_answer else 0xfe,  # Reset timeout, maybe expect an answer.
            device_id,
            command_id,
            self._sequence,
            len(data) + 1  # Data length (including checksum byte).
        ] + list(data)

        # Checksum
        cmd += [~(sum(cmd[2:]) % 256) & 0xFF]

        return bytes(cmd)

    def _pack_data(self, *data):
        pass

    def ping(self):
        return self._build(0x00, 0x01, expect_answer=True)

    def roll(self, speed, heading=0):
        speed = max(min(speed, 255), 0)
        heading = max(min(heading, 359), 0)

        heading_msb = 0
        heading_lsb = heading
        if heading_lsb > 255:
            heading_msb = 1
            heading_lsb = heading_lsb - 256

        return self._build(0x02, 0x30, speed, heading_msb, heading_lsb, 1)

    def stop(self):
        return self._build(0x02, 0x30, 0, 0, 0, 0)


def main(port='COM1'):
    builder = Sphero2CommandBuilder()

    with serial.Serial(port, 115200, timeout=5, writeTimeout=5) as ser:
        ser.write(builder.ping())
        print(f'Ping response: {ser.read(256)}')

        ser.write(builder.roll(30))
        time.sleep(1)
        ser.write(builder.stop())


if __name__ == '__main__':
    main(sys.argv[1])
