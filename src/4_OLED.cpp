#include "4_OLED.h"
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
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals

#ifdef OLED_SSD1306             // Compile only if OLED selected
  #include <SPI.h>              // Arduino SPI library
  #include <Adafruit_GFX.h>     // Core graphics library
  #include <Adafruit_SSD1306.h> // Hardware-specific library for driver SSD1306

  #include "1_Timers.h"        // Dt_short, t_screen, timer
  #ifdef remote_enable
    #include "5_Blynk.h"       // get_boiler(), get_sum_win()
  #endif

  //--------------------------------------------
  //  Create my OLED Display
  //--------------------------------------------
  static Adafruit_SSD1306 OLED(PIX_WIDE, PIX_HIGH, &Wire, OLED_RST);

  //--------------------------------------------
  //  Functions
  //--------------------------------------------
  void refresh_screen() {
    OLED.clearDisplay()             ; // Clear the buffer
    OLED.setTextSize(1)             ; // Text size
    OLED.setTextColor(SSD1306_WHITE); // Text color

    OLED.setCursor(7, 2)            ;
    OLED.print(Time)                ;

    OLED.setCursor(30, 14)                      ;
    OLED.print(T<10 ? F("T    ") : F("T   "))   ;
    if (fail) OLED.print(F("---"))              ;
    else      OLED.print(T, 1)                  ;
    OLED.drawCircle(80, 14, 1, SSD1306_WHITE)   ;   // Degree symbol ( ° )
    OLED.print(F(" C"))                         ;

    OLED.setCursor(30, 28)                      ;
    OLED.print(F("SP  "))                       ;
    OLED.print(T_sp, 1)                         ;
    OLED.drawCircle(80, 28, 1, SSD1306_WHITE)   ;   // Degree symbol ( ° )
    OLED.print(F(" C"))                         ;
    
    OLED.setCursor(10, 41)                            ;
    OLED.print(F("Heater "))                          ;
    #if defined(remote_enable)
      OLED.print(heater && get_sum_win() ? F("ON") : F("OFF"));
    #else
      OLED.print(heater ? F("ON") : F("OFF"))         ;
    #endif
    OLED.print(man_auto ? F(" Aut ") : F(" Man "))    ;
    OLED.print(Dt_update/1000)                        ;
    OLED.print(F("s"))                                ;

    #ifdef remote_enable
      OLED.setCursor(20, 55)                          ;
      OLED.print(F("Boiler: "))                       ;
      OLED.print(get_boiler() ? F("ON / ") : F("OFF / "))     ;  
      OLED.print(get_sum_win() ? F("W") : F("S"))             ;
    #endif
    OLED.display()                  ;
    #ifdef debug
      Serial.println(F("OLED done"));
    #endif
  }


  void setup_screen(){
    Serial.println(F("OLED booting"))             ;
    if (!OLED.begin(SSD1306_SWITCHCAPVCC, ADDRESS)) {
      Serial.println(F("OLED screen not found"))  ;
    }
    else Serial.println(F("OLED initialized"))    ;
    OLED.display()                                ;
    t_screen = timer.setTimer(Dt_short, refresh_screen, 3) ; // Fast refresh during boot 3 times
  }

#endif // OLED_SSD1306