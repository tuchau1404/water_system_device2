#include "ECTDS10_Example.h"


ectds10 ecSensor;
SoftwareSerial ectds10Modbus (RxPin, TxPin);
byte ecSlaveAddress= 0x03;
void ectds10_run()
{
    Serial.begin(9600);
    ectds10Modbus.begin(9600);
     
    bool success = ecSensor.begin(ecSlaveAddress, &ectds10Modbus);
    Serial.print("Start: ");
    Serial.println(success);
    float ec_temp;
    uint16_t ec_salanity,ec_ec,ec_tds;
   
    while(1)
    {
        bool success =ecSensor.getValues(ec_temp,ec_ec,ec_salanity,ec_tds);
        Serial.print("ec_temp: ");
        Serial.println(ec_temp);
        Serial.print("ec_ec: ");
        Serial.println(ec_ec);
        Serial.print("ec_salanity: ");
        Serial.println(ec_salanity);
        Serial.print("ec_tds: ");
        Serial.println(ec_tds);
        Serial.println("------------------------------");
        Serial.println();

        delay(1000);
    }
    

}