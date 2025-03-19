#include "OLEDModule.h"
#define I2C_SDA 8
#define I2C_SCL 9


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static void clearDisplay();
void initializeOLED() {
   Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Initialize with the I2C addr 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }
  display.display();
   

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);     // Start at top-left corner
}

void displaySensorData(SensorData data) {

    
    //DO  value
    //display.clearDisplay();
    clearDisplay();
    display.setTextSize(1);    display.setCursor(0, 0);    display.print("DO Value: ");
    display.setTextSize(1);    display.setCursor(0, 10);    display.print(data.DO_doValue);    display.print(" mg/l, ");

    // display Temperature
    display.print(data.DO_tempValue);    display.print(" ");
    display.setTextSize(1);    display.cp437(true);    display.write(167);
    display.setTextSize(1);    display.print("C");
    //EC value
    display.setTextSize(1);    display.setCursor(0, 25);    display.print("EC Value: ");
    display.setTextSize(1);    display.setCursor(0, 35);    display.print(data.EC_ec);    display.print(" uS/cm, ");
    display.print(data.EC_temp);    display.print(" ");
    display.setTextSize(1);    display.cp437(true);    display.write(167);
    display.setTextSize(1);    display.print("C");

    //display.setTextSize(1);    display.setCursor(0, 55);    display.print("Lecturer: Dr. Hien Vo");
    display.display();
}

float  displayBatteryVoltage(uint16_t adc_value)
{
    float batteryVoltage = 0.0021*adc_value+0.3677;
    clearLine(45);
    display.setTextSize(1);     display.setCursor(0, 45);    display.print("Voltage: "); 
    display.print(batteryVoltage);   display.print(" V");
    display.display();
    return batteryVoltage;
}

static void clearDisplay()
{
    display.setTextSize(1);    display.setCursor(0, 0);
    display.print("                           ");
    display.setTextSize(1);    display.setCursor(0, 10);
    display.print("                           ");
    display.setTextSize(1);    display.setCursor(0, 25);
    display.print("                           ");
    display.setTextSize(1);    display.setCursor(0, 35);
    display.print("                           ");
    
    
    
}
void clearLine (uint8_t line)
{
    display.fillRect(0, line, SCREEN_WIDTH, 8, SSD1306_BLACK);
}