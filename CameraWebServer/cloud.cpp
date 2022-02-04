#include <Arduino.h>
#include <WiFi.h>
#include <LCBUrl.h>
#include "photo.h"
#include "settings.h"

#define WIFI_RETRIES 5

WiFiClient client;

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

void upload(String host, int port, String path, photo_fb_t * photo) {
  if (!client.connect(host.c_str(), port)) {
    Serial.println("Failed to connect to host and port!");
    return;
  }

  String head = "--boundary\r\nContent-Disposition: form-data; name=\"image\"; filename=\"image.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--boundary--\r\n";

  uint32_t extraLen = head.length() + tail.length();
  uint32_t totalLen = photo->len + extraLen;
   
  client.println("POST /" + path + " HTTP/1.1");
  client.println("Host: " + host + ":" + String(port));
  client.println("Content-Length: " + String(totalLen));
  client.println("Content-Type: multipart/form-data; boundary=\"boundary\"");
  client.println();
  client.print(head);

  uint8_t *fbBuf = photo->buf;
  size_t fbLen = photo->len;
  for (size_t n=0; n<fbLen; n=n+1024) {
    if (n+1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    }
    else if (fbLen%1024>0) {
      size_t remainder = fbLen%1024;
      client.write(fbBuf, remainder);
    }
  }
  client.print(tail);
  client.stop();
}

void savePhoto(photo_fb_t * photo) {
  if(!connectWifi(WIFI_RETRIES)) {
    Serial.println("Failed to connect to WiFi!");
    return;
  }

  LCBUrl url;
  if (!url.setUrl(uploadEndpoint)) {
    Serial.println("Failed to parse endpoint URL!");
    return;
  }

  upload(url.getHost(), url.getPort(), url.getPath(), photo);
}
