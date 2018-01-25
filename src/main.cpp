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
#include <Button.hpp>

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

Adafruit_ILI9341* tft = new Adafruit_ILI9341(LCD_CS, LCD_DC);
XPT2046_Touchscreen ts(Touch_CS, Touch_IRQ);  // Param 2 - Touch IRQ Pin - interrupt enabled polling


bool update = true;
int updates = 0;
int screen_num = 0;

TS_Point last_touch;
void setup() {
  pinMode(LCD_PWR_PIN, OUTPUT);   // sets the pin as output
  digitalWrite(LCD_PWR_PIN, LOW); // PNP transistor on

  //
  tft->begin();
  ts.begin();
  ts.setRotation(2);
  tft->setRotation(4);

  tft->setFont(&FreeMonoBold9pt7b);

  // update = true;
  // last_touch = ts.getPoint();
  delay(100);
}

int draw_home(Adafruit_ILI9341* tft, TS_Point *last_touch) {
  int button = -1;

  //left side
  if (last_touch->x<2098) {

    if(last_touch->y<1266)      //top
    {
      button = 1;
      
    }else if(last_touch->y<2533)//mid
    {
      button = 3;
    }else
    {
      button = 5;
    }
    //right side
  } else {
    if(last_touch->y<1266)      //top
    {
      button = 2;
    }else if(last_touch->y<2533)//mid
    {
      button = 4;
    }else
    {
      button = 6;
    }
  }

  tft->fillScreen(ILI9341_BLACK);

  tft->setCursor(10,20);
  tft->print("home, selected: ");
  tft->println(button);

  tft->drawLine(120,0,120,340,ILI9341_WHITE);

  tft->drawLine(0,110,240,110,ILI9341_WHITE);
  tft->drawLine(0,220,240,220,ILI9341_WHITE);

  return 0;
}

void loop() {
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
          // pos = last_touch->x/16,last_touch->y/11;
        }
      }
      // }
    }

  if(update)
  {
    tft->setTextColor(ILI9341_MAGENTA,ILI9341_BLACK);
    switch (screen_num) {
      case 0://home screen
        screen_num = draw_home(tft,&last_touch);
      break;
      case 1://IOT screen
      break;
    }
    update = false;
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
