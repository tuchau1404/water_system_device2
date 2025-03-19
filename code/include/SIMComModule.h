#ifndef SIMComModuleTc_H //if not define
#define SIMComModuleTc_H
#define TINY_GSM_MODEM_SIM7600       // Modem is SIM7600  TINY_GSM_MODEM_SIM7600g
#include <TinyGsmClient.h> 
#include <ThingSpeak.h>
#include "OLEDModule.h"

// gsm pins

#define MODEM_TX        6
#define MODEM_RX        7

//#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb


void initializeSIMCom();
void runSIMCom();
#endif
