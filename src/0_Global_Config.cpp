#include "0_Global_Config.h"    // Options and globals

//--------------------------------------------
//  Wifi credentials
//--------------------------------------------
#if defined(Cespedal) || defined(Barrioscuro)
  const char ssid[] = "Pepe_Cimanes";
  const char pass[] = "Cimanes7581" ;
#elif defined(Toledo)
  const char ssid[] = "MIWIFI_HtR7" ;
  const char pass[] = "TdQTDf3H"    ;
#endif

//--------------------------------------------
//  Global Variables
//--------------------------------------------
char Time[20]     ; // Formatted time (YYYY-MM-DD hh:mm).

// ------------ Sensor Variables ------------
#if defined (Thermistor) || defined(DHT_sensor) || defined(BME_sensor) || defined(AHT_sensor)
  bool fail   = 0 ; // Faulty reading detection (0 = OK, 1 = Failed reading).
  float T     = 0 ; // Temperature reading from sensor (ºC).

  #if defined(DHT_sensor)
    float H     = 0 ; // Relative humidity reading from sensor (RH%).
  #endif

  #if defined(BME_sensor) || defined(AHT_sensor)
    float H   = 0   ; // Relative humidity reading from sensor (RH%).
    float P   = 800 ; // Pressure reading from sensor (mbar).
  #endif
#endif

// ------------ Control Variables ------------
#if defined(local_enable) || defined(remote_enable)
  float T_sp       = 5  ; // Temperature setpoint for thermostat control (ºC).
  bool heater      = 0  ; // D.O. to energyze / de-energize the heating relay (OFF = 0 / ON = 1).
  bool man_auto    = 0  ; // Operation mode (Manual = 0; Auto = 1).
  bool man_command = 0  ; // Heater command in manual mode.
#endif
