#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "config.h"

const int pirPin = 4; // GPIO4 (D2)

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(pirPin, INPUT);
}

void sendMotionAlert(String message) {
  WiFiClientSecure client;
  client.setInsecure(); 

  Serial.println("Starting motion alert...");
  Serial.print("Free heap before sending: ");
  Serial.println(ESP.getFreeHeap());

  const char* host = LAMBDA_ENDPOINT;
  const int httpsPort = 443;
  String url = "/default/motionLog";

  Serial.print("Connecting to host: ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("Failed to connect to Lambda API.");
    return;
  }
  Serial.println("Connected to Lambda API.");

  String payload = "{\"message\":\"" + message + "\"}";

  String request = String("POST ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + payload.length() + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   payload;

  Serial.println("Sending HTTP request:");
  Serial.println(request);

  client.print(request);

  Serial.println("Waiting for server response...");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("End of headers.");
      break;
    }
  }

  Serial.println("Done reading response.");
  Serial.print("Free heap after sending: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {
  if (digitalRead(pirPin) == HIGH) {
    Serial.println("🚨 Motion detected from ESP8266!");
    sendMotionAlert("Motion detected from ESP8266!");
    delay(10000); 
  } else {
    delay(100);
  }
}
