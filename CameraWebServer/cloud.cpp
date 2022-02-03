#include <Arduino.h>
#include <WiFi.h>
#include "photo.h"
#include "settings.h"

void connectWifi();

void uploadPhoto(photo_fb_t * photo) {
  connectWifi();    
}

void connectWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
