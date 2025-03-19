#include "SIMComModule.h"

bool modemConnected = false;
// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "internet"; // APN (example: airtelgprs.com)
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = "";


// TinyGSM Client for Internet connection


// TinyGSM Client for Internet connection
unsigned long Channel_Number = 1;
const char * APIKey = "OHCOIIA2D2ETP5DV";  //copy and past your api key of thingspeak
float temperatureC;
uint32_t tm = 0;
HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);
TinyGsmClient clientSIMCom(modem);
void initializeSIMCom()
{

   
    

}

void runSIMCom()
{

    delay(5000);
    
    SerialAT.begin(115200,SERIAL_8N1,MODEM_RX,MODEM_TX);
    
    
    
    modem.restart();
    ThingSpeak.begin(clientSIMCom);  // Initialize ThingSpeak
    uint8_t reconnectCount =0;
    while(1)
    {
        if (!modemConnected) {
                clearLine(55);
                display.setTextSize(1);     display.setCursor(0, 55);    display.print("Failed to restart modem"); 
                display.display();
                Serial.println("Failed to restart modem, delaying 10's and retrying");
            if (!modem.init()) {
                delay(500);   
                return;
            }
            clearLine(55);
            display.setTextSize(1);     display.setCursor(0, 55);    display.print("Waiting for network"); 
            display.display();
            Serial.print(F("Waiting for network..."));
            if (!modem.waitForNetwork()) {
                clearLine(55);
                display.setTextSize(1);     display.setCursor(0, 55);    display.print("Fail"); 
                display.display();
                Serial.println(" fail");
                delay(500);
                return;
            }
            clearLine(55);
            display.setTextSize(1);     display.setCursor(0, 55);    display.print("Network Connected"); 
            display.display();
            Serial.println("Network Connected!");

            clearLine(55);
            display.setTextSize(1);     display.setCursor(0, 55);    display.print("Connecting to ");  //display.print(apn);
            display.display();
            Serial.print(F("Connecting to "));
            Serial.print(apn);
            if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
                clearLine(55);
                display.setTextSize(1);     display.setCursor(0, 55);    display.print("Fail"); 
                display.display();
                Serial.println(" fail");
                delay(500);
                return;
            }
            modemConnected = true;

            clearLine(55);
            display.setTextSize(1);     display.setCursor(0, 55);    display.print("Internet Connected"); 
            display.display();
           
            Serial.println("Internet Connected!");
        }
        if (reconnectCount==10|| modemConnected==true)
        {
            
            delay(1000);
            break;
        }
        reconnectCount++;

        delay(1000);
    }


    
}