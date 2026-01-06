#include "6_Control.h"
/*
* Operate a digital output to energize a Relay based on several inputs:
*  - Automatic operation by comparison of current temperature (T) with setpoint temperature (T_sp).
*  -  Setpoint (T_sp) is constrained between T_min and T_max.
*  - If T_sp is set as T_min, the heater will remain OFF, no matter what the actual temperature is
*  - If T_sp is set as T_max, the heater will remain ON, no matter what the actual temperature is
*/

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals

#if defined(local_enable) || defined(remote_enable)
  #include <Arduino.h>

  #include "1_Timers.h"
  #include "4_OLED.h"
  #include "3_BME280.h"

  //--------------------------------------------
  //  Global Variables
  //--------------------------------------------
  #ifdef local_enable
    bool local_adj  = 0   ; // Flag to indicate if user is manually adjusting setpoint
    bool adjusting  = 0   ; // Flag to indicate local adjustment still ongoing.
  #endif

  //--------------------------------------------
  //  Local Variables
  //--------------------------------------------
  static const float T_db  = 0.2 ; // deadband for temperature control (T_sp +/- T_db)
  static const float T_min =   5 ; // minimum selectable temperature setpoint
  static const float T_max =  30 ; // maximum selectable temperature setpoint

  // --------------------------------------------
  //  Thermostat control function
  // --------------------------------------------
  void thermostat() {
    //  ------ Perform thermostat control
    if (!man_auto) heater = man_command                     ; // In manual mode, heater state follows manual command.
    else {
      if      (T + T_db < T_sp || T_sp == T_max) heater = 1 ; // Heater ON  when temp. below "setpoint -deadband" or setpoint at MAX
      else if (T - T_db > T_sp || T_sp == T_min) heater = 0 ; // Heater OFF when temp. above "setpoint +deadband" or setpoint at MIN
      man_command = heater                                  ; // Update manual command to reflect actual heater state in auto mode
    }
    
    digitalWrite(pin_relay, heater)   ;   //  ------ Update relay output and screen if there is a change
    #ifdef debug
      Serial.println(F("Ctrl done ")) ;
    #endif
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()                ;
    #endif
  }

  //--------------------------------------------
  //  Local operation of thermostat (pushbuttons)
  //--------------------------------------------
  #ifdef local_enable
    void local_done() {
      thermostat()                      ; // Perform control action after local adjustment
      timer.enable(t_control)           ; // Re-enable periodic control
      #ifdef debug
        Serial.println(F("Local done")) ;
      #endif
    }

    /**
     * @brief Handle local thermostat operation using pushbuttons.
     * @details If no local action is detected (all buttons released), the local adjustment flag is reset after 5 seconds.
     *        If the auto/manual mode button is pressed, the mode is toggled.
     *        If the "-" button is pressed in manual mode, the heater is turned off.
     *        If the "+" button is pressed in manual mode, the heater is turned on.
     *        If the "-" button is pressed in auto mode, the temperature setpoint is decreased by 0.5 degrees.
     *        If the "+" button is pressed in auto mode, the temperature setpoint is increased by 0.5 degrees.
     *        The temperature setpoint is always constrained within the limits of T_min and T_max.
     *        The function also updates the display and restarts the Blynk update timer.
     */
    void local_adjust() {
      //--------------------------------------------
      //  Check for no local action
      //--------------------------------------------
      if (!digitalRead(pin_auto) && digitalRead(pin_raise) && digitalRead(pin_lower)) {
        if (adjusting) {
          t_resetAdjust = timer.setTimeout(Dt_short, local_done); // Reset "adjusting" flag after 5s of no local action
          adjusting = 0 ;
        }
        return;
      }

      #ifdef debug
        Serial.println(F("Local btn"))  ;
      #endif

      //--------------------------------------------
      //  Handle local adjustment timer
      //--------------------------------------------
      timer.deleteTimer(t_resetAdjust); // Delete previous "reset_adjusting" timer if any
      if (!adjusting) adjusting = 1   ; // Flag local adjustment ongoing.
      if (!local_adj) {
        local_adj = 1                 ; // Flag local adjustment for Blynk update.
        timer.disable(t_control)      ; // Disable periodic control during local adjustment
      }

      //--------------------------------------------
      //  Switch auto / manual mode
      //--------------------------------------------
      if (digitalRead(pin_auto)) {
        man_auto = !man_auto            ; // Toggle mode
        #ifdef debug
          Serial.print(F("Man/Auto> ")) ;
          Serial.println(man_auto)      ;
        #endif
        #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
          refresh_screen()              ;
        #endif
        return;                         // Exit to avoid multiple actions on same button press
      }

      //--------------------------------------------
      //  Manual operation
      //--------------------------------------------
      if (!man_auto) {
        if (!digitalRead(pin_lower)) {   // "-" button --> switch heater OFF
          man_command = 0               ;
          #ifdef debug
            Serial.println(F("M_OFF>")) ;
          #endif
          #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
            refresh_screen()            ;
          #endif
          return;                         // Exit to avoid multiple actions on same button press
        }
        else if (!digitalRead(pin_raise)) {   // "+" button --> switch heater ON
          man_command = 1               ;
          #ifdef debug
            Serial.println(F("M_ON>"))  ;
          #endif
          #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
            refresh_screen()            ;
          #endif
          return;                         // Exit to avoid multiple actions on same button press
        }
      }

      //--------------------------------------------
      //  Auto operation
      //--------------------------------------------
      else {
        if (!digitalRead(pin_lower))  {                 // "-" button --> decrease temperature setpoint
          T_sp = constrain(T_sp - 0.5, T_min, T_max)  ; // Adjust temp. setpoint within limits
          #ifdef debug
            Serial.print(F("T_down> ")) ;
            Serial.println(T_sp)        ;
          #endif
          #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
            refresh_screen()            ;
          #endif
          return                        ;
        }
        else if (!digitalRead(pin_raise))  {  // "+" button --> increase temperature setpoint
          T_sp = constrain(T_sp + 0.5, T_min, T_max)  ; // Adjust setpoint within limits
          #ifdef debug
            Serial.print(F("T_up> "))   ; 
            Serial.println(T_sp)        ;
          #endif
          #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
            refresh_screen()            ;
          #endif
          return                        ;
        }
      }
    }
  #endif

  void setup_control() {
    pinMode(pin_relay, OUTPUT)        ;
    #ifdef local_enable
      pinMode(pin_lower, INPUT_PULLUP);
      pinMode(pin_raise, INPUT_PULLUP);
      pinMode(pin_auto , INPUT)       ;
      t_local = timer.setInterval(Dt_local, local_adjust) ; // Periodically perform Local control (check pushbuttons);
    #endif
    #ifndef remote_enable                                   // Blynk takes care of control timer if remote enabled
      t_control = timer.setInterval(Dt_update, thermostat)   ; // Periodically perform thermostat control;
    #endif
  }

#endif  // local_enable || remote_enable