/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install "esp32" via the board manager (IMPORTANT: Must be version 1.0.2).
 * Select "AI Thinker ESP32-CAM" as the board.
*/
#include <WiFi.h>
#include "esp_camera.h"

#include "settings.h"

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  setupCamera();
    
  camera_fb_t * x = NULL;
  x = takePhoto();
  
  size_t len = x->len;
  uint8_t *fbBuf = x->buf;

  Serial.println(len);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}
