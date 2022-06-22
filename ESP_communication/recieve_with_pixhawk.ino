#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>


//Structure example to receive data
//Must match the sender structure
typedef struct RxStruct
{
  char buffer[255];
} RxStruct;
RxStruct receivedData;

//Create a struct_message called myData


int count = 0;

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  int byteCount = len;
  memcpy(&receivedData, incomingData, byteCount);
//  Serial.println(receivedData.buffer);
  Serial2.write(incomingData, byteCount);
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
//  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
