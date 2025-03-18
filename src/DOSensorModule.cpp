
#include "DOSensorModule.h"


// Construct the Yosemitech modbus instance
yosemitech sensor;
bool success=0 ;
// Define pin number variables
#define PwrPin  4    // The pin sending power to the sensor *AND* RS485 adapter
#define DEREPin -1  // The pin controlling Recieve Enable and Driver Enable

#define SSRxPin  3  // Recieve pin for software serial (Rx on RS485 adapter)
#define SSTxPin  2  // Send pin for software serial (Tx on RS485 adapter)

// Define the sensor type
yosemitechModel model = Y504;  // The sensor model number
// Define the sensor's modbus address
byte modbusAddress = 0x01;  // The sensor's modbus address, or SlaveID
// Yosemitech ships sensors with a default ID of 0x01.

// Construct software serial object for Modbus
SoftwareSerial modbusSerial(SSRxPin, SSTxPin);


// YosemitechModbus sensor;

void initializeDOSensors() {
   

    //modbusSerial.begin(9600); // The modbus serial stream - Baud rate MUST be 9600.

    // Start up the sensor
    sensor.begin(model, modbusAddress, &Serial1, DEREPin);

    // Turn on debugging
    sensor.setDebugStream(&Serial);
    //
    //  // Start up note
    Serial.println("Yosemitech ");
    Serial.println(sensor.getModel());
    Serial.println(sensor.getParameter());
    Serial.println("Sensor");

    float hardwareV, softwareV;
    sensor.getVersion(hardwareV, softwareV);
    Serial.println("Hardware Version:");
    Serial.println(hardwareV);
    Serial.println("Software Version:");
    Serial.println(softwareV);

    //  // Get the sensor serial number
    String SN = sensor.getSerialNumber();
    Serial.println("Serial Number:");
    Serial.print(SN);
    delay(3000);

    
    // Get the sensor's current calibration values
    if (model != Y532)
    {
        float Kval = 0;
        float Bval = 0;
        sensor.getCalibration(Kval, Bval);
        Serial.println("Current Calibration Equation:");
        Serial.print(Kval);
        Serial.print("*raw + ");
        Serial.println(Bval);
        delay(3000);
    }

    

    Serial.println("Starting sensor measurements");
    while (success==0)
    {
        success = sensor.startMeasurement();
        if (success)
            Serial.println("    Measurements started.");
        else
            Serial.print("    Failed to start measuring!");
            Serial.println(success);
    }
    Serial.println("Allowing sensor to stabilize..");
    //
   
    

}

SensorData readSensorData() {
    SensorData data;
    // send the command to get the values
    data.DO_doValue = -9999;         //oC
    data.DO_tempValue = -9999;       //mg/L
    data.DO_doPercentValue = -9999;  //%
    // log data from sensor
    
    modbusSerial.begin(9600); // The modbus serial stream - Baud rate MUST be 9600.
    // Start up the sensor
    
   while (success==0)
    {
        success = sensor.startMeasurement();
        if (success)
            Serial.println("    Measurements started.");
        else
            Serial.print("    Failed to start measuring!");
            Serial.println(success);
    }
    sensor.getValues(data.DO_doPercentValue, data.DO_tempValue, data.DO_doValue);
    sensor.stopMeasurement();
  return data;
}
