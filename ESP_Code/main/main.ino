#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
WebSocketsClient webSocket;
const char* ssid = "SUNHOUSENM"; //Đổi thành wifi của bạn
const char* password = "Shg@2021"; //Đổi pass luôn
const char* ip_host = "172.16.4.223"; //Đổi luôn IP host của PC nha
const uint16_t port = 3000; //Port thích đổi thì phải đổi ở server nữa
const int LED = 2;
const int BTN = 0;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      if (strcmp((char*)payload, "LED_ON") == 0) {
        digitalWrite(LED, 0); // Khi client phát sự kiện "LED_ON" thì server sẽ bật LED
      } else if (strcmp((char*)payload, "LED_OFF") == 0) {
        digitalWrite(LED, 1); // Khi client phát sự kiện "LED_OFF" thì server sẽ tắt LED
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      if (length == 6) {
        digitalWrite(LED, 0); // Khi client phát sự kiện "LED_ON" thì server sẽ bật LED
      } else if (length == 7) {
        digitalWrite(LED, 1); // Khi client phát sự kiện "LED_OFF" thì server sẽ tắt LED
      }
      break;
  }
}
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  Serial.begin(9600);
  Serial.println("ESP8266 Websocket Client");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  webSocket.begin(ip_host, port);
  webSocket.onEvent(webSocketEvent);
}
void loop() {
  webSocket.loop();
  static bool isPressed = false;
  if (!isPressed && digitalRead(BTN) == 0) { //Nhấn nút nhấn GPIO0
    isPressed = true;
    webSocket.sendTXT("BTN_PRESSED");
  } else if (isPressed && digitalRead(BTN)) { //Nhả nút nhấn GPIO0
    isPressed = false;
    webSocket.sendTXT("BTN_RELEASE");
  }
}
