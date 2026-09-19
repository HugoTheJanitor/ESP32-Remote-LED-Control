#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

const char* SSID = "YOUR_WIFI_NAME";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";

const char* SERVER_URL =
  "https://YOUR-NGROK-DOMAIN.ngrok-free.dev/status";

const int RED_PIN = 18;
const int GREEN_PIN = 19;
const int BLUE_PIN = 21;
const int YELLOW_PIN = 23;

unsigned long lastCheck = 0;
const unsigned long POLL_INTERVAL = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - lastCheck < POLL_INTERVAL) return;
  lastCheck = millis();

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  if (http.begin(client, SERVER_URL)) {
    http.addHeader("ngrok-skip-browser-warning", "true");
    int code = http.GET();

    if (code == 200) {
      String payload = http.getString();
      payload.trim();

      Serial.print("Server status: ");
      Serial.println(payload);

      int red, green, blue, yellow;

      if (sscanf(
          payload.c_str(),
          "%d,%d,%d,%d",
          &red,
          &green,
          &blue,
          &yellow
      ) == 4) {
        digitalWrite(RED_PIN, red ? HIGH : LOW);
        digitalWrite(GREEN_PIN, green ? HIGH : LOW);
        digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
        digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
      }
    } else {
      Serial.print("HTTP error: ");
      Serial.println(code);
    }

    http.end();
  }
}
