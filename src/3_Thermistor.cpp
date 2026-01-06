 #include "3_Thermistor.h"
 /*
 * Thermistor connection:
 * Voltage divider using a fixed resistor (user selectable resistance "Rf").
 * Thermistor connected to +3.3 VDC
 * Resistor in series and connected to ground
 * Connection resistor-thermistor will provide temperature analog input (channel "A0", user configurable).
 */

 //--------------------------------------------
 //  Libraries
 //--------------------------------------------
#include "0_Global_Config.h"    // Options and globals

 #ifdef Thermistor
  #include <Arduino.h>
  #include "1_Timers.h"
  //--------------------------------------------
  //  Constants
  //--------------------------------------------
  static const byte  tempPin   = A0          ;   // pin for Analog input signal from thermistor
  static const int   Rf        = 9900       ;   // Fixed resistor installed (Ohms)
  static const int   AI_Max    = 1023        ;   // Max value for analog read (1023 in Arduino/ESP8266; 4095 in ESP32)
  static const float A         = 1.125309e-3 ;   // Stainhart-hart "A" coefficient  -  0,001125308852122
  static const float B         = 2.34712e-4  ;   // Stainhart-hart "B" coefficient  -  0,000234711863267
  static const float C         = 8.76741e-8  ;   // Stainhart-hart "C" coefficient  -  0,000000085663516
  static const float offset_T  = -4          ;   // Temperature offset to add to reading

  //--------------------------------------------
  //  Variables
  //--------------------------------------------
  float Volt  = 512.0 ;   // Voltage reading from analog input
  float R     = 10000 ;   // Thermistor resistance (Ohms)

  void read_sensor() {
  // Measure
    Volt = analogRead(tempPin)    ;   // Voltage reading from the thermistor
    R    = Rf*(AI_Max / Volt - 1) ;   // Resistance of the thermistor

  // Calculation using Steinhart-Hart coefficients:
    T = offset_T + (1 / (A + B*log(R) + C*pow(log(R),3))) - 273.15 ;   // Temp ºC
    #ifdef debug
      // Serial.printf_P(PSTR("DHT Read - %.1f *C\n"), T);
      Serial.print(F("Thermistor> R = ")); 
      Serial.print(R, 0); Serial.print(F(" Ohms | "));
      Serial.print(T, 1); Serial.println(F(" *C")); 
    #endif
  }

  void setup_sensor() {
    read_sensor()                                       ;
    t_sensor = timer.setInterval(Dt_sensor, read_sensor); // Periodically read sensor
  }

#endif // Thermistor