import sys
import time

import serial


class Sphero2CommandBuilder():
    def __init__(self, sequence_ids=True):
        self._sequence_id = 0
        self._sequence_ids = sequence_ids

    @property
    def _sequence(self):
        if self._sequence_ids:
            self._sequence_id += 1
        return self._sequence_id

    def _build(self, device_id, command_id, *data, expect_answer=False, parser=None):
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

        class Command():
            def __init__(self):
                self.bytes = bytes(cmd)
                self.parse = parser if parser is not None else lambda x: None

            @property
            def arduino_bytes(self):
                line = 'uint8_t command[] = {'
                line += ', '.join([f'0x{b:02X}' for b in self.bytes])
                line += '};'
                return line

        return Command()

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
        return self._build(0x02, 0x30, 0x00, 0x00, 0x00, 0x00)

    def sleep(self, deep=False):
        """Warning: deep sleep requires charger to wake."""
        if deep:
            return self._build(0x00, 0x22, 0xFF, 0xFF, 0x00, 0x00, 0x00)
        else:
            return self._build(0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00)

    def battery_voltage(self):
        def parser(response):
            voltage_msb = response[7]
            voltage_lsb = response[8]
            return ((voltage_msb * 255) + voltage_lsb) / 100.0

        return self._build(0x00, 0x20, expect_answer=True, parser=parser)


def main(port='COM1'):
    commands = Sphero2CommandBuilder()

    with serial.Serial(port, 115200, timeout=5, writeTimeout=5) as ser:
        ser.write(commands.ping().bytes)
        print(f'Ping response: {ser.read(256)}')

        voltage_command = commands.battery_voltage()
        ser.write(voltage_command.bytes)
        voltage = voltage_command.parse(ser.read(256))
        print(f'Voltage: {voltage} volts')

        ser.write(commands.roll(30).bytes)
        time.sleep(1)
        ser.write(commands.stop().bytes)

        time.sleep(1)
        ser.write(commands.sleep().bytes)


if __name__ == '__main__':
    main(sys.argv[1])
