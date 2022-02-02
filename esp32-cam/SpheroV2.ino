#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void sphero_connect(char* name)
{
  bool result;

  SerialBT.enableSSP();

  result = SerialBT.begin("ESP32-CAM", true);
  Serial.print("begin: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.setPin("1234");
  Serial.print("setPin: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.isReady(true, 5);
  Serial.print("ready: ");
  Serial.println(result ? "True" : "False");  

  result = SerialBT.connect(name);
  Serial.print("Connect: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.connected(30);
  Serial.print("Connected: ");
  Serial.println(result ? "True" : "False");
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
