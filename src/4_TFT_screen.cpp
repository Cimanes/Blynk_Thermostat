#include "4_TFT_screen.h"

/*
 *
 *  NOT CONFIGURED !!
 *
 * Connection of TFT screen with driver ST7789 or ST7735
 * Based on Adafruit libraries
 * Connection using SPI protocol
 * 
  */

/*******************    SPI Connections
 * Connect SCK pin in TFT to SCK pin on board
 * Connect SDA pin in TFT to MOSI pin on board:
 *                      SCK     SDA(MOSI)
 * Arduino UNO          13      11
 * Arduino Mega         52      51
 * Arduino Nano         D13     D11
 * Arduino Pro Micro    15      16
 * Ard. Leonardo/Micro  ICSP-3  ICSP-4
 * ESP8266              GPIO14  GPIO13
 * Wemos D1 R1          D13     D11
 * Wemos D1 R2          D5      D7
 * Wemos D1 Mini        D5      D7
 * NodeMCU              D5      D7
 *
******************      Some 16-bit ('565') color settings:
 // Some 16-bit ('565') color settings:
 * ST77XX_BLACK 0x0000
 * ST77XX_WHITE 0xFFFF
 * ST77XX_RED 0xF800
 * ST77XX_GREEN 0x07E0
 * ST77XX_BLUE 0x001F
 * ST77XX_CYAN 0x07FF
 * ST77XX_MAGENTA 0xF81F
 * ST77XX_YELLOW 0xFFE0
 * ST77XX_ORANGE 0xFC00
 *
 *
 * ******************      Screen models:
 *  Use this initializer if using a 1.8" TFT screen:
 * tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
 *
 *  OR use this initializer if using a 1.44" TFT:
 * tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
 *
 *  OR use this initializer if using a 0.96" 180x60 TFT:
 * tft.initR(INITR_MINI160x80);  // Init ST7735S mini display
 *
 *  OR use this initializer if using a 1.54" 240x240 TFT:
 * tft.init(240, 240);           // Init ST7789 240x240
 *
 *  OR use this initializer if using a 2.0" 320x240 TFT:
 * tft.init(240, 320);           // Init ST7789 320x240
 *
 *****************
 *
 * For 1.44" and 1.8" TFT with ST7735 (including HalloWing) use:
 * Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
 *
 * For 1.3", 1.54", and 2.0" TFT with ST7789:
 * Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
*****************/


/********************************************************
// utility function for digital clock display: prints colon and leading 0
void digits(byte num){
  tft.print(F(":"))                 ;
  if(num < 10)    tft.print('0') ;
  tft.print(num,DEC)             ;
}
*********************************************************/

//--------------------------------------------
//  Libraries
//--------------------------------------------
#include "0_Global_Config.h"    // Options and globals

#if defined(TFT_ST7789) || defined(TFT_ST7735)  // Compile only if TFT selected
  #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
  #include "1_Timers.h"
  #include <Adafruit_GFX.h>    // Core graphics library
  #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
  //#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
  #include <SPI.h>             // Arduino SPI library

  //--------------------------------------------
  //  Define my TFT (ST7789 or ST7735)
  //--------------------------------------------
  #if defined(TFT_ST7735)
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  #elif defined(TFT_ST7789)
    Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
  #endif


  void refresh_screen() {
    tft.fillScreen(ST77XX_BLACK)            ;
    tft.setTextSize(3)                      ;
    tft.setCursor(32, 10)                   ;
    tft.setTextColor(ST77XX_WHITE)          ;
    tft.setCursor(20, 50)                   ;
    tft.setCursor(45, 50)                   ;
    tft.print(Time)                         ;
    tft.setCursor(160, 50)                  ;
    tft.print(dow)                          ;
    tft.setCursor(30, 100)                  ;
    tft.setTextColor(ST77XX_RED)            ;
    tft.print(F("T  "))                     ;
    tft.print(T, 1)                         ;
    tft.drawCircle(180, 105, 4, ST77XX_RED) ;   // Degree symbol ( ° )
    tft.print(F("  C"))                     ;
    tft.setCursor(10, 150)                  ;
    tft.setTextColor(ST77XX_GREEN)          ;
    tft.print(F("RH  "))                    ;
    tft.print(H, 1)                         ;
    tft.print(F("  %"))                     ;
    tft.setCursor(10, 200)                  ;
    tft.setTextColor(ST77XX_CYAN)           ;
    tft.print(F("P "))                      ;
    if (P < 1000)  tft.print(F(" "))        ;
    tft.print(P, 1)                         ;
    tft.print(F(" mbar"))                   ;

    #ifdef debug
      Serial.println(F("TFT done")) ;
    #endif

    // if (!sync_screen and sync_time) {
    //   timer.deleteTimer(t_screen)        ;
    //   sync_screen = 1                    ;
    // }
  }

  void setup_screen() {
    // if the display has CS pin try with SPI_MODE0
    tft.init(240, 240, SPI_MODE2)                     ; // Init ST7789 display 240x240 pixel
    tft.setRotation(2)                                ; // if the screen is flipped, remove this command
    tft.setTextWrap(false)                            ;
      t_screen = timer.setTimer(Dt_short, refresh_screen, 3) ; // Fast refresh during boot 3 times
    Serial.println(F("Initialized"))                  ;
  }
#endif