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
#include "SpheroV2.h"

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  if(!setupCamera()) {
    Serial.println("Camera init failed!");
    return;
  }
}

void loop() {

  // explore();
  // if (crashed()) {
  delay(10000);

  photo_fb_t * last_photo = takePhoto();
  if (last_photo == NULL) {
    Serial.println("Failed to take photo!");
    return;
  }

  int error;
  if(!savePhoto(last_photo, &error)) {
    Serial.printf("Failed to save photo: %d\n", error);
    return;
  }

  // }
}
