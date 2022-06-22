#include <ESP8266WiFi.h>
#include <espnow.h>
//#include<SoftwareSerial.h> 
//
//SoftwareSerial SerialPort(13,15);


// Structure example to receive data
// Must match the sender structure
typedef struct RxStruct
{
  char buffer[255];
} RxStruct;
RxStruct receivedData;

// Create a struct_message called myData


// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  int byteCount = len;
  memcpy(&receivedData, incomingData, byteCount);
//  Serial.println(receivedData.buffer);
  Serial.write(incomingData, byteCount);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
//  SerialPort.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
