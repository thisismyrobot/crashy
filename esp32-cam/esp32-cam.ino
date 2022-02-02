/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install esp32 board via the board manager (1.0.2).
 * Select Heltec_Wifi_Kit_32 as board.
*/
#define SPHERO_NAME "Sphero-RPW"
#define EXPLORE_TIME 10
#define CONNECT_RETRIES 3

void setup() {
  Serial.begin(115200);

  explore();
}

void loop() {
  delay(100);
}

void explore() {
  int i;
  bool connected;
  for(i = 0; i < CONNECT_RETRIES; i++) {
    if(sphero_connect(SPHERO_NAME)) {
      connected = true;
      break;
    }    
  }

  if (!connected) {
    return;
  }

  for(i = 0; i < EXPLORE_TIME; i++) {
    sphero_roll();
    delay(1000);  
  }

  sphero_disconnect();
}
