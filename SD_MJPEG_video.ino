
#define MJPEG_FILENAME "/220_30fps.mjpeg"
#define MJPEG_BUFFER_SIZE (220 * 176 * 2 / 4)
#include <WiFi.h>
#include <SD.h>
#include <SD_MMC.h>

#include <Arduino_GFX_Library.h>
#define TFT_BRIGHTNESS 128

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
//#define TFT_RST   4  // Reset pin (could connect to RST pin)
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

//#define SS 0
#define TFT_BL 21

Arduino_HWSPI *bus = new Arduino_HWSPI(TFT_DC /* DC */, TFT_CS /* CS */, TFT_SCLK, TFT_MOSI, TFT_MISO);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST /* RST */, 1 /* rotation */, true);

#include "MjpegClass.h"
static MjpegClass mjpeg;

void setup()
{
  WiFi.mode(WIFI_OFF);
  Serial.begin(115200);

  gfx->begin();
  gfx->fillScreen(OLIVE);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPIClass spi = SPIClass(VSPI);
  if (!SD.begin(SS, spi, 80000000)) /* SPI bus mode */
  //if ((!SD_MMC.begin()) && (!SD_MMC.begin())) /* 4-bit SD bus mode */
  //if ((!SD_MMC.begin("/sdcard", true)) && (!SD_MMC.begin("/sdcard", true))) /* 1-bit SD bus mode */
  {
    Serial.println(F("ERROR: SD card mount failed!"));
    gfx->println(F("ERROR: SD card mount failed!"));
  }
  else
  {
    File vFile = SD.open(MJPEG_FILENAME);
    // File vFile = SD_MMC.open(MJPEG_FILENAME);
    if (!vFile || vFile.isDirectory())
    {
      Serial.println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
      gfx->println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
    }
    else
    {
      uint8_t *mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
      if (!mjpeg_buf)
      {
        Serial.println(F("mjpeg_buf malloc failed!"));
      }
      else
      {
        Serial.println(F("MJPEG video start"));
        mjpeg.setup(vFile, mjpeg_buf, (Arduino_TFT *)gfx, true);
        while (mjpeg.readMjpegBuf())
        {
          mjpeg.drawJpg();
        }
        Serial.println(F("MJPEG video end"));
        vFile.close();
      }
    }
  }
  //delay(60000);
  // gfx->displayOff();
  // esp_deep_sleep_start();
}

void loop()
{
}
