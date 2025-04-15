#include <ESP8266WiFi.h>
#include <WifiClientSecure.h>
#include "config.h"

const int pirPin = 4; // GPIO4 (D2)

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(pirPin, INPUT);

}

String urlencode(String str) {
  String encoded = "";
  char c;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      encoded += '%';
      char hex1 = (c >> 4) & 0xF;
      char hex2 = c & 0xF;
      encoded += "0123456789ABCDEF"[hex1];
      encoded += "0123456789ABCDEF"[hex2];
    }
  }
  return encoded;
}

void sendTelegramMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure(); 
  const char* host = "api.telegram.org";
  const int httpsPort = 443;

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection to Telegram failed.");
    return;
  }

  String url = "/bot" + String(TELEGRAM_BOT_TOKEN) +
               "/sendMessage?chat_id=" + String(TELEGRAM_CHAT_ID) +
               "&text=" + urlencode(message);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
    Serial.println(line);
  }

  Serial.println("Message sent to Telegram.");
}


void loop() {
  int motionDetected = digitalRead(pirPin);

  if (motionDetected == HIGH) {
    Serial.println("Motion detected!");
    sendTelegramMessage("Motion detected in your security system!");
    delay(10000); // Prevent multiple alerts in short intervals
  }
}


