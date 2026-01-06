// Header
#include "3_BME280.h"

/*
 * In this version we are using the library <forcedClimate.h>, instead of the standard
 * libraries <Adafruit_Sensor.h> and <Adafruit_BME280.h>.
 * Refer to https://github.com/JVKran/Forced-BME280
 * This library automatically uses the forced mode of data retrieval
 *
 * I2C communication, SDA & SCL:
 *    SDA: GPIO4 = D2 (Wemos-D1-R2 or D1-mini) / D14 (Wemos D1-R1)
 *    SCL: GPIO5 = D1 (Wemos-D1-R2 or D1-mini) / D15 (Wemos D1-R1)
 *
 *  Variables T, P, H and fail are declared and defined in  0_Global_Config
 * 
 */

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals
#ifdef BME_sensor
  #include <forcedClimate.h>
  #include "1_Timers.h"

  // OPTIONAL: Create structure with data from BME sensor  *********
  // (Useful when sending data via RF)
  // typedef struct { float P = 0; float TA = 160; float HA = 160; } data;
  // data BME;

  //--------------------------------------------
  //  Create BME280 sensor (local only)
  //--------------------------------------------
  static ForcedClimate bme = ForcedClimate();

  //--------------------------------------------
  //  Read T, P, H
  //--------------------------------------------
  void read_sensor() {
    bme.takeForcedMeasurement()               ;   // Needed when in "Forced Mode"
    float read_T = bme.getTemperatureCelcius();
    float read_H = bme.getRelativeHumidity()  ;
    float pread = bme.getPressure()           ;

    //  ---- Filter and update values: ----------------------------
    if (read_T < -20 or read_T > 100.0 or read_H < 0 or read_H >100 or pread < 850 or pread > 1100) {
      #ifdef debug
        Serial.print(F("BME Fail. "));
      #endif
      fail = 1  ;
      return    ;
    }
    else {
      fail = 0  ;
      T = read_T ;
      H = read_H ;
      P = pread ;

      #ifdef debug
        // Serial.printf_P(PSTR("BME> %.1f *C | %.1f %% | %.2f mbar\n"), T, H, P);
        Serial.print(F("BME>"));
        Serial.print(T, 1); Serial.print(F(" *C | "));
        Serial.print(H); Serial.print(F(" % | "));
        Serial.print(P); Serial.println(F(" mbar"));
      #endif
    }

    /* OPTIONAL: Refresh the data structure (optional)
      BME.P = bme.readPressure() / 100.0  ;   // Converted to mbar
      BME.T = bme.readTemperature()       ;
      BME.H = bme.readHumidity()          ;
    */
  }

  //--------------------------------------------
  //  Setup
  //--------------------------------------------
  void setup_sensor() {
    bme.begin()                                         ;
    read_sensor()                                       ;
    t_sensor = timer.setInterval(Dt_sensor, read_sensor); // Periodically read sensor
  }

#endif // BME_sensor