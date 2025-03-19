#ifndef DATAHUB_WISEPASS_H
#define DATAHUB_WISEPASS_H
#define SIM_RX 7
#define SIM_TX 6
#define SIM_COM_POWER_PIN 1

#define SERVICE_HOST "tcp://rabbitmq-pub.education.wise-paas.com:1883"
#define USERNAME "7LeD0ox8MZeY:2VQ66jwX6nQZ"
#define PASSWORD "nBoyDSRbbRsyX2S2yQbt"
#define TOPIC2PUBLISH "/wisepaas/scada/14b290c2-d759-4247-ae6e-d55e64656aba/data" 

#include "WString.h"
#include "HardwareSerial.h"
#include <ctype.h>
#include "WifiModule.h"
#include "OLEDModule.h"
void WISE_initSimCOM();


//void WISE_sendATCommand(String command);
String WISE_sendATCommand(const char* command, bool isReturn);
void WISE_MQTT_upload(const SensorData& data);
static String extractString(String source, String startMarker, String endMarker);
static void checkCSQ();
static void checkCREG();
static void givenPayload(char* payload,  const SensorData& data);
void WISE_turnOffSIMCom();
void WISE_turnOnSIMCom();
#endif