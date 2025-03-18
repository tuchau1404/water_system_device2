#include "SDCardModule.h"
#include "OLEDModule.h"

// Pin definitions for SPI (SD card)
#define SD_CS 5  // Chip Select pin for SD card
#define SD_SCK 10 // SCK pin for SPI
#define SD_MOSI 18 // MOSI pin for SPI
#define SD_MISO 19 // MISO pin for SPI
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SPIClass spi(FSPI); 
void initializeSDCard() {
  // Config SD Card
 
    spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS,spi))  {
    Serial.println("Card Mount Failed");
    //WARNING
    display.clearDisplay();
    display.setTextSize(1);    display.setCursor(0, 0);    display.print("Mount Fail");
    display.display();
    return;
  }
  display.clearDisplay();
    display.setTextSize(1);    display.setCursor(0, 0);    display.print("Mount OK");
    display.display();
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)  {
    Serial.println("MMC");
  }  else if (cardType == CARD_SD)  {
    Serial.println("SDSC");
  }  else if (cardType == CARD_SDHC)  {
    Serial.println("SDHC");
  }  else  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}


void checkAndCreateFile(const char* fileName) {
    if (!SD.exists(fileName)) {
        File dataFile = SD.open(fileName, FILE_WRITE);
        if (dataFile) {
            dataFile.println("Date,Time,DOValue,TempValue");
            dataFile.close();
            Serial.println("New file created !");
        } else {
            Serial.println("Failed to create a new file.");
        }
    }
}

void appendToFile(const char* fileName, const char* data) {
    File file = SD.open(fileName, FILE_APPEND);
    if (file) {
        file.println(data);
        Serial.println("Data appended to file.");
    } else {
        Serial.println("Failed to open file for appending.");
    }
    file.close();
}


