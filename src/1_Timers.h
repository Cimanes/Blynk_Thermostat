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
/*
* Functions:
* SimpleTimer()       --> constructor to create the object
* setInterval(d, f)   --> Call function "f" periodically, every "d" ms. // d is "long".
* setTimeout(d, f)    --> Call function "f" only ONCE after "d" milliseconds. // d is "long"  // "f" is void().
* setTimer(d, f, n)   --> Call function "f" every "d" milliseconds for "n" times.
* enable(t_xxx)       --> Enables that timer.
* disable(t_xxx)      --> Disables that timer.
* toggle(t_xxx)       --> Toggles status of that timer (enable / disable).
* restartTimer(t_xxx) --> The timer starts counting from now, although the corresponding function has not been triggered (Watchdog...).
* deleteTimer(t_xxx)  --> Stop using this timer slot.
* n = getNumTimers()  --> Return the number of used slots in a timer object
*
* Define actions instead of calling function:
* t_flash = timer.setTimeout(d, []() { digitalWrite(flash_Pin, LOW);  } );
*/
extern AppTimer timer     ; //  Timer object.
extern int t_sensor       ; //  Timer to read sensor periodically.
extern int t_clock        ; //  Timer for date & time refresh from Blynk or NTP.
extern int t_screen       ; //  Timer screen refresh.

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

//--------------------------------------------
//  Timer intervals
//--------------------------------------------
static constexpr unsigned long Dt_clock  = 120000UL; // 120 s
static constexpr unsigned long Dt_sensor =  15000UL; // 180 s
static constexpr unsigned long Dt_short  =   4000UL; // 4 s

// time interval for local control (fixed, check for button pulses).
#ifdef local_enable
  static constexpr unsigned long Dt_local =  500UL ; 
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