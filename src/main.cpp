/*
 * ESP8266 WiFi Analyzer
 * Revise from ESP8266WiFi WiFiScan example.
 * Require ESP8266 board support, Adafruit GFX and ILI9341 library.
 */
// Uncomment only the TFT model you are using
#define ILI9341

#include "ESP8266WiFi.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>
#include <Fonts\FreeMonoBold9pt7b.h>
#include "ILI9341_SPI.h" // Hardware-specific library
#include <MiniGrafx.h>

// #define D0    16
// #define D1    5
// #define D2    4
// #define D3    0
// #define D4    2
// #define D5    14
// #define D6    12
// #define D7    13
// #define D8    15
// #define D9    3
// #define D10   1

//POWER SAVING SETTING
#define SCAN_COUNT_SLEEP 150
#define PNP_PWR_TRANSISTOR

#define LCD_PWR_PIN 16 // For AZSMZ TFT 1.6X

#define LCD_DC     0 // For AZSMZ TFT 1.6X
#define LCD_CS     2 // For AZSMZ TFT 1.6X
// #define LCD_DC D2
// #define LCD_CS D1

#define Touch_CS  2
#define Touch_IRQ  4

// #define JPG_SS    D2
// #define JPG_MOSI  13
// #define JPG_MISO  12
// #define JPG_SCK   14
// #define JPG_RST   -1
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

Adafruit_ILI9341 tft = Adafruit_ILI9341(LCD_CS, LCD_DC);
XPT2046_Touchscreen ts(Touch_CS, Touch_IRQ);  // Param 2 - Touch IRQ Pin - interrupt enabled polling


bool update = true;
int updates = 0;
TS_Point last_touch;
// ILI9341_SPI grafx_driver = ILI9341_SPI(LCD_CS,LCD_DC);//, JPG_MOSI,JPG_SCK, JPG_RST, JPG_MISO);
// // ILI9341_SPI grafx_driver = ILI9341_SPI(LCD_CS, LCD_DC);
// MiniGrafx gfx = MiniGrafx(&grafx_driver, BITS_PER_PIXEL, palette);

void setup() {
  pinMode(LCD_PWR_PIN, OUTPUT);   // sets the pin as output
  digitalWrite(LCD_PWR_PIN, LOW); // PNP transistor on

  //
  tft.begin();
  ts.begin();
  ts.setRotation(2);
  tft.setRotation(4);

  // tft.setTextSize(5);
  // tft.fillScreen(TFT_BLUE);
  // tft.setTextColor(TFT_WHITE, TFT_RED);
  // tft.setCursor(0, 50);
  //
  // tft.print(" Thank you for calling JPG Industries");
  // tft.setTextColor(TFT_WHITE, TFT_ORANGE);

  // gfx.setColor(0);
  //   gfx.fillBuffer(0);
  // gfx.commit();
  update = true;
  last_touch = ts.getPoint();
  delay(100);
}

void loop() {
  // gfx.fillBuffer(0);
  //   gfx.setColor(0);
  //   gfx.drawLine(0, 0, 20, 20);
  //   gfx.setColor(0);
  //   gfx.fillCircle(20, 20, 5);
  //   gfx.commit();

  // clear old graph
  // tft.fillRect(0, BANNER_HEIGHT, 320, 224, TFT_BLACK);
  // tft.setTextSize(1);


  //
  // tft.print(scan_count);


  // print WiFi stat
  // tft.setTextColor(TFT_WHITE);
  // tft.setCursor(0, BANNER_HEIGHT);
  // tft.print(" networks found, suggested channels: ");

  if (ts.tirqTouched()) {
    // if (ts.touched()) {
      TS_Point curr_touch = ts.getPoint();
      if(curr_touch.z >2000){
        if (curr_touch.x != last_touch.x
          || curr_touch.y != last_touch.y
          || curr_touch.z != last_touch.z)
          {
            update = true;
            last_touch.x = curr_touch.x;
            last_touch.y = curr_touch.y;
            last_touch.z = curr_touch.z;
          }
      }
    // }
  }
    if(update)
    {

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_MAGENTA,TFT_BLACK);

      tft.setCursor(10,25);
      tft.setTextSize(.5);
      tft.setFont(&FreeMonoBold9pt7b);
      tft.print("Pressure = ");
      tft.println(last_touch.z);
      tft.print("x = ");
      tft.println(last_touch.x);
      tft.print("y = ");
      tft.println(last_touch.y);
      tft.print("updates: = ");
      tft.print(++updates);
      tft.fillCircle(last_touch.x/16,last_touch.y/11,4,TFT_WHITE);


      update = false;
      // Serial.println();
    }
    delay(50);


  // Wait a bit before scanning again
  // delay(5000);

  //POWER SAVING
  // if (++scan_count >= SCAN_COUNT_SLEEP) {
  //   pinMode(LCD_PWR_PIN, INPUT);   // disable pin
  //   ESP.deepSleep(0);
  // }
}
