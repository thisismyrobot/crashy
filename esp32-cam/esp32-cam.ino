/*
 * Add https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json 
 * as an additional board manager URL.
 * Install esp32 board via the board manager (2.0.2).
 * Select Heltec_Wifi_Kit_32 as board.
*/
#include "BluetoothSerial.h"

#define SPHERO_NAME "Sphero-RPW"
uint8_t address[6] = {0x68, 0x86, 0xe7, 0x01, 0xaa, 0xd2};

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  bool result;

  result = SerialBT.begin("ESP32-CAM", true);
  Serial.print("begin: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.setPin("1234");
  Serial.print("setPin: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.isReady(true, 5);
  Serial.print("ready: ");
  Serial.println(result ? "True" : "False");  

  result = SerialBT.connect(SPHERO_NAME);
  Serial.print("Connect: ");
  Serial.println(result ? "True" : "False");

  result = SerialBT.connected(30);
  Serial.print("Connected: ");
  Serial.println(result ? "True" : "False");

  Serial.print("Writing... ");
  // '0xff', '0xfe', '0x2', '0x30', '0x1', '0x5', '0x1e', '0x0', '0x0', '0x1', '0xa8'
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

void loop() {
  delay(100);
}
