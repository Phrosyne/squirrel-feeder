#include "WiFi.h"
#include "esp_wpa2.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#include "hush.h"

void sendDataToFirestore(int, int);

void setup() {
  Serial.begin(115200);

  delay(5000);

  Serial.print("Connecting");

  WiFi.disconnect(true); // Disconnect from any previous networks
  WiFi.mode(WIFI_STA);

  // Configure WPA2 Enterprise Enterprise Identity and Password
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)id, strlen(id));

  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)id, strlen(id));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
  
  // Enable WPA2 Enterprise
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println(F("Connected!"));
}

void loop() {
}