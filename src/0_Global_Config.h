#pragma once

//--------------------------------------------
//  Options
//--------------------------------------------
// Attribute to place functions in Flash memory
#define debug         //  Uncomment to allow serial print debug.
#define remote_enable //  Uncomment to enable remote operation(Blynk).
#define local_enable  //  Uncomment to enable local operation (push-buttons).

//**** Choose the time provider (NTP client or Blynk cloud).
#define Blynk_clock   //  Uncomment to enable Blynk clock refresh.
// #define NTP_clock   //  Uncomment to enable NTP Clock refresh.

//**** Choose the temperature sensor (Thermistor, DHT11 or BME280).
// #define Thermistor
// #define DHT_sensor
// #define BME_sensor
#define AHT_sensor

//**** Choose display type.
//#define TFT_ST7789
//#define TFT_ST7735
#define OLED_SSD1306

//**** Choose the location for wifi credentials.
#define Cespedal
// #define Barrioscuro
// #define Toledo

//--------------------------------------------
//  Global variables
//--------------------------------------------

// ------------ WiFi Credentials ------------
extern const char ssid[];
extern const char pass[];

// ------------ Global Variables ------------
extern char Time[20];

// ------------ Sensor Variables ------------
#if defined (Thermistor) || defined(DHT_sensor) || defined(BME_sensor) || defined(AHT_sensor)
  extern bool fail;
  extern float T;
#endif

#if defined(DHT_sensor)
  extern float H;
#endif

#if defined(BME_sensor) || defined(AHT_sensor)
  extern float H;
  extern float P;
#endif

// ------------ Control Variables ------------
#if defined(local_enable) || defined(remote_enable)
  extern float T_sp;
  extern bool heater;
  extern bool man_auto;
  extern bool man_command;
#endif


