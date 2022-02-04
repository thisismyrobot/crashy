#include <Arduino.h>
#include <WiFi.h>
#include <LCBUrl.h>
#include "photo.h"
#include "settings.h"

#define ERROR_WIFI_CONNECT 1
#define ERROR_PARSE_URL 2
#define ERROR_UPLOAD 3

#define WIFI_RETRIES 2

WiFiClient client;

bool connectWiFi(int retries) {
  int beginTries = 0;
  while(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);

    int waits = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      waits++;
  
      // If it doesn't work in 5s, then it's bust.
      if (waits > 10) {
        beginTries++;
        if (beginTries > WIFI_RETRIES) {
          return false;
        }
        else {
          break;
        }
      }
    }    
  }

  return true;
}

bool disconnectWiFi() {
  WiFi.disconnect();  
}

bool upload(String host, int port, String path, photo_fb_t * photo) {
  if (!client.connect(host.c_str(), port)) {
    return false;
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

  delay(2000);
  Serial.println("Upload complete!");
  return true;
}

bool savePhoto(photo_fb_t * photo, int * error) {
  if(!connectWiFi(WIFI_RETRIES)) {
    *error = ERROR_WIFI_CONNECT;
    return false;
  }

  LCBUrl url;
  if (!url.setUrl(uploadEndpoint)) {
    *error = ERROR_PARSE_URL;
    disconnectWiFi();
    return false;
  }
  
  if(!upload(url.getHost(), url.getPort(), url.getPath(), photo)) {
    *error = ERROR_UPLOAD;
    disconnectWiFi();
    return false;
  }

  disconnectWiFi();
  return true;
}
