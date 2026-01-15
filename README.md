## Introduction
Board: Wifi thermostat - Wemos D1 R1 or D1 mini.
Initially coded in Arduino enviroment using VS code and ".hpp" files.
Updated to use ".h" / ".cpp" files

### Components
The board is connected to a display, a climate sensor and a relay.
  - The relay output activates the heating system (boiler).
  - Screen: choose TFT (ST7789 or ST7735) or OLED (SSD1306).
  - Sensor: choose DHT11 or BME280 or thermistor or AHT+BMP280.
  - Clock synchronization: choose NTP server or Blynk clock. 
  
  (optional): Send and receive data to Blynk (operation and monitor in remote mode). In this case, time and date can be syncrhonized with Blynk cloud.
  (optional): Retrieve time and date from a webserver using NTPclient library.
  (optional): serial printer for debug and troubleshooting.  
  
  Retrieve temperature (and RH& if applicable) from the DHT / BME sensor / thermistor.
  Display data on the screen.
  Send a digital output to operate the relay.
  The user can choose "Manual operation" (force ON / OFF) or "Auto operation" (heater ON when temperature below setpoint).
  The user can change the temperature setpoint.

### Libraries  
  - The library <dhtnew.h> is used to handle the DHT sensor: (https://github.com/RobTillaart/DHTNew)
  - The library <forcedClimate.h> is used to handle the BME sensor: (https://github.com/JVKran/Forced-BME280)
  - The libraries <AHT20.h> and <Wire.h> are used to handle AHT20 temperature and humidity sensor.
  - The library <Adafruit_BMP280.h> is used to handle BMP280 temperature and pressure sensor.
  - The library <SimpleTimer.h> is used to handle the timers for the periodic actions (sensing, local/remote control, updates...): (https://github.com/jfturcot/SimpleTimer)
  - The library <TimeLib.h> is used to ease handling of date: (https://github.com/PaulStoffregen/Time)
  - The libraries <Adafruit SSD1306> and <Adafruit GFX> are used to control the OLED screen via I2C: (https://github.com/adafruit/Adafruit_SSD1306), (https://github.com/adafruit/Adafruit-GFX-Library)
  - The library <Adafruit ST7735> is used to control the TFT screen (code loaded, but not tested): (https://github.com/adafruit/Adafruit-ST7735-Library)


- [SimpleTimer]
- [Time Library]
- [Adafruit SSD1306 OLED screen]
- [Adafruit GFX Library]
- [Adafruit ST7735 & ST7789 Library]
- [Forced BME280 sensor Library]
- [DHT New library]
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Blynk library](https://github.com/blynkkk/blynk-library)
