#ifndef DO_SENSOR_MODULE_H
#define DO_SENSOR_MODULE_H
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <YosemitechModbus.h>  // Assuming use of a specific sensor library

typedef struct {
    float DO_doValue;
    float DO_tempValue;
    float DO_doPercentValue;
     
}SensorData;

void initializeDOSensors();

SensorData readSensorData();


#endif