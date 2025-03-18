#ifndef ECTDS10_Modbus_h
#define ECTDS10_Modbus_h

#include <Arduino.h>
#include <SensorModbusMaster.h>

/**
 * @brief The class for communication with ECTDS10 sensor via modbus
 */
class ectds10
{
    
    public:
        /**
         * @brief This function sets up the communication.
         *
         * It should be run during the arduino "setup" function.
         * The "stream" device must be initialized prior to running this.
         * 
         * @param modbusSlaveID The byte identifier of the modbus slave device.
         * @param stream A pointer to the Arduino stream object to communicate with.
         * @return *bool* True if the starting communication was successful, false if not.
         */
        bool begin(byte modbusSlaveID, Stream* stream);
        byte getSlaveID(void);
        bool setSalveID(byte newSlaveID);

        /**
         * @brief Gets value back from the sensor
         * @param tempValue A float to replace with the temparature parameter value from the
         * sensor.
         * @param ecValue A uint16_t to replace with the Electrical Conductivity parameter value from the
         * sensor.
         * @param salinityValue A uint16_t to replace with the Salinity parameter value from the
         * sensor.
         * @param tdsValue A uint16_t to replace with the TDS parameter value from the
         * sensor.
         
         * 
         */
        bool getValues( float& tempValue, uint16_t& ecValue,uint16_t& salinityValue, uint16_t& tdsValue);

    private:

        byte _slaveID;
        modbusMaster modbus;
};
#endif