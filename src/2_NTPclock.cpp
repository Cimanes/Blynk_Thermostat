#include "2_NTPclock.h"

/*
 * Ref: https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
 *
 * getDay() – returns an int number that corresponds to the the week day (0 to 6) starting on Sunday;
 * getHours() – returns an int number with the current hour (0 to 23) in 24 hour format;
 * getMinutes() – returns an int number with the current minutes (0 to 59);
 * getSeconds() – returns an int number with the current second;
 * getEpochTime() – returns an unsigned long with the epoch time (number of seconds that have elapsed since January 1, 1970 (midnight GMT);
 * getFormattedTime() – returns a String with the time formatted like hh:mm:ss;
 * getFormattedDate() – returns a String with the time formatted like YYYY-MM-DDThh:mm:ss;
 */

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals
#ifdef NTP_clock
  #include "1_Timers.h"
  #include <Timelib.h>
  #ifdef ARDUINO_ARCH_ESP32
    #include <WiFi.h>
  #endif
  #ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h> 
  #endif

  //--------------------------------------------
  //Variables
  //--------------------------------------------
  static time_t ntp_time; // this are the seconds since Epoch (1970) - UTC
  static struct tm tm   ; // the structure tm holds time information in a more convenient way *
  static bool sync_time = 0 ; // Flag to indicate time sync status

  // static const char weekDays[7][4]= {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  // static const char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  //--------------------------------------------
  //  Receive data from NTP and process
  //--------------------------------------------
  void get_Date() {
    time(&ntp_time)  ;
    localtime_r(&ntp_time, &tm); 
      const int yr = tm.tm_year + 1900 ;
      const byte mo = tm.tm_mon + 1    ;
      const byte dy = tm.tm_mday       ;
      const byte hh = tm.tm_hour       ;
      const byte mm = tm.tm_min        ;
      const byte ss = tm.tm_sec        ;
    //  const byte weekDay = tm.tm_wday      ;  
    //  const char *dow = weekDays[weekDay]  ;
    //  const char *monthName = months[mo-1] ;
    //  const byte DST = tm.tm_isdst         ;  // Daylight Saving Time flag

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

      if(yr > 2024 && !sync_time) {
        timer.deleteTimer(t_clock);
        t_clock = timer.setInterval(Dt_clock, get_Date) ; // Periodically refresh time
        sync_time = 1 ;
        #ifdef debug
          Serial.println(F("NTP sync'd"))  ;
        #endif
      }
  }


  void setup_NTP() {
    #ifdef ARDUINO_ARCH_ESP32 // ESP32 seems to be a little more complex:
      configTime(0, 0, MY_NTP_SERVER);  // 0, 0 because we will use TZ in the next line
      setenv("TZ", MY_TZ, 1);            // Set environment variable with your time zone
      tzset();
    #else // ESP8266
      configTime(MY_TZ, MY_NTP_SERVER);    // --> for the ESP8266 only
    #endif
    t_clock = timer.setInterval(Dt_short, get_Date) ; // Periodically refresh time
    #ifdef debug
      Serial.println("NTP configured")  ; 
    #endif
  }

#endif