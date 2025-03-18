
/**
 * @file main.cpp
 * @author Bùi Ngọc Long
 * @date 2024-04-17
 * @brief Short description of what this file does.
 *
 * Long description if needed...
 * 
 * @license MIT License
 */

#include <Arduino.h>



#include "Datahub_WisePaas.h"
#include "YosemitechModbus.h"
#include "WifiModule.h"

#include "ECTDS10_Example.h"
#include "SIMComModule.h"
#include "OLEDModule.h"
#include <esp_sleep.h>
#include "SDCardModule.h"
#include <RTClib.h>
#include <Wire.h>
#define uS_TO_S_FACTOR 1000000  // Conversion factor for microseconds to seconds
#define TIME_TO_SLEEP  600    // Time ESP32 will sleep (in seconds)
#include "SHTC3Modbus.h"

const long interval = 60000;  // ms

RTC_DS3231  rtc;

SensorData shared_data;
#define RX_SHT 3
#define TX_SHT 2
SHTC3 demo(RX_SHT, TX_SHT, BAUD_4800, 0x01);


void DOSensor();
void ECSensor();
void RTCandSD();
void WIFI();
void OLED();
void BatteryShow();
void SIMCom();
void DataHubUpload();
void ThingSpeakUpload();
void runTasks();
void setupDeepSleep();
void executeDeepSleep();
void SHTC3Sensor();

char test_str[50];
void setup() {
    WISE_turnOnSIMCom();
    setupDeepSleep();
    Serial.begin(115200);
    initializeOLED();
    BatteryShow();
    ECSensor();
    OLED();
    ThingSpeakUpload();
    //DataHubUpload();
    executeDeepSleep();

}


void loop() 
{
  
   
}





void ECSensor()
{
    ectds10 ecSensor;
   
    delay(100);
    HardwareSerial modbus_Serial(1);  
    modbus_Serial.begin(9600,SERIAL_8N1,3,2);

    float ec_temp=0;
    uint16_t ec_salanity=0,ec_ec=0,ec_tds=0;

    bool success = ecSensor.begin(0x03, &modbus_Serial);

    ecSensor.getValues(ec_temp,ec_ec,ec_salanity,ec_tds);
    Serial.print("EC Value: ");    Serial.print(ec_ec); Serial.println(" uS/cm");
    Serial.print("EC Temperature Value: ");    Serial.print(ec_temp); Serial.println(" oC");
    Serial.println("------------------------");
    delay(1000);

    modbus_Serial.end();
    shared_data.EC_ec       = ec_ec;
    shared_data.EC_temp     = ec_temp;
    shared_data.EC_salanity = ec_salanity;
    shared_data.EC_tds      = ec_tds;
}

void DataHubUpload()
{
    Serial.println("EC_sensor read done");
    WISE_initSimCOM();
    WISE_MQTT_upload(shared_data);
    WISE_turnOffSIMCom();
}
void ThingSpeakUpload()
{
    runSIMCom();
    sendDataToThingSpeak(shared_data);
}

void OLED()
{
    displaySensorData(shared_data);

}


void BatteryShow()
{          
    uint16_t adcReading=0;
    adcReading= analogRead(4);
    shared_data.BatteryVoltage= displayBatteryVoltage(adcReading);
    Serial.println(adcReading);
}


void setupDeepSleep() {
    // Set the wakeup time
    esp_sleep_enable_timer_wakeup((uint64_t)TIME_TO_SLEEP * uS_TO_S_FACTOR);
}


void executeDeepSleep()
{
    clearLine(55);
    display.setTextSize(1); 
    display.setCursor(0,55);
    display.print("Going to sleep now...");
    display.display();
    Serial.println("Going to sleep now...");
    Serial.end();
    
    esp_deep_sleep_start();


}

void SHTC3Sensor()
{
    //OLED must be initialized before function is called 

}
