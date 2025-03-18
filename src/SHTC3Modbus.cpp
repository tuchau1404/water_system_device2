/*!
 *  @file SHTC3Modbus.h
 *
 *  This library is for the "SHTC3 RS485 Modbus RTU" series of sensors.
 *  These sensors communicate via RS485 standard.
 *  So you will need an "RS485 to UART (TTL) communication converter circuit" for use with Microcontrollers.
 *  Like, UNO board, MEGA board, ...
 *
 *  Wiring diagram of SHT3C with "Communication Converter Circuit":
 *  V+      : BROWN         - Power supply 5~28VDC
 *  V-      : BLACK         - Power supply 0VDC (Mass)
 *  RS485-A : YELLOW/GREEN  - Signal wire A
 *  RS485-B : BLUE          - Signal wire B
 *
 *  @author TrungKuro (Hshop)
 */

 #include "SHTC3Modbus.h"
 #include "ModbusCRC.h"
 
 /* ------------------------------ Constructor ------------------------------ */
 
 SHTC3::SHTC3(uint8_t baud, uint8_t addr)
 {
   port = &Serial1;
   typeSerial = HARD_SERIAL;
 
   _baud = baud;
   _addr = addr;
 }
 
 SHTC3::SHTC3(uint8_t rxPin, uint8_t txPin, uint8_t baud, uint8_t addr)
 {
   SoftwareSerial *ss = new SoftwareSerial(rxPin, txPin);
   port = ss;
   typeSerial = SOFT_SERIAL;
 
   _baud = baud;
   _addr = addr;
 }
 
 /* ----------------------------- Initialization ---------------------------- */
 
 void SHTC3::begin(uint16_t baud)
 {
   switch (baud)
   {
   case 2400:
     _baud = BAUD_2400;
     break;
   case 4800:
     _baud = BAUD_4800;
     break;
   case 9600:
     _baud = BAUD_9600;
     break;
   default:
     _baud = INVALID_BAUD;
   }
 
   if (typeSerial == HARD_SERIAL)
   {
     HardwareSerial *hs = (HardwareSerial *)port;
     hs->begin(baud);
   }
   else if (typeSerial == SOFT_SERIAL)
   {
     SoftwareSerial *ss = (SoftwareSerial *)port;
     ss->begin(baud);
   }
 }
 
 void SHTC3::setTimeout(uint16_t timeOut)
 {
   if (timeOut < MIN_SHTC3_TIMEOUT)
   {
     _timeOut = MIN_SHTC3_TIMEOUT;
   }
   else if (timeOut > MAX_SHTC3_TIMEOUT)
   {
     _timeOut = MAX_SHTC3_TIMEOUT;
   }
   else
     _timeOut = timeOut;
 }
 
 /* --------------------------------- Struct -------------------------------- */
 
 dataSHTC3 SHTC3::getData()
 {
   dataSHTC3 value;
 
   /* Remove all previous junk data (if have) */
   while (port->available())
   {
     port->read();
   }
 
   /* Send temperature & humidity reading command
   **
   ** 0x xx 03 00 00 00 02 xx xx
   */
   getValue[0] = _addr;
   uint16_t crc = checkModbusCRC(getValue, 6);
   getValue[6] = crc & 0xFF;
   getValue[7] = crc >> 8;
   port->write(getValue, sizeof(getValue));
 
   unsigned long now = millis();
   bool complete = false;
 
   /* Number of bytes to respond
   **
   ** Addr : 1 Byte
   ** Cmd  : 1 Byte (0x03)
   ** Resp : 1 Byte (0x04)
   ** Temp : 2 Byte [H_Temp] [L_Temp]
   ** Humi : 2 Byte [H_Humi] [L_Humi]
   ** Crc  : 2 Byte [L_CRC]  [H_CRC]
   */
   uint8_t myBuf[9];
 
   while ((millis() - now) < _timeOut)
   {
     if (port->available())
     {
       port->readBytes(myBuf, 9);
       complete = true;
       break;
     }
   }
 
   if (complete)
   {
     /* Check CRC */
     if (checkModbusCRC(myBuf, 9) == 0)
     {
       value.temperatureC = (myBuf[5] << 8 | myBuf[6]);
       if (value.temperatureC >= 32768.0) // Negative value
       {
         value.temperatureC = value.temperatureC - 65536.0;
       }
       value.temperatureC = value.temperatureC / 10.0;
       value.temperatureF = value.temperatureC * 1.8 + 32.0;
       value.humidity = (myBuf[3] << 8 | myBuf[4]) / 10.0;
     }
     else
     {
       DEBUG_PRINTLN(F("Error CRC in Get Data"));
       value.temperatureC = 0.0;
       value.temperatureF = 0.0;
       value.humidity = 0.0;
     }
   }
   else
   {
     DEBUG_PRINTLN(F("Timeout when Get Data"));
     value.temperatureC = 0.0;
     value.temperatureF = 0.0;
     value.humidity = 0.0;
   }
 
   return value;
 }
 
 /* ------------------------------- Read Data ------------------------------- */
 
 float SHTC3::readTemperature(bool isDegreeCelsius)
 {
   dataSHTC3 value = getData();
 
   if (isDegreeCelsius)
   {
     return value.temperatureC;
   }
   else
   {
     return value.temperatureF;
   }
 }
 
 float SHTC3::readHumidity()
 {
   dataSHTC3 value = getData();
 
   return value.humidity;
 }
 
 /* --------------------------- Read Configuration -------------------------- */
 
 uint16_t SHTC3::readBaudrate()
 {
   uint16_t value;
 
   /* Remove all previous junk data (if have) */
   while (port->available())
   {
     port->read();
   }
 
   /* Send baudrate reading command
   **
   ** 0x xx 03 07 D1 00 01 xx xx
   */
   getBaud[0] = _addr;
   uint16_t crc = checkModbusCRC(getBaud, 6);
   getBaud[6] = crc & 0xFF;
   getBaud[7] = crc >> 8;
   port->write(getBaud, sizeof(getBaud));
 
   unsigned long now = millis();
   bool complete = false;
 
   /* Number of bytes to respond
   **
   ** Addr : 1 Byte
   ** Cmd  : 1 Byte (0x03)
   ** Resp : 1 Byte (0x02)
   ** Data : 2 Byte [H_Data] [L_Data]
   ** Crc  : 2 Byte [L_CRC]  [H_CRC]
   */
   uint8_t myBuf[7];
 
   while ((millis() - now) < _timeOut)
   {
     if (port->available())
     {
       port->readBytes(myBuf, 7);
       complete = true;
       break;
     }
   }
 
   if (complete)
   {
     /* Check CRC */
     if (checkModbusCRC(myBuf, 7) == 0)
     {
       switch (myBuf[4])
       {
       case 0:
         _baud = BAUD_2400;
         value = 2400;
         break;
       case 1:
         _baud = BAUD_4800;
         value = 4800;
         break;
       case 2:
         _baud = BAUD_9600;
         value = 9600;
         break;
       default:
         _baud = INVALID_BAUD;
         value = 0;
       }
     }
     else
     {
       DEBUG_PRINTLN(F("Error CRC in Get Baud"));
       value = 0;
     }
   }
   else
   {
     DEBUG_PRINTLN(F("Timeout when Get Baud"));
     value = 0;
   }
 
   return value;
 }
 
 uint8_t SHTC3::readAddress()
 {
   uint8_t value;
 
   /* Remove all previous junk data (if have) */
   while (port->available())
   {
     port->read();
   }
 
   /* Send address reading command
   **
   ** 0x FF 03 07 D0 00 01 91 59
   */
   port->write(getAddr, sizeof(getAddr));
 
   unsigned long now = millis();
   bool complete = false;
 
   /* Number of bytes to respond
   **
   ** Addr : 1 Byte
   ** Cmd  : 1 Byte (0x03)
   ** Resp : 1 Byte (0x02)
   ** Data : 2 Byte [H_Data] [L_Data]
   ** Crc  : 2 Byte [L_CRC]  [H_CRC]
   */
   uint8_t myBuf[7];
 
   while ((millis() - now) < _timeOut)
   {
     if (port->available())
     {
       port->readBytes(myBuf, 7);
       complete = true;
       break;
     }
   }
 
   if (complete)
   {
     /* Check CRC */
     if (checkModbusCRC(myBuf, 7) == 0)
     {
       _addr = myBuf[4];
       value = _addr;
     }
     else
     {
       DEBUG_PRINTLN(F("Error CRC in Get Addr"));
       value = 0;
     }
   }
   else
   {
     DEBUG_PRINTLN(F("Timeout when Get Addr"));
     value = 0;
   }
 
   return value;
 }
 
 /* ------------------------- Configuration Settings ------------------------ */
 
 bool SHTC3::setBaudrate(uint8_t baud)
 {
   /* Remove all previous junk data (if have) */
   while (port->available())
   {
     port->read();
   }
 
   /* Send install command baudrate
   **
   ** 0x xx 06 07 D1 xx xx xx xx
   */
   setBaud[0] = _addr;
   setBaud[5] = baud;
   uint16_t crc = checkModbusCRC(setBaud, 6);
   setBaud[6] = crc & 0xFF;
   setBaud[7] = crc >> 8;
   port->write(setBaud, sizeof(setBaud));
 
   unsigned long now = millis();
   bool complete = false;
 
   uint8_t myBuf[8];
 
   while ((millis() - now) < _timeOut)
   {
     if (port->available())
     {
       port->readBytes(myBuf, 8);
       complete = true;
       break;
     }
   }
 
   if (complete)
   {
     /* Check CRC */
     if (checkModbusCRC(myBuf, 8) == 0)
     {
       _baud = myBuf[5];
       return true;
     }
     else
     {
       DEBUG_PRINTLN(F("Error CRC in Set Baud"));
       return false;
     }
   }
   else
   {
     DEBUG_PRINTLN(F("Timeout when Set Baud"));
     return false;
   }
 }
 
 bool SHTC3::setAddress(uint8_t addr)
 {
   /* Remove all previous junk data (if have) */
   while (port->available())
   {
     port->read();
   }
 
   /* Send install command address
   **
   ** 0x xx 06 07 D0 xx xx xx xx
   */
   setAddr[0] = _addr;
   setAddr[5] = addr;
   uint16_t crc = checkModbusCRC(setAddr, 6);
   setAddr[6] = crc & 0xFF;
   setAddr[7] = crc >> 8;
   port->write(setAddr, sizeof(setAddr));
 
   unsigned long now = millis();
   bool complete = false;
 
   uint8_t myBuf[8];
 
   while ((millis() - now) < _timeOut)
   {
     if (port->available())
     {
       port->readBytes(myBuf, 8);
       complete = true;
       break;
     }
   }
 
   if (complete)
   {
     /* Check CRC */
     if (checkModbusCRC(myBuf, 8) == 0)
     {
       _addr = myBuf[5];
       return true;
     }
     else
     {
       DEBUG_PRINTLN(F("Error CRC in Set Addr"));
       return false;
     }
   }
   else
   {
     DEBUG_PRINTLN(F("Timeout when Set Addr"));
     return false;
   }
 }