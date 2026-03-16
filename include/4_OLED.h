#pragma once
/*
 *  Monochrome OLED based on SSD1306 drivers
 *  128x64 pixel display using I2C to communicate
 *
 *  3 pins are required to interface (two I2C and one reset).
 *  If the screen has only 4 pins (VCC, GND, SDA, SCL),
 *  then the RESET pin is shared (use option "-1" for reset pin)
 *
 *  SDA: GPIO4 = D2 (Wemos-D1-R2 or D1-mini) / D14 (Wemos D1-R1)
 *  SCL: GPIO5 = D1 (Wemos-D1-R2 or D1-mini) / D15 (Wemos D1-R1)
 */
  //--------------------------------------------
  //  Configuration
  //--------------------------------------------
  // Number of pixels wide/high
  // Connected to I2C (SDA, SCL pins):
  #define ADDRESS 0x3C  // I2C address of the OLED screen (0x3C or 0x3D typical)
  #define OLED_RST -1   // Reset pin # ("4" Default in Arduino; "-1" if sharing Arduino reset pin)
  #define PIX_WIDE 128  // OLED screen wide
  #define PIX_HIGH 64   // OLED screen high// const int  OLED_RST =  -1 ; // Reset pin # ("4" Default in Arduino; "-1" if sharing Arduino reset pin)
  // const int OLED_RST = -1   ; // Reset pin # ("4" Default in Arduino; "-1" if sharing Arduino reset pin)
  // const byte PIX_WIDE = 128 ; // OLED screen wide
  // const byte PIX_HIGH =  64 ; // OLED screen high

  //--------------------------------------------
  //  Functions
  //--------------------------------------------
  void refresh_screen();
  void setup_screen();