#ifndef WIFI_MODULE_H //if not define
#define WIFI_MODULE_H
#include <stdint.h>


//  Define init fuction for Wifi module



typedef struct 
{
    float DO_doValue;
    float DO_tempValue;
    float DO_doPercentValue;
    float EC_temp;
    uint16_t EC_salanity,EC_ec,EC_tds;
    float BatteryVoltage;
}SensorData;
// Init Wifi function
void initializeWiFi();
void connectWifi();
void disconnectWifi();

// Init Thingspeak function
void sendDataToThingSpeak(const SensorData& data);



#endif