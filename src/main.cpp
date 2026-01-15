/* Wifi thermostat - Wemos D1 R1 or D1 mini.
 *
 * Board connected to a display, a climate sensor and a relay.
 * The relay output activates the heating system.
 * Screen: choose TFT screen (ST7789 or ST7735) or OLED screen (SSD1306)
 * Sensor: choose DHT11 or BME280 or thermistor sensor.
 * The library <dhtnew.h> is used to handle the DHT sensor.
 * The library <forcedClimate.h> is used to handle the BME sensor.
 * The library <SimpleTimer.h> is used to handle the timers for the periodic actions (sensing, control and remote update).
 *
 * (optional): Send and receive data to Blynk (operation and monitor in remote).
 * (optional): Retrieve time and date from a webserver using NTPclient library.
 * (optional): Retrieve time and date from Blynk cloud.
 * (optional): serial printer for debug and troubleshooting.
 *
 * Retrieve RH% and temperature from the DHT / BME sensor.
 * Display data on the screen.
 * Send a digital output to operate the relay.
 * The user can choose "Manual operation" (force ON / OFF) or "Auto operation" (heater ON when temperature below setpoint)
 * The user can change the temperature setpoint.
 */


//--------------------------------------------
//  Libraries
//--------------------------------------------

#include "0_Global_Config.h"    // Options and globals
// #ifdef remote_enable
//   #include <Blynk/BlynkTimer.h>   // Use BlynkTimer when Blynk is enabled
// #else
//   #include <SimpleTimer.h>        // Use SimpleTimer when Blynk is not enabled
// #endif

#include "1_Timers.h"        // Define timers.

#if defined(NTP_clock)
  #ifdef ARDUINO_ARCH_ESP32
    #include <WiFi.h>
  #endif
  #ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h> 
  #endif
  #include "2_Wifi_Connect.h" // Handle Wifi communication.
  #include "2_NTPclock.h"     // Handle NTP clock.
#endif

#if defined(Thermistor)
  #include "3_Thermistor.h"   
#elif defined(DHT_sensor)
  #include "3_DHT11.h"
#elif defined(BME_sensor)
  #include "3_BME280.h"
#elif defined(AHT_sensor)
  #include "3_AHT20.h"
#endif

#if defined(TFT_ST7735) || defined(TFT_ST7789)
  #include "4_TFT_screen.h"
#elif defined(OLED_SSD1306)
  #include "4_OLED.h"
#endif

#ifdef remote_enable
  #include "5_Blynk.h"
#endif

#if defined(local_enable) || defined(remote_enable)
  #include "6_Control.h"
#endif

//--------------------------------------------
//  Setup
//--------------------------------------------
void setup() {
  #ifdef debug
    Serial.begin(115200);
  #endif
  setup_sensor()        ;
  
  #if defined(local_enable) || defined(remote_enable)
    setup_control()     ;
  #endif

  #if defined(TFT_ST7735) || defined(TFT_ST7789) || defined(OLED_SSD1306)
    setup_screen()      ;
  #endif

  #ifdef NTP_clock
    ConnectWiFi_STA() ;
    setup_NTP()       ;
  #endif

  #ifdef remote_enable
    setup_Blynk()     ;
  #endif
}

//--------------------------------------------
//  Loop
//--------------------------------------------
void loop() {
  #ifdef remote_enable
    loop_Blynk() ;
  #endif
  loop_timer()  ;

//************** Reconnect WiFi if connection is lost
  #ifdef NTP_clock
    if (WiFi.status() != WL_CONNECTED)    reconnect_STA();
  #endif
}
