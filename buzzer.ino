#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

const int LED_PIN = D10;
int received_minimum;
int ontime = 200;

unsigned long lastReceived = 0;
const unsigned long TIMEOUT = 1000; //milliseconds

unsigned long lastStateChange = 0;
bool buzzerOn = false;


void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(received_minimum)) {
    memcpy(&received_minimum, incomingData, sizeof(received_minimum));
    lastReceived = millis();
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  delay(1000);

  Serial.print("hello from led esp32");

  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_NONE);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("success initializing esp-now");
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  unsigned long currentMillis = millis();

  bool dataIsFresh = (currentMillis - lastReceived) < TIMEOUT;

  Serial.println(received_minimum);

  if (dataIsFresh && received_minimum <= 500 && received_minimum > 0){

    if (!buzzerOn) { //buzzer is currently OFF
      digitalWrite(LED_PIN, HIGH);
      buzzerOn = true;
      lastStateChange = currentMillis
    }

    else if (currentMillis - lastStateChange  >= (unsigned long)ontime) { //buzzer is currently ON

      digitalWrite(LED_PIN, LOW);
      buzzerOn = false;
      lastStateChange = currentMillis;

    }

    //buzzer is currently OFF

  } else {
    //no valid or fresh obstacle detection
    if (buzzerOn) {
      digitalWrite(LED_PIN, LOW);
      buzzerOn = false;
    }
    lastStateChange = currentMillis;
  }

  //delay(100);
}