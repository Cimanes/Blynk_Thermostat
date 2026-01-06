#include "3_DHT11.h"
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
#ifdef DHT_sensor

  #include <dhtnew.h>
  #include "1_Timers.h"

  //--------------------------------------------
  //  Constants
  //--------------------------------------------
  static const float offset_H = -5.0; // Humidity offset to add to reading
  static const float offset_T = -1.8; // Temperature offset to add to reading
  static const float gain_H   =  1.0; // Humidity offset to add to reading
  static const float gain_T   =  1.0; // Temperature offset to add to reading
  static const byte  DHTPIN   =   13; // Data pin GPIO13 = D7 (Wemos D1 R1 / R2 / Mini)
                                        // Note: D5 NOT-OK in Wemos D1 R1

  //--------------------------------------------
  //  Define DHT sensor
  //--------------------------------------------
  DHTNEW dht(DHTPIN);

  void measure() {
    #ifdef debug
      Serial.print(F("Read DHT:")) ;
      Serial.println(dht.read())     ;
    #else
      dht.read()                     ;
    #endif

    float read_T = gain_T * dht.getTemperature() + offset_T ;
    float read_H = gain_H * dht.getHumidity()    + offset_H ;
    dht.powerDown()                                         ;

    //  --------  Filter wrong values
    if (isnan(read_H) or isnan(read_T) or read_T < -20
        or read_T > 50.0 or read_H < 0 or read_H > 100) {
      fail = 1                        ;
      #ifdef debug
        Serial.println(F("DHT FAILED")) ;
      #endif
      return                          ;
    }
    else {
      fail = 0    ;
      T = read_T  ;
      H = read_H  ;
      #ifdef debug
        Serial.print(F("DHT> "))                      ;
        Serial.print(T, 1); Serial.print(F(" *C | ")) ;
        Serial.print(H); Serial.println(F(" %"))      ; 
      #endif
    }
  }

  void read_sensor() {
    dht.powerUp()                                   ;
    #ifdef debug
      Serial.println(F("wake up sensor"))           ;
    #endif
    t_warmup = timer.setTimeout(Dt_warmup, measure) ;
  }

  void setup_sensor() {
    dht.setType(DHT_TYPE) ; // 11 = DHT11, 22 = DHT22 (auto-detect caused error)
    dht.powerUp()         ;
    delay(2000)           ;
    measure()             ;
    t_sensor = timer.setInterval(Dt_sensor, read_sensor)  ; // Periodically read sensor
  }
#endif  // DHT_sensor