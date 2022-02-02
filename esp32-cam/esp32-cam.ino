/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install esp32 board via the board manager (1.0.2).
 * Select Heltec_Wifi_Kit_32 as board.
*/
#define SPHERO_NAME "Sphero-RPW"

void setup() {
  Serial.begin(115200);

  sphero_connect(SPHERO_NAME);
  sphero_roll();
}

void loop() {
  delay(100);
}
