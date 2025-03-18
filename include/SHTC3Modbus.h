
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

 #ifndef __SHTC3_MODBUS_H__
 #define __SHTC3_MODBUS_H__
 
 #include <Arduino.h>
 #include <SoftwareSerial.h>
 
 /* -------------- DEBUG (uncomment to open the Debug function) ------------- */
 
 // #define ENABLE_DEBUG
 
 #if defined(ENABLE_DEBUG)
 #define Debug Serial
 #define DEBUG_PRINT(...) Debug.print(__VA_ARGS__)
 #define DEBUG_PRINTLN(...) Debug.println(__VA_ARGS__)
 #else
 #define DEBUG_PRINT(...)
 #define DEBUG_PRINTLN(...)
 #endif
 
 /* ------------------------------------------------------------------------- */
 
 // #define CMD_READ 0x03  // Command Read
 // #define CMD_WRITE 0x06 // Command Write
 
 // #define REG_HUMI 0x0000 // Temperature (Only-Read)
 // #define REG_TEMP 0x0001 // Humidity (Only-Read)
 
 // #define REG_CODE_ADDR 0x07D0 // Default: Address 1 (0x01)
 // #define REG_CODE_BAUD 0x07D1 // Default: Baudrate 4800 bit/s (0x01)
 
 #define BAUD_2400 0x00
 #define BAUD_4800 0x01
 #define BAUD_9600 0x02
 #define INVALID_BAUD 0xFF
 
 #define MAX_SHTC3_TIMEOUT (20000) // Unit (ms)
 #define MIN_SHTC3_TIMEOUT (100)   // Unit (ms)
 
 /* ------------------------------------------------------------------------- */
 
 enum
 {
   HARD_SERIAL,
   SOFT_SERIAL
 };
 
 typedef struct dataSHTC3
 {
   float temperatureC;
   float temperatureF;
   float humidity;
 };
 
 /* ------------------------------------------------------------------------- */
 
 class SHTC3
 {
 private:
   /**
    * Command frame format structure:
    *
    * Address  : 1 Byte
    * Command  : 1 Byte
    * Register : 2 Byte
    * Data     : 2 Byte
    * L_CRC    : 1 Byte
    * H_CRC    : 1 Byte
    */
   uint8_t getValue[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00}; // 0x xx 03 00 00 00 02 xx xx
   uint8_t setBaud[8] = {0x00, 0x06, 0x07, 0xD1, 0x00, 0x00, 0x00, 0x00};  // 0x xx 06 07 D1 xx xx xx xx
   uint8_t getBaud[8] = {0x00, 0x03, 0x07, 0xD1, 0x00, 0x01, 0x00, 0x00};  // 0x xx 03 07 D1 00 01 xx xx
   uint8_t setAddr[8] = {0x00, 0x06, 0x07, 0xD0, 0x00, 0x00, 0x00, 0x00};  // 0x xx 06 07 D0 xx xx xx xx
   uint8_t getAddr[8] = {0xFF, 0x03, 0x07, 0xD0, 0x00, 0x01, 0x91, 0x59};  // 0x FF 03 07 D0 00 01 91 59
 
   Stream *port;
   uint8_t typeSerial;
 
   uint8_t _baud;
   uint8_t _addr;
   uint16_t _timeOut = MIN_SHTC3_TIMEOUT;
 
 public:
   SHTC3(uint8_t baud = BAUD_4800, uint8_t addr = 0x01);                                       // Constructor - HardwareSerial (RX=0) (TX=1)
   SHTC3(uint8_t rxPin = 2, uint8_t txPin = 3, uint8_t baud = BAUD_4800, uint8_t addr = 0x01); // Constructor - SoftwareSerial (RX=2) (TX=3)
   virtual ~SHTC3() { delete port; }                                                           // Virtual Destructor
 
   /* Initialization */
   void begin(uint16_t baud);
   void setTimeout(uint16_t timeOut);
 
   /* Struct */
   dataSHTC3 getData();
 
   /* Read Data */
   float readTemperature(bool isDegreeCelsius = true);
   float readHumidity();
 
   /* Read Configuration */
   uint16_t readBaudrate();
   uint8_t readAddress();
 
   /* Configuration Settings (return "TRUE" when success) */
   bool setBaudrate(uint8_t baud);
   bool setAddress(uint8_t addr);
 };
 
 /* ------------------------------------------------------------------------- */
 
 #endif
 