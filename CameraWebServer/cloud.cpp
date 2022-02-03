#include <Arduino.h>
#include <WiFi.h>
#include "photo.h"
#include "settings.h"

#define WIFI_RETRIES 5

bool connectWifi(int retries) {
  WiFi.begin(ssid, password);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tries++;

    if (tries > retries) {
        return false;
    }
  }

  return true;
}

void savePhoto(photo_fb_t * photo) {
  if(!connectWifi(WIFI_RETRIES)) {
    Serial.println("Failed to connect to WiFi!");
    return;
  }
}
