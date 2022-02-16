#include "BluetoothSerial.h"

#define SPHERO_NAME "Sphero-RPW"
#define EXPLORE_TIME 10
#define CONNECT_RETRIES 3

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
  uint8_t request[] = {0xFF, 0xFE, 0x02, 0x30, 0x01, 0x05, 0x50, 0x00, 0x00, 0x01, 0x76};
  SerialBT.write(request, sizeof request);
}

void sphero_deep_sleep() {
  uint8_t request[] = {0xFF, 0xFE, 0x00, 0x22, 0x01, 0x06, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xD8};
  SerialBT.write(request, sizeof request);
}

void enable_crash_detection() {
  uint8_t request[] = {0xFF, 0xFE, 0x02, 0x12, 0x03, 0x07, 0x01, 0x64, 0x64, 0x64, 0x64, 0x64, 0xEC};
  SerialBT.write(request, sizeof request);
}

bool crashed() {
  if (SerialBT.available() >= 3) {
    // First three bytes of async crash notification.
    if (SerialBT.read() == 0xFF && SerialBT.read() == 0xFE && SerialBT.read() == 0x07) {
      return true;      
    }
  }
  return false;
}

bool connect() {
  bool connected;
  for(int i = 0; i < CONNECT_RETRIES; i++) {
    if(sphero_connect(SPHERO_NAME)) {
      connected = true;
      break;
    }
  }

  if (!connected) {
    return false;
  }

  enable_crash_detection();
  return true;
}

void explore() {
  for(int i = 0; i < EXPLORE_TIME; i++) {
    sphero_roll();
    delay(1000);
  }
}

void disconnect() {
  sphero_deep_sleep();
  delay(500);
  sphero_disconnect();
}
