/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install "esp32" via the board manager (IMPORTANT: Must be version 1.0.2).
 * Select "AI Thinker ESP32-CAM" as the board.
 * Install these libs:
 *  - LCBUrl (1.1.4)
*/
#include <Arduino.h>
#include "photo.h"
#include "camera.h"
#include "cloud.h"

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  if(!setupCamera()) {
    Serial.printf("Camera init failed!");
    return;
  }

  photo_fb_t * last_photo = takePhoto();
  if (last_photo == NULL) {
    return;
  }

  savePhoto(last_photo);
}

void loop() {
  delay(100);
}
