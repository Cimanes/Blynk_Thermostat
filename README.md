## Introduction
Board: Wifi thermostat - Wemos D1 R1 or D1 mini.
Initially coded in Arduino enviroment using VS code and ".hpp" files.
Updated to use ".h" / ".cpp" files

### Components
The board is connected to a display, a climate sensor and a relay.
  - The relay output activates the heating system (boiler).
  - Screen: choose TFT screen (ST7789 or ST7735) or OLED screen (SSD1306)
  - Sensor: choose DHT11 or BME280 or thermistor sensor.
  
  The library <dhtnew.h> is used to handle the DHT sensor.
  The library <forcedClimate.h> is used to handle the BME sensor.
  The library <SimpleTimer.h> is used to handle the timers for the periodic actions (sensing, control and remote update).
 
  (optional): Send and receive data to Blynk (operation and monitor in remote mode). In this case, retrieve time and date from Blynk cloud.
  (optional): Retrieve time and date from a webserver using NTPclient library.
  (optional): serial printer for debug and troubleshooting.
 
  Retrieve temperature (and RH& if applicable) from the DHT / BME sensor / thermistor.
  Display data on the screen.
  Send a digital output to operate the relay.
  The user can choose "Manual operation" (force ON / OFF) or "Auto operation" (heater ON when temperature below setpoint).
  The user can change the temperature setpoint.
 
### Libraries
- [SimpleTimer](https://github.com/jfturcot/SimpleTimer)
- [Time Library](https://github.com/PaulStoffregen/Time)
- [Adafruit SSD1306 OLED screen](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735 & ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library)
- [Forced BME280 sensor Library](https://github.com/JVKran/Forced-BME280)
- [DHT New library](https://github.com/RobTillaart/DHTNew)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Blynk library](https://github.com/blynkkk/blynk-library)
- [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA)