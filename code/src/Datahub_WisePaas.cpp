#include <Datahub_WisePaas.h>







 
HardwareSerial WISE_simSerial(1);

//String httpResponse = ""; 
char buffer[128];
void WISE_initSimCOM()
{
    
    
    WISE_simSerial.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);
    WISE_sendATCommand("AT+CPOF",0);
    
    delay(10000);
   
   
}




void WISE_MQTT_upload(const SensorData& data)

{
    //set up oled  
    clearLine(55);
    display.setTextSize(1); 
    display.setCursor(0,55);    

    //cmqttconnect = String("AT+CMQTTCONNECT=") + String("0,\"") + String(SERVICE_HOST) + String("\",10,1,\"") + String(USERNAME) + "\",\"" + String(PASSWORD) + "\"";
    char cmqttconnect[] =    "AT+CMQTTCONNECT=0,\"tcp://rabbitmq-pub.education.wise-paas.com:1883\",10,1,\"7LeD0ox8MZeY:2VQ66jwX6nQZ\",\"nBoyDSRbbRsyX2S2yQbt\"";
   
    char topic []= TOPIC2PUBLISH;
    uint8_t len_topic = sizeof(topic)-1;
   
    
    char  payload[150];
 
    givenPayload( payload, data);
    uint8_t len_payload = strlen(payload);
    char cmqtttopic[50];
    sprintf(cmqtttopic,"AT+CMQTTTOPIC=0,%d",len_topic);
    Serial.println("Payload:------------------");
    Serial.println(payload);
    Serial.println(cmqtttopic);
     char temp[50];
 
    sprintf(temp,"AT+CMQTTPAYLOAD=0,%d",len_payload);
    Serial.println(temp);
   

   //Start 
    WISE_sendATCommand("AT+CPIN?",0);
    checkCSQ();
    checkCREG();
    WISE_sendATCommand("AT+CMQTTSTART",0);
    WISE_sendATCommand("AT+CMQTTACCQ=0,\"a1ccq\",0",0);
    WISE_sendATCommand(cmqttconnect,0);
    for (int i =0; i<3;i++)
    {
        // data may be lost

        WISE_sendATCommand(cmqtttopic,0);
        WISE_sendATCommand(topic,0);
        WISE_sendATCommand(temp,0);
    
        WISE_sendATCommand(payload,0);
        WISE_sendATCommand("AT+CMQTTPUB=0,0,1",0);
    }
    WISE_sendATCommand("AT+CMQTTDISC=0,1",0);
    WISE_sendATCommand("AT+CMQTTREL=0",0);
    WISE_sendATCommand("AT+CMQTTSTOP",0);

    
}


    
    


static String extractString(String source, String startMarker, String endMarker) {
  int startIndex = source.indexOf(startMarker);
  if (startIndex == -1) return "";  // Start marker not found

  startIndex += startMarker.length();  // Move to end of start marker
  int endIndex = source.indexOf(endMarker, startIndex);
  if (endIndex == -1) return "";  // End marker not found

  return source.substring(startIndex, endIndex);
}
static void checkCSQ()
{
    //check Signal Quality
    while(1)
    {
        String data = WISE_sendATCommand("AT+CSQ",1);
        String rssi = extractString(data, "CSQ: ",",");
        uint8_t num_rssi;
        

    
        num_rssi = rssi.toInt();  // Convert to int
        if (num_rssi !=99)
        {
        
            break;
        }
        Serial.println("Bad signalquality");
        delay(1000);
    }
    
    
}
static void checkCREG()
{
    uint8_t reconnectCount=0;
    while(1)
    {
        String data = WISE_sendATCommand("AT+CREG?",1);
    
        String status = extractString(data, "CREG: 0,","\n");
        uint8_t num_status=0;
        num_status = status.toInt();  // Convert to int
        Serial.print(status);
        Serial.println(num_status);
        if (num_status ==1)
        {
       
            break;
        }
        Serial.println("Not registered");
        if (reconnectCount==10)
        {
            WISE_turnOffSIMCom();
            clearLine(55);
            display.setTextSize(1); 
            display.setCursor(0,55);
            display.print("Going to sleep now...");
            display.display();
            Serial.println("Going to sleep now...");
            Serial.end();
    
            esp_deep_sleep_start();

        }
        reconnectCount++;

    delay(1000);
    }

}
static void givenPayload( char* te_payload,const SensorData& data)
{
    //convert data to Payload
    sprintf(te_payload,"{\"d\":{\"Device1\":{\"EC_value\":%d,\"EC_temperature\":%.2f,\"DO_value\":%.2f,\"DO_temperature\":%.2f,\"Voltage\":%.2f}}}",
            data.EC_ec,data.EC_temp,data.DO_doValue,data.DO_tempValue,data.BatteryVoltage);
   
    
}



String WISE_sendATCommand(const char* command, bool isReturn) {
    clearLine(55);
    display.setTextSize(1); 
    display.setCursor(0,55);
    display.print(command);
    display.display();
    Serial.print("Command:      ");
    Serial.println(command);
    WISE_simSerial.println(command);   // Send AT command to SIMCom
    delay(1000);           // Wait for response;



    String incomingData="";
    for (int i =0;i<3;i++)
    {
        //Make sure there is not data in receiver buffer
        while (WISE_simSerial.available()) {
            
            int bytesRead = WISE_simSerial.readBytes(buffer, sizeof(buffer));  // Read bytes
            Serial.write(buffer, bytesRead);
            
            buffer[bytesRead] = '\0';         // Print response to Serial Monitor
            if (isReturn==1)
            {
                incomingData.concat(buffer);
            }
            //i=i+1;
        
        
        }
        delay(100);
    }
    return incomingData;

    
    
  
    
    Serial.println("__________________________________");
             // Newline after each command response
    
}
void WISE_turnOffSIMCom()
{
    WISE_sendATCommand("AT+CPOF",0);
    delay(500);
    pinMode(SIM_COM_POWER_PIN, INPUT);

}
void WISE_turnOnSIMCom()
{
    pinMode(SIM_COM_POWER_PIN, OUTPUT);
    digitalWrite(SIM_COM_POWER_PIN,LOW);
}