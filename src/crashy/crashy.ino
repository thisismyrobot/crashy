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

  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  delay(1000);

  if(!setupCamera()) {
    Serial.println("Camera init failed!");
    delay(10000);
    ESP.restart();
  }

  while(1) {
  
    if(!explore()) {
      Serial.println("Failed to explore!");    
      delay(10000);
      ESP.restart();
    }
    
    // if (crashed()) {
  
    photo_fb_t * last_photo = takePhoto();
    if (last_photo == NULL) {
      Serial.println("Failed to take photo!");
      delay(10000);
      ESP.restart();
    }
    
    int saveError = 0;
    if(!savePhoto(last_photo, &saveError)) {
      Serial.printf("Failed to save photo: %d\n", saveError);
      delay(10000);
      ESP.restart();
    }
  
    for (int i = 0; i < 3; i++) {
      digitalWrite(33, HIGH);
      delay(500);
      digitalWrite(33, LOW);
      delay(500);
    }
    esp_deep_sleep_start();
    
    // }
  }
}

void loop() {
    // nothing to do.
}
