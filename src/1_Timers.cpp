#include "1_Timers.h"

//--------------------------------------------
//  Timers
//--------------------------------------------
AppTimer timer          ; //  Timer object (SimpleTimer or BlynkTimer).

int t_sensor = -1       ; //  Timer to read sensor periodically.
#if defined(remote_enable) || defined(local_enable)
  int t_control = -1    ; //  Timer for remote (Blynk) data retrieval and control.
#endif
#ifdef local_enable
  int t_local = -1      ; //  Timer for local control (pushbuttons). 
  int t_resetAdjust = -1; //  Timer to reset local adjustment flag.
#endif
#ifdef DHT_sensor
  int t_warmup = -1     ; //  Timer to allow warmup after power up.
#endif
int t_clock = -1        ; //  Timer for date & time refresh from Blynk or NTP.
int t_screen = -1       ; //  Timer screen refresh.

#if defined(local_enable) || defined(remote_enable)
  unsigned long Dt_update          =  60000UL  ; // Time delay to process remote control (adjustable from Blynk) (60s).
#endif

//--------------------------------------------
//  Timer function for loop
//--------------------------------------------
void loop_timer() { timer.run()  ; }