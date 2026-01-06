#include "5_Blynk.h"

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals

#ifdef remote_enable
  #ifdef ARDUINO_ARCH_ESP32
    #include <WiFi.h>
  #endif
  #ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h> 
  #endif
  #include <BlynkSimpleEsp8266.h>
  #include <WidgetRTC.h>

  #include "1_Timers.h"   // t_clock, Dt_clock, timer 
  #include "3_BME280.h"   // T
  #include "4_OLED.h"     // refresh_screen()
  #include "6_Control.h"  // local_adj heater, man_auto, man_command

  #if defined(NTP_clock) && !defined(Blynk_clock)
    #include "2_NTPclock.h" // get_Date()
  #endif

  //--------------------------------------------
  //  Variables
  //--------------------------------------------
  const char auth[] = BLYNK_AUTH_TOKEN  ;
  WidgetRTC rtc         ; // Blynk real time clock
  static bool boiler   = 0 ; // Boiler state (0 = OFF, 1 = ON).
  static bool sum_win  = 0 ; // Summer / Winter mode (1 = Winter, 0 = Summer).

  #if defined(Cespedal) || defined(Toledo)
    #define vpin_temp V0      // Temperature reading.
    #define vpin_setpoint V1  // Thermostat setpoint.
    #define vpin_heater V2    // Heater feedback ON / OFF.
    #define vpin_auto V3      // Thermostat mode (MAN / AUTO).
    #define vpin_manrun V4    // Manual ON / Manual OFF command
    #define vpin_time V5      // Time feedback (from last connection)
    #define vpin_boiler V6    // Boiler power ON / OFF (1 = ON).
    // #define vpin_reset V7     //  Not used (reset using InternalPinDBG instead)
    #define vpin_period V8    // Control period (seconds).
    #define vpin_winter V9    // Summer / Winter operation mode 
  #endif

  #ifdef Barrioscuro
    #define vpin_temp V12      // Temperature reading.
    #define vpin_setpoint V13  // Thermostat setpoint.
    #define vpin_heater V14    // Heater feedback ON / OFF.
    #define vpin_auto V15      // Thermostat mode (MAN / AUTO).
    #define vpin_manrun V16    // Manual ON / Manual OFF command
    #define vpin_time V17      // Time feedback (from last connection)
    #define vpin_boiler V18    // Boiler power ON / OFF (1 = ON).
    // #define vpin_reset V7     //  Not used (reset using InternalPinDBG instead)
    #define vpin_period V20    // Control period (seconds).
    #define vpin_winter V21    // Summer / Winter operation mode
  #endif

  //--------------------------------------------
  //  Send data to Blynk
  //--------------------------------------------
  void send_Blynk() {
  // send readings from sensor to Blynk (<10 values per second).
    Blynk.virtualWrite(vpin_temp, T)              ;
    Blynk.virtualWrite(vpin_time, Time)           ;
    Blynk.virtualWrite(vpin_heater, heater)       ;
    Blynk.virtualWrite(vpin_manrun, man_command)  ;
    #ifdef debug
      // Serial.printf_P(PSTR("T> %.1f | Time> %s | Heater> %d | man_cmd> %d\n"), T, Time, heater, man_command) ;
      Serial.print(F("T> ")); Serial.print(T, 1)              ;
      Serial.print(F(" | Time> ")); Serial.print(Time)     ;
      Serial.print(F(" | Heater> ")); Serial.print(heater) ;
      Serial.print(F(" | man_cmd> ")); Serial.println(man_command) ;
    #endif
    if (local_adj) {
      Blynk.virtualWrite(vpin_setpoint, T_sp)     ;
      Blynk.virtualWrite(vpin_auto, man_auto)     ;
      #ifdef debug
        // Serial.printf_P(PSTR("T_sp> %.1f | Auto> %d\n"), T_sp, man_auto) ;
        Serial.print(F("T_sp> ")); Serial.print(T_sp, 1)        ;
        Serial.print(F(" | Auto> ")); Serial.println(man_auto)  ;
      #endif
      local_adj = 0 ;
    }
  }

  void control_Blynk() {
    thermostat();
    send_Blynk(); // Update Blynk with new values
    #ifdef debug
      Serial.println(F("Blynk updated "));
    #endif
  }

  //--------------------------------------------
  //  Handle changes in virtual pin values from Blynk
  //  Receive data from Blynk when these virtual pins change
  //--------------------------------------------
  BLYNK_WRITE(vpin_setpoint) {
    if(T_sp == param.asFloat()) return ; // No change
    #ifdef local_enable
      if (local_adj) return       ; // Ignore remote changes when local adjustment ongoing.
    #endif
    T_sp = param.asFloat()        ;
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()            ;
    #endif
    #ifdef debug
      Serial.print(F(">T_sp: ")) ;
      Serial.println(T_sp)        ;
    #endif
  }

  BLYNK_WRITE(vpin_auto) {
    if(man_auto == param.asInt()) return ; // No change
    #ifdef local_enable
      if (local_adj) return       ; // Ignore remote changes when local adjustment ongoing.
    #endif
    man_auto = param.asInt()      ;
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()            ;
    #endif
    #ifdef debug
      Serial.print(F(">Auto: ")) ;
      Serial.println(man_auto)    ;
    #endif
  }

  BLYNK_WRITE(vpin_manrun) {
    if(man_command == param.asInt()) return ; // No change
    #ifdef local_enable
      if (local_adj) return         ; // Ignore remote changes when local adjustment ongoing.
    #endif
    man_command = param.asInt()     ;
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()              ;
    #endif
    #ifdef debug
      Serial.print(F(">Man_cmd: "));
      Serial.println(man_command)   ;
    #endif
  }

  BLYNK_WRITE(vpin_boiler)   {
    if(boiler == param.asInt()) return ; // No change
    boiler = param.asInt()          ;
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()              ;
    #endif
    #ifdef debug
      Serial.print(F(">Boiler: ")) ;
      Serial.println(boiler)        ;
    #endif
  }

  BLYNK_WRITE(vpin_period) {               // Moved to control loop
    if(Dt_update == param.asInt()*1000) return ; // No change

    Dt_update = param.asInt()*1000      ;  // Convert to milliseconds.
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()                  ;
    #endif
    if(t_control >= 0 ) timer.deleteTimer(t_control);
    t_control = timer.setInterval(Dt_update, control_Blynk)  ; // Update control period
    #ifdef debug
      Serial.print(F(">Period: "))      ;
      Serial.println(Dt_update / 1000)  ;
    #endif
  }

  BLYNK_WRITE(vpin_winter)   {
    sum_win = param.asInt()         ;
    #if defined(OLED_SSD1306) || defined(TFT_ST7735) || defined(TFT_ST7789)
      refresh_screen()              ;
    #endif
    #ifdef debug
      Serial.print(F(">Sum_Win: "));
      Serial.println(sum_win)       ;
    #endif
  }

  BLYNK_WRITE(InternalPinDBG) {     // Internal pin = V255 -> used for special commands such as reboot device
    const char* cmd = param.asStr()                 ;
    if (strcmp(cmd, "reboot") == 0) {
      #ifdef debug
        Serial.println(F(">Rebooting..."))          ;
      #endif
      timer.setTimeout(500, [](){ ESP.restart(); }) ;
      for (;;) {}
    }
  }


  //--------------------------------------------
  //  RTC from blynk
  //--------------------------------------------
  #ifdef Blynk_clock
    // Obtain date and time from Blynk RTC and format it into Time[]
    void get_Date() {
      // using sprintf (slower method):
      // sprintf(Time,"%04d-%02d-%02d / %02d:%02d", year(), month(), day(), hour(), minute());

      // using direct char manipulation (faster method):
      const int yr = year();
      const byte mo = month();
      const byte dy = day();
      const byte hh = hour();
      const byte mm = minute(); 
      
      // "'0' +" converts int to char: '0' = ASCII 48. So '0' + 1 = ASCII 49 = char '1'
      Time[0] = '0' + (yr / 1000) % 10;
      Time[1] = '0' + (yr / 100)  % 10;
      Time[2] = '0' + (yr / 10)   % 10;
      Time[3] = '0' + (yr % 10);
      Time[4] = '-';
      Time[5] = '0' + (mo / 10);
      Time[6] = '0' + (mo % 10);
      Time[7] = '-';
      Time[8]  = '0' + (dy / 10);
      Time[9]  = '0' + (dy % 10);
      Time[10] = ' ';
      Time[11] = '/';
      Time[12] = ' ';
      Time[13] = '0' + (hh / 10);
      Time[14] = '0' + (hh % 10);
      Time[15] = ':';
      Time[16] = '0' + (mm / 10);
      Time[17] = '0' + (mm % 10);
      Time[18] = '\0';

      #ifdef debug
        Serial.print(F("Time> "));
        Serial.println(Time);
      #endif

      if(year() > 2024) {
        timer.deleteTimer(t_clock)                      ;
        t_clock = timer.setInterval(Dt_clock, get_Date) ;   // Change to "Slow" retrieve time after sync
        #ifdef debug
          Serial.println(F("date sync'd"))  ;
        #endif
      }
    }
  #endif

  //--------------------------------------------
  //  RTC (create Blynk Real Time Clock)
  //--------------------------------------------
    BLYNK_CONNECTED() { 
      #ifdef Blynk_clock
        Blynk.sendInternal("rtc", "sync") ; // Synchronize time on connection
      #endif
      Blynk.syncVirtual(vpin_setpoint)    ;
      Blynk.syncVirtual(vpin_auto)        ;
      Blynk.syncVirtual(vpin_manrun)      ;
      Blynk.syncVirtual(vpin_boiler)      ;
      Blynk.syncVirtual(vpin_winter)      ;

      #if defined(NTP_clock) || defined(Blynk_clock)
        get_Date()                          ;    
        Blynk.virtualWrite(vpin_time, Time) ;
      #endif
      Blynk.syncVirtual(vpin_period)      ; // Get control period from Blynk and start t_control (Dt_update) timer
    }           

  //--------------------------------------------
  //  Setup
  //--------------------------------------------
  void setup_Blynk() {
    Blynk.begin(auth, ssid, pass) ; // Connect with Blink Cloud
    rtc.begin()                   ; // Start RTC widget
    setSyncInterval(3600)         ; // re-synch every hour
    if(t_control < 0) t_control = timer.setInterval(Dt_update, control_Blynk)  ; // Start control timer
  // Option: you could specify server:
  // Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  }

  void loop_Blynk() {  Blynk.run(); }
  bool get_boiler() { return boiler; }
  bool get_sum_win() { return sum_win; }

#endif  // remote_enable