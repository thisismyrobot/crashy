#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

bool sphero_connect(char* name)
{
  SerialBT.enableSSP();
  if (!SerialBT.begin("ESP32-CAM", true)) return false;
  if (!SerialBT.setPin("1234")) return false;
  if (!SerialBT.isReady(true, 5)) return false;
  if (!SerialBT.connect(name)) return false;
  if (!SerialBT.connected(30)) return false;
  return true;
}

void sphero_roll(){
  Serial.print("Writing... ");
  SerialBT.write(0xFF);
  SerialBT.write(0xFE);
  SerialBT.write(0x02);
  SerialBT.write(0x30);
  SerialBT.write(0x01);
  SerialBT.write(0x05);
  SerialBT.write(0x1e);
  SerialBT.write(0x00);
  SerialBT.write(0x00);
  SerialBT.write(0x01);
  SerialBT.write(0xa8);
  Serial.print("Written!");  
}
