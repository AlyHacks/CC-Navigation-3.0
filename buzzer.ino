#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

const int BUZZER_PIN = D10;

volatile int received_minimum = 1000000; //fake placeholder value

int ontime = 200;

volatile unsigned long lastReceived = 0;
const unsigned long TIMEOUT = 300; //milliseconds

unsigned long lastStateChange = 0;

bool buzzerOn = false;
bool previousObstacleDetected = false;


void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(int)) {
    int newMinimum;
    memcpy(&newMinimum, incomingData, sizeof(newMinimum));
    received_minimum = newMinimum;
    lastReceived = millis();
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_NONE);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed.");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  unsigned long currentMillis = millis();

  int currentMinimum = received_minimum;
  unsigned long currentLastReceived = lastReceived;

  bool dataIsFresh = (currentLastReceived != 0) && (currentMillis - currentLastReceived) < TIMEOUT;

  bool obstacleDetected = (dataIsFresh) && (currentMinimum <= 500) && (currentMinimum > 0);

  Serial.println(received_minimum);

  if (obstacleDetected) {

    if (!previousObstacleDetected) { //buzzer is currently OFF
      digitalWrite(BUZZER_PIN, HIGH);
      buzzerOn = true;
      lastStateChange = currentMillis;
    }
    
    else if (buzzerOn) { //buzzer is currently ON

      if (currentMillis - lastStateChange >= ontime) {
        digitalWrite(BUZZER_PIN, LOW);
        buzzerOn = false;
        lastStateChange = currentMillis;
      }

    }

    //buzzer is currently OFF

    else {

      if (currentMillis - lastStateChange >= currentMinimum) {
        digitalWrite(BUZZER_PIN, HIGH);
        buzzerOn = true;
        lastStateChange = currentMillis;
      }
    
    }
  }

  else {
    //stop buzzing if no obstacle detected or if data is stale
    if (buzzerOn) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerOn = false;
    }
  }

  previousObstacleDetected = obstacleDetected;

}