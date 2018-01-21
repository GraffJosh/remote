/*
 * ESP8266 WiFi Analyzer
 * Revise from ESP8266WiFi WiFiScan example.
 * Require ESP8266 board support, Adafruit GFX and ILI9341 library.
 */
// Uncomment only the TFT model you are using
#define ILI9341

//POWER SAVING SETTING
#define SCAN_COUNT_SLEEP 15
#define PNP_PWR_TRANSISTOR

#define LCD_PWR_PIN 16 // For AZSMZ TFT 1.6X

#include "ESP8266WiFi.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>
#include "ILI9341_SPI.h" // Hardware-specific library
#include <MiniGrafx.h>

#define TFT_DC     0 // For AZSMZ TFT 1.6X
#define TFT_CS     2 // For AZSMZ TFT 1.6X

#define T_CS  D3
#define TIRQ  D4

// #define JPG_SS    D2
#define JPG_MOSI  D7
#define JPG_MISO  D6
#define JPG_SCK   D5
#define JPG_RST   -1
#define HEIGHT 320
#define WIDTH 240

int BITS_PER_PIXEL = 4; // 2^4 = 16 colors


#define BANNER_HEIGHT 16


// // define
#define TFT_WHITE   ILI9341_WHITE   /* 255, 255, 255 */
#define TFT_BLACK   ILI9341_BLACK   /*   0,   0,   0 */
#define TFT_RED     ILI9341_RED     /* 255,   0,   0 */
#define TFT_ORANGE  ILI9341_ORANGE  /* 255, 165,   0 */
#define TFT_YELLOW  ILI9341_YELLOW  /* 255, 255,   0 */
#define TFT_GREEN   ILI9341_GREEN   /*   0, 255,   0 */
#define TFT_CYAN    ILI9341_CYAN    /*   0, 255, 255 */
#define TFT_BLUE    ILI9341_BLUE    /*   0,   0, 255 */
#define TFT_MAGENTA ILI9341_MAGENTA /* 255,   0, 255 */

// defines the colors usable in the paletted 16 color frame buffer
uint16_t palette[] = {ILI9341_BLACK, // 0
                      ILI9341_WHITE, // 1
                      ILI9341_NAVY, // 2
                      ILI9341_DARKCYAN, // 3
                      ILI9341_DARKGREEN, // 4
                      ILI9341_MAROON, // 5
                      ILI9341_PURPLE, // 6
                      ILI9341_OLIVE, // 7
                      ILI9341_LIGHTGREY, // 8
                      ILI9341_DARKGREY, // 9
                      ILI9341_BLUE, // 10
                      ILI9341_GREEN, // 11
                      ILI9341_CYAN, // 12
                      ILI9341_RED, // 13
                      ILI9341_MAGENTA, // 14
                      ILI9341_YELLOW}; // 15
uint8_t scan_count = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// ILI9341_SPI grafx_driver = ILI9341_SPI(TFT_CS,TFT_DC);//, JPG_MOSI,JPG_SCK, JPG_RST, JPG_MISO);
XPT2046_Touchscreen ts(T_CS, TIRQ);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// ILI9341_SPI grafx_driver = ILI9341_SPI(TFT_CS, TFT_DC);
// MiniGrafx gfx = MiniGrafx(&grafx_driver, BITS_PER_PIXEL, palette);

void setup() {
  pinMode(LCD_PWR_PIN, OUTPUT);   // sets the pin as output
  digitalWrite(LCD_PWR_PIN, LOW); // PNP transistor on

  tft.begin();
  //
  tft.setRotation(1);
  //
  // tft.setTextSize(5);
  // tft.fillScreen(TFT_BLUE);
  // tft.setTextColor(TFT_WHITE, TFT_RED);
  // tft.setCursor(0, 50);
  //
  // tft.print(" Thank you for calling JPG Industries");
  // tft.setTextColor(TFT_WHITE, TFT_ORANGE);

  // gfx.setColor(0);
  //   // Fill the buffer with color 0 (Black)
  //   gfx.fillBuffer(0);
  // gfx.commit();

  delay(100);
}

void loop() {

  // clear old graph
  // tft.fillRect(0, BANNER_HEIGHT, 320, 224, TFT_BLACK);
  // tft.setTextSize(1);
  delay(500);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_MAGENTA,TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10,10);
  tft.print("test");

  tft.print(scan_count);

  // gfx.fillBuffer(0);
    // gfx.setColor(0);
    // gfx.drawLine(0, 0, 20, 20);
    // gfx.setColor(0);
    // gfx.fillCircle(20, 20, 5);
    // gfx.commit();

  // print WiFi stat
  // tft.setTextColor(TFT_WHITE);
  // tft.setCursor(0, BANNER_HEIGHT);
  // tft.print(" networks found, suggested channels: ");


  if (ts.tirqTouched()) {
    if (ts.touched()) {
    tft.setCursor(10,5);
    tft.setTextSize(.5);
      TS_Point p = ts.getPoint();
      tft.print("Pressure = ");
      tft.print(p.z);
      tft.print(", x = ");
      tft.print(p.x);
      tft.print(", y = ");
      tft.print(p.y);
      delay(30);
      // Serial.println();
    }
  }


  // Wait a bit before scanning again
  delay(5000);

  //POWER SAVING
  if (++scan_count >= SCAN_COUNT_SLEEP) {
    pinMode(LCD_PWR_PIN, INPUT);   // disable pin
    ESP.deepSleep(0);
  }
}
