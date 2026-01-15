#include "3_AHT20.h"
/*
 * In this version we are using the library <dhtnew.h>, instead of the standard
 * library <DHT.h>.
 * Refer to https://github.com/RobTillaart/DHTNew
 * This library allows automatic sensortype recognition (DHT11 or DHT22)
 * It also allows to "powerDown" and "powerUp" the sensor when desired
 * Note: using auto-detect caused errors with DHT11. Recommended to set type explicitly.
 */

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals
#ifdef AHT_sensor

  #if defined(Use_T_AHT) || defined(Use_H)
    // #include <Wire.h>
    #include <AHT20.h>
  #endif
  #if defined(Use_P) || defined(Use_T_BMP)
    #include <Adafruit_BMP280.h>
  #endif
  #include "1_Timers.h"

  //--------------------------------------------
  //  Constants
  //--------------------------------------------
  static const float offset_T = 0.0; // Temperature offset to add to reading
  static const float gain_T   = 1.0; // Temperature offset to add to reading
  #ifdef Use_H
    static const float offset_H = 0.0; // Humidity offset    
    static const float gain_H   = 1.0; // Humidity gain
  #endif
  #ifdef Use_P
    static const float offset_P = 0.0; // Pressure offset
    static const float gain_P   = 1.0; // Pressure gain 
  #endif
                                        // Note: D5 NOT-OK in Wemos D1 R1

  //--------------------------------------------
  //  Define sensors
  //--------------------------------------------
  #if defined(Use_T_AHT) || defined(Use_H)
    static AHT20 aht;
  #endif
  #if defined(Use_P) || defined(Use_T_BMP)
    static Adafruit_BMP280 bmp; // I2C
  #endif

  void read_sensor() {
    #ifdef Use_T_AHT
      float read_T = gain_T * aht.getTemperature() + offset_T ;
    #endif
    #ifdef Use_H
      float read_H = gain_H * aht.getHumidity() + offset_H ;
    #endif
    #if defined(Use_P) || defined(Use_T_BMP)
      bmp.takeForcedMeasurement() ;
      #ifdef Use_T_BMP
        float read_T = gain_T * bmp.readTemperature() + offset_T ;
      #endif      
      #ifdef Use_P
        float read_P = gain_P * bmp.readPressure() + offset_P ;
      #endif
    #endif

    //  --------  Filter wrong values
    #ifdef Use_T_AHT
      if (isnan(read_T) || read_T < -20 || read_T > 100.0) {
        fail = 1                            ;
        #ifdef debug
          Serial.println(F("AHT T FAILED")) ;
        #endif
        return                              ;
      }
      else {
        T = read_T ;
        fail = 0   ;
        #ifdef debug
          Serial.print(F("T> "));
          Serial.print(T, 1); Serial.println(F(" *C"));
        #endif
      }
    #endif
    #ifdef Use_T_BMP
      if (isnan(read_T) || read_T < -20 || read_T > 100.0) {
        fail = 1                            ;
        #ifdef debug
          Serial.println(F("BMP T FAILED")) ;
        #endif
      }
      else {
        T = read_T ;
        fail = 0   ;
        #ifdef debug
          Serial.print(F("T> "));
          Serial.print(T, 1); Serial.println(F(" *C"));
        #endif
      }
    #endif
    #ifdef Use_H
      if (isnan(read_H) || read_H < 0 || read_H > 100) {
        // fail = 1                            ;
        #ifdef debug
          Serial.println(F("AHT RH FAILED"));
        #endif
      }
      else {
        H = read_H ;
        // fail = 0   ;
        #ifdef debug
          Serial.print(F("H> "));
          Serial.print(H, 1); Serial.println(F(" %"));
        #endif
      }
    #endif
    #ifdef Use_P
      if (isnan(read_P) || read_P < 85000 || read_P > 110000) {
        // fail = 1                            ;
        #ifdef debug
          Serial.println(F("BMP P FAILED")) ;
        #endif
      }
      else {
        P = read_P / 100.0 ;
        // fail = 0   ;
        #ifdef debug
          Serial.print(F("P> "));
          Serial.print(P, 0); Serial.println(F(" mbar"));
        #endif
      }
    #endif
  }

  void setup_sensor() {
    #if defined(Use_T_AHT) || defined(Use_H)
      Wire.begin();
      if (!aht.begin() ) {
        Serial.println(F("AHT20 not found"));
        while(true);
      }
    #endif
    #if defined(Use_P) || defined(Use_T_BMP)
      if (!bmp.begin()) {
        Serial.println(F("BMP280 not found"));
        while (1) delay(50);
      }
      bmp.setSampling(Adafruit_BMP280::MODE_FORCED,   /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    #endif
    read_sensor() ; // Initial read         ;
    t_sensor = timer.setInterval(Dt_sensor, read_sensor)  ; // Periodically read sensor
  }
#endif  // DHT_sensor