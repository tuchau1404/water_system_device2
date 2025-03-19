#include "WiFiModule.h"
#include <WiFi.h>
#include <ThingSpeak.h>

// // Config Wifi
// const char* ssid = "VGU_AQI_1";
// const char* password = "myPMS5003";

const char* ssid = "Hoangdeptrai2k4_24";
const char* password = "07036525";

// const char* ssid = "LongBui";
// const char* password = "12345678";


//Config Thingspeak
//Long Bui
unsigned long channelID = 1;
//Long api key
//const char* apiKey = "O499F31QG9B7BFCC";

//Tu Chau api key
const char* apiKey= "DKEWUVQR7YI17DN6";

WiFiClient client;


void initializeWiFi() {
  //
  WiFi.mode(WIFI_OFF);
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  Serial.print("Connecting to Wi-Fi");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) { //sau 10 giay connect khong duoc thi bo qua
    delay(500);
    Serial.print(".");

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
    }
  }
}

void disconnectWifi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected.");
}

void sendDataToThingSpeak( const SensorData& data) {
  //ThingSpeak.setField(1, data.DO_doValue);
  //ThingSpeak.setField(2, data.DO_tempValue);
  ThingSpeak.setField(3, data.EC_ec);
  ThingSpeak.setField(4, data.EC_temp);
  ThingSpeak.setField(5, data.BatteryVoltage);
  
  delay(200);
  ThingSpeak.writeFields(channelID, apiKey);
  //  int result = ThingSpeak.writeFields(channelID, apiKey);
  // if (result == 200) {
  //   Serial.println("Data sent to ThingSpeak successfully!");
  // } else {
  //   Serial.println("Failed to send data to ThingSpeak. Error code: " + String(result));
  // }
}
