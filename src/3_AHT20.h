#pragma once

// --- OPTIONS --------------------------------
#define Use_H   //  Uncomment to use Humidity sensor  
#define Use_P   //  Uncomment to use Pressure sensor  
#define Use_T_AHT   //  Uncomment to use Temperature reading from AHT20
// #define Use_T_BMP   //  Uncomment to use Temperature reading from BMP280

void read_sensor()  ;
void setup_sensor() ;