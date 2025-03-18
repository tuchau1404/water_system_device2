#include "ECTDS10_Modbus.h"



bool ectds10::begin(byte modbusSlaveID, Stream* stream)
{
    //Give values to variables
    _slaveID =modbusSlaveID;
    //Start up the modbus instace
    //-1:RS485 modbus does not need GPIO for controlling direction
    bool success = modbus.begin(modbusSlaveID,stream, -1);
    return success;


}

/**
 * Slave Response Frame: AA 03 MM VV0 VV1 VV2 .... CCCC
 *      AA      |   1 byte      |   Slave Address,0-255
 *      0x04    |   1 byte      |   Function Code 4
 *      MM      |   1 byte      |   Register Data Byte Count
 *      VV0,VV1 |   2 byte      |   Register Value (High8bits first)
 *      VV2,VV3 |   2 byte      |   Register Value (High8bits first)
 *      ...     |   ...         |   Register Value (High8bits first)
 *      CCCC    |   2 byte      |   CRC CHECKSUM
 * ---------------------------------------------------------    |
 * 
 * Temperature =(FIRST_8BIT*256 + SECOND_8BIT)/100=2070/100=20.70 °C 
 * EC=FIRST_8BIT*256+SECOND_8BIT =5*256+120 =1400 us/cm
 * Salinity = EC*SALINITYCOFF
 * 
 *  
 * ---------------------------------------------------------    |
 * TEMPERATURE                  |   0x0000/0
 * EC-Electrical Conductivity   |   0x0002/2
 * SALINITY                     |   0x0003/3
 * TDS                          |   0x0004/4
 * SALINITYCOFF                 |   0x0023/35
 * SLAVEADDRESS                 |   0x0200/512
 * ---------------------------------------------------------    |
 * 
 * Example: modbus.byteFromFrame(x) of temperature : 
 * (itself address(0x0000)*2)+ 3 --> 3 and 4
 * High8bits first -->Big endian

 */


bool ectds10::getValues( float& tempValue,uint16_t& ecValue, uint16_t& salinityValue, uint16_t& tdsValue)
{

    leFrame fram;
    tempValue       = -9999;
    salinityValue   = -9999;
    tdsValue       = -9999;
    if (modbus.getRegisters(0x04,0x00,5))
    {
       
        
        uint8_t first8BitTemp   = modbus.byteFromFrame(3);
        uint8_t second8BitTemp  = modbus.byteFromFrame(4);
        uint8_t first8BitEC     = modbus.byteFromFrame(7);
        uint8_t second8BitEC    = modbus.byteFromFrame(8);

        // formula for calculating temperature and ec is above
        tempValue       = (first8BitTemp*256 + second8BitTemp)/100;
        ecValue         = (first8BitEC*256 + second8BitEC);
        salinityValue   =  modbus.uint16FromFrame(bigEndian,9);
        tdsValue        =  modbus.uint16FromFrame(bigEndian,11);

        
        return true;
    }
    return false;

    
}