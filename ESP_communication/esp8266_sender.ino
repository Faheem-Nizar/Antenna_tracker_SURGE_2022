#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
// Mac address of the slave
uint8_t peer1[] = {0xB8, 0xD6, 0x1A, 0x82, 0x42, 0xA4}; //  B8:D6:1A:82:40:A4
typedef struct message {
    char buffer[255];
};
struct message myMessage;
void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println("Status:");
  Serial.println(sendStatus);
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP-Now Sender");
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Register the peer
  Serial.println("Registering a peer");
  esp_now_add_peer(peer1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("Registering send callback function");
  esp_now_register_send_cb(onSent);
}
void loop() {
 if (SerialPort.available()) {      // If anything comes in Serial (USB),

    input_count = SerialPort.available();
    if (input_count > 249)
    {
      input_count = 249;
    }

    bytes_read = SerialPort.readBytes(sentData.buffer, input_count);   // this is a problem because of the carriage returnb

    sentData.buffer[bytes_read] = 0; // safety for printing
    

    esp_err_t result = esp_now_send(RxMACaddress, (uint8_t *) &sentData.buffer, bytes_read);
 }
}
