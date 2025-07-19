#include <MPU9250_asukiaaa.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ;
uint8_t broadcastAddress[] = {0x3c,0x8a,0x1f,0xb3,0x81,0xc4}; //Make sure the MAC address is in the format 0xAA, 0xBB etc

typedef struct struct_message {
  float aX,aY,aZ,gX,gY,gZ;
} struct_message;
struct_message myData;
esp_now_peer_info peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK){
    Serial.print("ERRORERRORERRORERRORERRORERRORERRORERRORERRORERRORERRORERROR");
    return;
  }
esp_now_register_send_cb(OnDataSent);
memcpy(peerInfo.peer_addr, broadcastAddress, 6);
peerInfo.channel = 0;
peerInfo.encrypt = false;
if (esp_now_add_peer(&peerInfo) != ESP_OK){
  Serial.println("Failed to add peer");
  return;
 }
#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif
  mySensor.beginAccel();
  mySensor.beginGyro();
}

void loop() {
  int result;

  result = mySensor.accelUpdate();
  if (result == 0) {
    aX = mySensor.accelX() * 9.81;
    aY = mySensor.accelY() * 9.81;
    aZ = mySensor.accelZ() * 9.81;
    aSqrt = mySensor.accelSqrt();
    Serial.println("accelX: " + String(aX));
    Serial.println("accelY: " + String(aY));
    Serial.println("accelZ: " + String(aZ));
    Serial.println("accelSqrt: " + String(aSqrt));
  } else {
    Serial.println("No accel val   " + String(result));
  }

  result = mySensor.gyroUpdate();
  if (result == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.println("gyroX: " + String(gX));
    Serial.println("gyroY: " + String(gY));
    Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values " + String(result));
  }
  myData.aX = aX;
  myData.aY = aY;
  myData.aZ = aZ;
  myData.gX = gX;
  myData.gY = gY;
  myData.gZ = gZ;
  result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK){
    Serial.println("SENTSENTSENTSENT YAAAY");
  }
  else {
    Serial.println("ERRRORRRRERERROOERAEORJAEORIUAEJ");
  }
  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line
  delay(500);
}
