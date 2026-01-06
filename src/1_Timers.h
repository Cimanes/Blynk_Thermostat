#pragma once

#include "0_Global_Config.h"    // Options and globals
#ifdef remote_enable
  #include <Blynk/BlynkTimer.h>   // Use BlynkTimer when Blynk is enabled
  using AppTimer = BlynkTimer   ; // BlynkTimer function to control all timer id's.
#else
  #include <SimpleTimer.h>        // Use SimpleTimer when Blynk is not enabled
  using AppTimer = SimpleTimer  ; // SimpleTimer function to control all timer id's.
#endif

//--------------------------------------------
//  Timers
//--------------------------------------------
extern AppTimer timer     ; //  Timer object.
extern int t_sensor       ; //  Timer to read sensor periodically.
#if defined(remote_enable) || defined(local_enable)
  extern int t_control    ; //  Timer for remote (Blynk) data retrieval and control.
#endif
#ifdef local_enable
  extern int t_local      ; //  Timer for local control (pushbuttons). 
  extern int t_resetAdjust; //  Timer to reset local adjustment flag.
#endif
#ifdef DHT_sensor
  extern int t_warmup     ; //  Timer to allow warmup after power up.
#endif
extern int t_clock        ; //  Timer for date & time refresh from Blynk or NTP.
extern int t_screen       ; //  Timer screen refresh.

//--------------------------------------------
//  Timer intervals
//--------------------------------------------
constexpr unsigned long Dt_clock  = 120000UL; // 120 s
constexpr unsigned long Dt_sensor = 15000UL; // 180 s
constexpr unsigned long Dt_short  =   4000UL; // 4 s

// time interval for local control (fixed, check for button pulses).
#ifdef local_enable
  constexpr unsigned long Dt_local =  500UL ; 
#endif

// Time delay to process thermostat control (dynamic, adjustable from Blynk) (60s).
#if defined(local_enable) || defined(remote_enable)
  extern unsigned long Dt_update; 
#endif

#ifdef DHT_sensor
  const int Dt_warmup   =   3000UL  ; // warm up timer for DHT11 temp sensor (3s).
#endif

//--------------------------------------------
//  Timer function for loop
//--------------------------------------------
void loop_timer()  ;