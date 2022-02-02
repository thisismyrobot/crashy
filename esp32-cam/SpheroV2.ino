#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

bool sphero_connect(char* name) {
  SerialBT.enableSSP();
  if (!SerialBT.begin("ESP32-CAM", true)) return false;
  if (!SerialBT.setPin("1234")) return false;
  if (!SerialBT.isReady(true, 5)) return false;
  if (!SerialBT.connect(name)) return false;
  if (!SerialBT.connected(30)) return false;
  return true;
}

void sphero_disconnect() {
  SerialBT.end();
}

void sphero_roll(){
  uint8_t request[] = {0xFF, 0xFE, 0x02, 0x30, 0x01, 0x05, 0x1e, 0x00, 0x00, 0x01, 0xa8};
  SerialBT.write(request, sizeof request);
}
